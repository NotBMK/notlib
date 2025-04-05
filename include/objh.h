#ifndef INC__OBJECT_HELPER__H
#define INC__OBJECT_HELPER__H

#include <type_traits>
#include <iostream>
#include <string>

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

struct ReportInterface
{
public:

    static
    void report(const std::string& message, bool throw_msg = false)
    {
        return _S_do_report(message, throw_msg);
    }

protected:

    static
    void _S_do_report(const std::string& message, bool throw_msg)
    {
        std::string msg = _S_make_message("REPORT", message);
        if (throw_msg)
            throw std::runtime_error(msg);
        else
            std::cerr << msg << '\n';
    }

    static
    std::string _S_make_message(const std::string& hint, const std::string& message)
    {
        std::string msg;
        msg.push_back('[');
        msg += hint;
        msg.push_back(']');
        msg.push_back(' ');
        msg += message;
        return msg;
    }
};

#endif