#ifndef INC__BUFFER__H
#define INC__BUFFER__H 1

#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <mutex>

namespace ntl
{
    class buffer
    {
        using value_type = unsigned char;
        using pointer = value_type*;

    public:

        buffer(const char* __file, size_t __chunk_size)
        : buffer(0, 0, __chunk_size, reinterpret_cast<pointer>(malloc(__chunk_size)), fopen(__file, "rb+"))
        { }

        buffer(buffer&& other)
        : buffer(other._M_chunk, other._M_pos, other._M_size, other._M_data, other._M_file)
        { new (&other) buffer(0, 0, -1, 0, 0); }

        ~buffer()
        {
            if (_M_data)
                delete[] _M_data;
            if (_M_file)
                fclose(_M_file);
        }

        void clear()
        {
            memset(_M_data, 0, _M_size);
        }

        void flush()
        {
            fseek(_M_file, _M_chunk * _M_size, SEEK_SET);
            fwrite(_M_data, _M_pos, 1, _M_file);
        }

        void putc(unsigned char ch)
        {
            if (_M_pos >= _M_size) flush();

            _M_data[_M_pos++] = ch;
        }

        // @brief Deleted copy constructor to prevent copying of buffer objects.
        buffer(const buffer&) = delete;
        // @brief Deleted copy assignment operator to prevent copying of buffer objects.
        buffer& operator= (const buffer&) = delete;
        // @brief Deleted move assignment operator to prevent moving of buffer objects.
        buffer& operator= (buffer&&) = delete; 

    private:

        buffer(size_t __chunk, size_t __pos, size_t __size, pointer __data, FILE* __file)
        : _M_chunk(__chunk), _M_pos(__pos), _M_size(__size), _M_data(__data), _M_file(__file)
        {
            if (__size != 0)
            {
                throw std::bad_alloc();
            }
        }

    protected:

        size_t _M_chunk;
        size_t _M_pos;
        size_t _M_size;
        pointer _M_data;
        FILE* _M_file;
        std::mutex _M_mu;
    };

} // namespace ntl


#endif