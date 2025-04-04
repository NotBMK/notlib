#ifndef INC__OBJECT_HELPER__H
#define INC__OBJECT_HELPER__H

#include <type_traits>
#include <iostream>
#include <source_location>

template <class T>
struct Singleton
{
public:

    static
    T&
    GetIns()
    {
        static T inst;
        return inst;
    }

protected:

    constexpr
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
};

template <typename T>
struct UnInit
{
    unsigned char self[sizeof(T)];

    T* get() noexcept { return (T*)self; }

    operator T& () noexcept { return *(this->get()); }
};

template <typename Class>
class View
{
protected:

    constexpr
    View() { }

public:

    template <typename T, T Class::* MemPtr>
    class Member
    {
    public:
    
        constexpr
        Member() = delete;
    
        constexpr inline
        T get() const noexcept
        {
            return ((Class*)this)->*MemPtr;
        }

        constexpr inline
        void set(const T& value) noexcept
        {
            ((Class*)this)->*MemPtr = value;
        }
    };

    template <typename T, T Class::* MemPtr>
    class Getter : public Member<T, MemPtr>
    {
    private:
        constexpr inline
        void set(const T& value) noexcept = delete;
    };

    template <typename T, T Class::* MemPtr>
    class Setter : public Member<T, MemPtr>
    {
    private:
        constexpr inline
        T get() const noexcept = delete;
    };

protected:

    template <typename T, T Class::* MemPtr>
    constexpr
    Member<T, MemPtr>* getMember() noexcept
    {
        return ((Member<T, MemPtr>*)this);
    }

    template <typename T, T Class::* MemPtr>
    constexpr
    Getter<T, MemPtr>* getGetter() const noexcept
    {
        return ((Getter<T, MemPtr>*)this);
    }

    template <typename T, T Class::* MemPtr>
    constexpr
    Setter<T, MemPtr>* getSetter() noexcept
    {
        return ((Setter<T, MemPtr>*)this);
    }
};

#endif