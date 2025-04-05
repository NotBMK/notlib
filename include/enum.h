#ifndef INC__ENUM__H
#define INC__ENUM__H

#include <type_traits>

template <typename T, bool isEnum = std::is_enum<T>::value>
class Enum;

template <typename T>
class Enum<T, true>
{
public:

    constexpr
    Enum()
    { }

    constexpr
    Enum(T en)
    : value(en) { }

    constexpr
    Enum(const Enum& en)
    : value(en.value) { }

    constexpr
    operator bool () const noexcept
    {
        return static_cast<bool>(this->value);
    }

    constexpr
    operator T () const noexcept
    {
        return this->value;
    }

    constexpr
    Enum& operator= (const T& en) noexcept
    {
        this->value = en;
        return *this;
    }
    constexpr
    Enum& operator= (const Enum& en) noexcept
    {
        this->value = en.value;
        return *this;
    }

    constexpr
    bool operator== (const T& en) noexcept
    {
        return this->value == en;
    }
    constexpr
    bool operator== (const Enum& en) noexcept
    {
        return this->value == en.value;
    }

    constexpr
    bool operator!= (const T& en) noexcept
    {
        return this->value != en;
    }
    constexpr
    bool operator!= (const Enum& en) noexcept
    {
        return this->value != en.value;
    }

    constexpr
    Enum operator~ () const noexcept
    {
        Enum ret;
        ret.value = static_cast<T>(~static_cast<unsigned long>(this->value));
        return ret;
    }

    constexpr
    Enum operator| (const T& en) const noexcept
    {
        Enum ret;
        ret.value = static_cast<T>(static_cast<unsigned long>(this->value) | static_cast<unsigned long>(en));
        return ret;
    }
    constexpr
    Enum operator| (const Enum& en) const noexcept
    {
        Enum ret;
        ret.value = static_cast<T>(static_cast<unsigned long>(this->value) | static_cast<unsigned long>(en.value));
        return ret;
    }
    constexpr
    Enum& operator|= (const T& en) noexcept
    {
        this->value = static_cast<T>(static_cast<unsigned long>(this->value) | static_cast<unsigned long>(en));
        return *this;
    }
    constexpr
    Enum& operator|= (const Enum& en) noexcept
    {
        this->value = static_cast<T>(static_cast<unsigned long>(this->value) | static_cast<unsigned long>(en.value));
        return *this;
    }

    constexpr
    Enum operator& (const T& en) const noexcept
    {
        Enum ret;
        ret.value = static_cast<T>(static_cast<unsigned long>(this->value) & static_cast<unsigned long>(en));
        return ret;
    }
    constexpr
    Enum operator& (const Enum& en) const noexcept
    {
        Enum ret;
        ret.value = static_cast<T>(static_cast<unsigned long>(this->value) & static_cast<unsigned long>(en.value));
        return ret;
    }
    constexpr
    Enum& operator&= (const T& en) noexcept
    {
        this->value = static_cast<T>(static_cast<unsigned long>(this->value) & static_cast<unsigned long>(en));
        return *this;
    }
    constexpr
    Enum& operator&= (const Enum& en) noexcept
    {
        this->value = static_cast<T>(static_cast<unsigned long>(this->value) & static_cast<unsigned long>(en.value));
        return *this;
    }

    constexpr
    Enum operator^ (const T& en) const noexcept
    {
        Enum ret;
        ret.value = static_cast<T>(static_cast<unsigned long>(this->value) ^ static_cast<unsigned long>(en));
        return ret;
    }
    constexpr
    Enum operator^ (const Enum& en) const noexcept
    {
        Enum ret;
        ret.value = static_cast<T>(static_cast<unsigned long>(this->value) ^ static_cast<unsigned long>(en.value));
        return ret;
    }
    constexpr
    Enum& operator^= (const T& en) noexcept
    {
        this->value = static_cast<T>(static_cast<unsigned long>(this->value) ^ static_cast<unsigned long>(en));
        return *this;
    }
    constexpr
    Enum& operator^= (const Enum& en) noexcept
    {
        this->value = static_cast<T>(static_cast<unsigned long>(this->value) ^ static_cast<unsigned long>(en.value));
        return *this;
    }

    template <typename EnumType>
    friend constexpr
    Enum<EnumType, true> operator| (const EnumType& enum1, const EnumType& enum2) noexcept;

    template <typename EnumType>
    friend constexpr
    Enum<EnumType, true> operator& (const EnumType& enum1, const EnumType& enum2) noexcept;

    template <typename EnumType>
    friend constexpr
    Enum<EnumType, true> operator^ (const EnumType& enum1, const EnumType& enum2) noexcept;

    template <typename EnumType>
    friend constexpr
    Enum<EnumType, true> operator~ (const EnumType& en) noexcept;

private:

    T value;
};

template <typename EnumType>
constexpr
Enum<EnumType, true> operator| (const EnumType& enum1, const EnumType& enum2) noexcept
{
    return Enum<EnumType, true>(enum1) | enum2;
}

template <typename EnumType>
constexpr
Enum<EnumType, true> operator& (const EnumType& enum1, const EnumType& enum2) noexcept
{
    return Enum<EnumType, true>(enum1) & enum2;
}

template <typename EnumType>
constexpr
Enum<EnumType, true> operator^ (const EnumType& enum1, const EnumType& enum2) noexcept
{
    return Enum<EnumType, true>(enum1) ^ enum2;
}

template <typename EnumType>
constexpr
Enum<EnumType, true> operator~ (const EnumType& en) noexcept
{
    return ~Enum<EnumType, true>(en);
}

#endif