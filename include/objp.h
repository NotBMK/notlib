#ifndef INC__OBJECT_POOL__H
#define INC__OBJECT_POOL__H

#include <objh.h>
#include <array>
#include <vector>
#include <utility>

enum PoolConfig : unsigned char
{
    EmptyConfig = 0,
    DynamicPage = 0b0000'0001,
    UserRelease = 0b0000'0010,
};

constexpr PoolConfig operator| (PoolConfig c1, PoolConfig c2)
{
    return static_cast<PoolConfig>(static_cast<unsigned long>(c1) | static_cast<unsigned long>(c2));
}

template <typename Object, typename IDIntType = std::size_t, IDIntType PoolPageSize = 32lu, PoolConfig Config = EmptyConfig>
class Poolize
{
protected:

    class PoolManager final : public Singleton<PoolManager>
    {
        friend class Poolize;
        friend class Singleton<PoolManager>;
        using PoolPage = std::array<UnInit<Object>, PoolPageSize>;

    private:

        void _M_AllocatePage()
        {
            PoolPage* page = new PoolPage();
            _M_pool.push_back(page);
            for (IDIntType i = _M_allocated_id.size(), end = i + PoolPageSize; i < end; ++i)
            {
                _M_allocated_id.emplace_back(i, i);
            }
        }

        Object* _M_AllocateObject(IDIntType* ID = nullptr)
        {
            if (_M_active_id_amount >= _M_allocated_id.size())
            {
                if constexpr (Config & DynamicPage)
                {
                    _M_AllocatePage();
                }
                else
                {
                    return nullptr;
                }
            }
            IDIntType id = _M_allocated_id[_M_active_id_amount].first;
            Object* obj = getByID(id);
            _M_allocated_id[id].second = _M_active_id_amount++;
            if (ID) *ID = id;
            return obj;
        }

        void _M_Deallocate(Object* obj)
        {
            if constexpr (!(Config & UserRelease))
                obj->~Object();
            IDIntType id = obj->ID();
            IDIntType index = _M_allocated_id[id].second;
            if (_M_active_id_amount && index != --_M_active_id_amount)
            {
                IDIntType last = _M_allocated_id[_M_active_id_amount].first;
                std::swap(_M_allocated_id[index].first, _M_allocated_id[_M_active_id_amount].first);
                std::swap(_M_allocated_id[id].second, _M_allocated_id[last].second);
            }
        }

        PoolManager()
        : _M_pool(), _M_active_id_amount(0)
        {
            _M_AllocatePage();
        }

        ~PoolManager()
        {
            for (IDIntType i = 0, end = _M_active_id_amount; i < end; ++i)
            {
                getByID(_M_allocated_id[i].first)->~Object();
            }
            for (PoolPage* page : _M_pool)
            {
                delete page;
            }
        }

    public:

        Object* getByID(IDIntType ID)
        {
            if constexpr (Config & DynamicPage)
            {
                IDIntType page = ID / PoolPageSize;
                IDIntType idx = ID % PoolPageSize;
                return _M_pool[page]->at(idx).get();
            }
            else
            {
                return _M_pool[0]->at(ID).get();
            }
        }

        template <typename... Args>
        Object* create(Args... args)
        {
            IDIntType id;
            Object* obj = _M_AllocateObject(&id);
            if (obj)
            {
                new (obj) Object(args...);
                ((Poolize*)obj)->_M_ID = id;
            }
            return obj;
        }

        void destroy(Object* obj)
        {
            _M_Deallocate(obj);
        }

        size_t count() const noexcept
        {
            return _M_active_id_amount;
        }

        size_t capacity() const noexcept
        {
            return _M_allocated_id.size();
        }

        bool empty() const noexcept
        {
            return !!_M_active_id_amount;
        }

    private:

        std::vector<PoolPage*> _M_pool;
        std::vector<std::pair<IDIntType, IDIntType>> _M_allocated_id;
        IDIntType _M_active_id_amount;
    };

    template <typename... Args>
    [[deprecated]]
    static Object* Init(Object* obj, Args... args)
    {
        return new (obj) Object(args...);
    };

public:

    static PoolManager& Manager()
    {
        return PoolManager::GetIns();
    }

    template <IDIntType PoolID>
    static PoolManager& GetPool()
    {
        if constexpr (PoolID == 0)
        {
            return Manager();
        }
        else
        {
            static PoolManager inst;
            return inst;
        }
    }

protected:

    Poolize() = default;
    ~Poolize() = default;

    const IDIntType& ID() const { return _M_ID; }

private:

    IDIntType _M_ID;
};

#endif