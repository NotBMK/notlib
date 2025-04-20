#ifndef INC__NOTLIB_MEMORY__H
#define INC__NOTLIB_MEMORY__H 1

#include <string>
#include <cstring>

namespace ntl
{
    class memory
    {
    public:

        using value_type = unsigned char;
        using pointer = value_type*;
        using iterator = pointer;
        using const_iterator = const pointer;
        using ptrdiff_t = std::ptrdiff_t;

    public:

        memory(size_t __size)
        : memory(__size, reinterpret_cast<pointer>(malloc(__size)))
        { }

        memory(memory&& other)
        : memory(other._M_size, other._M_data)
        { new (&other) memory(0, nullptr); }
        
        memory& operator= (memory&& other)
        {
            this->~memory();
            return *new (this) memory(std::move(other));
        }
        
        ~memory()
        { if(_M_data) delete[] _M_data; }

        memory(const memory& other) = delete;
        memory& operator= (const memory& other) = delete;

        void clear()
        { memset(_M_data, 0, _M_size); }

        size_t size() const noexcept
        { return _M_size; }

        pointer data() noexcept
        { return _M_data; }

        const pointer data() const noexcept
        { return _M_data; }

        unsigned char at(size_t __offset) const noexcept
        { return _M_data[__offset]; }

        unsigned char& at(size_t __offset) noexcept
        { return _M_data[__offset]; }

        unsigned char operator[] (size_t __offset) const noexcept
        { return _M_data[__offset]; }

        unsigned char& operator[] (size_t __offset) noexcept
        { return _M_data[__offset]; }

        iterator begin() noexcept
        { return _M_data; }

        const_iterator begin() const noexcept
        { return _M_data; }

        const_iterator cbegin() const noexcept
        { return _M_data; }

        iterator end() noexcept
        { return _M_data + _M_size; }

        const_iterator end() const noexcept
        { return _M_data + _M_size; }

        const_iterator cend() const noexcept
        { return _M_data + _M_size; }

        // @return 0 if there is insufficient memory; no changes are made in this case.
        size_t write(const void* __src, size_t __size, size_t __offset)
        {
            if (__offset + __size > _M_size)
                return 0;
            memcpy(_M_data + __offset, __src, __size);
            return __size;
        }

        // @return 0 if there is insufficient memory; no changes are made in this case.
        size_t write_uint64(unsigned long long __value, size_t __offset)
        {
            return write(&__value, 8, __offset);
        }

        // @return 0 if there is insufficient memory; no changes are made in this case.
        size_t write_string(const std::string& __value, size_t __offset)
        {
            if (__offset + __value.size() + 8 > _M_size)
                return 0;
            return write_uint64(__value.size(), __offset) + write(__value.data(), __value.size(), __offset + 8);
        }

        void read(void* __dst, size_t __size, size_t __offset) const
        {
            memcpy(__dst, _M_data + __offset, __size);
        }

        size_t read_uint64(size_t __offset) const
        {
            size_t num;
            read(&num, 8, __offset);
            return num;
        }

        std::string read_string(size_t __offset) const
        {
            size_t size = read_uint64(__offset);
            std::string str(size+1, '\0');
            read(str.data(), size, __offset+8);
            return str;
        }

        void dump(size_t __offset, size_t __count, FILE* __dst) const
        {
            fwrite(_M_data + __offset, __count, 1, __dst);
        }

        void dump(FILE* __dst) const
        {
            fwrite(_M_data, _M_size, 1, __dst);
        }
        
    private:

        memory(size_t __size, pointer __ptr)
        : _M_size(__size), _M_data(__ptr)
        { }

    protected:

        size_t          _M_size;
        unsigned char*  _M_data;
    };
} // namespace ntl


#endif