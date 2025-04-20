#ifndef INC__TYPE_DEFS__H
#define INC__TYPE_DEFS__H

#include <iostream>

namespace ntl
{
    template <typename UnderType>
    class byte_type;

    template <typename UnderType, typename __Ty>
    struct __operand_traits;

    template <typename UnderType> struct __operand_traits<UnderType, bool> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, char> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, signed char> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, unsigned char> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, wchar_t> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, short> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, unsigned short> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, int> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, unsigned int> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, long> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, unsigned long> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, long long> { using type = byte_type<UnderType>; };
    template <typename UnderType> struct __operand_traits<UnderType, unsigned long long> { using type = byte_type<UnderType>; };

    template <typename UnderType, typename IntType>
    struct __operand_traits<UnderType, const IntType> : __operand_traits<UnderType, IntType> { };
    template <typename UnderType, typename IntType>
    struct __operand_traits<UnderType, volatile IntType> : __operand_traits<UnderType, IntType> { };
    template <typename UnderType, typename IntType>
    struct __operand_traits<UnderType, const volatile IntType> : __operand_traits<UnderType, IntType> { };

    template <typename ValueType>
    class byte_type final
    {
        template <typename IntType>
        using operand_type = typename __operand_traits<ValueType, IntType>::type;
        using value_type = ValueType;

    public:

        constexpr inline
        byte_type() : _M_value(0) { }
        byte_type(const byte_type& other) : _M_value(other._M_value) { }
        template <typename IntType>
        byte_type(IntType __value)
        : _M_value(static_cast<value_type>(__value))
        { }

        friend std::ostream& operator<< (std::ostream& os, const byte_type& byte)
        {
            return os << std::hex << static_cast<unsigned long long>(byte._M_value);
        }

        constexpr inline
        operator value_type() const noexcept
        { return _M_value; }

        constexpr inline
        value_type value() const noexcept
        { return _M_value; }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator= (IntType __value) noexcept
        { _M_value = static_cast<value_type>(__value); return *this; }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator&= (IntType __value) noexcept
        { _M_value &= static_cast<value_type>(__value); return *this; }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator|= (IntType __value) noexcept
        { _M_value |= static_cast<value_type>(__value); return *this; }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator^= (IntType __value) noexcept
        { _M_value ^= static_cast<value_type>(__value); return *this; }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator>>= (IntType __value) noexcept
        { _M_value >>= static_cast<value_type>(__value); return *this; }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator<<= (IntType __value) noexcept
        { _M_value <<= static_cast<value_type>(__value); return *this; }

        template <typename IntType>
        constexpr inline
        operand_type<IntType> operator& (IntType __value) const noexcept
        { return _M_value & static_cast<value_type>(__value); }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator| (IntType __value) const noexcept
        { return _M_value | static_cast<value_type>(__value); }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator^ (IntType __value) const noexcept
        { return _M_value ^ static_cast<value_type>(__value); }

        template <typename IntType>
        constexpr inline
        friend operand_type<IntType> operator& (IntType __value, byte_type __byte) noexcept
        { return __byte.operator& (__value); }

        template <typename IntType>
        constexpr inline
        friend operand_type<IntType>& operator| (IntType __value, byte_type __byte) noexcept
        { return __byte.operator| (__value); }

        template <typename IntType>
        constexpr inline
        friend operand_type<IntType>& operator^ (IntType __value, byte_type __byte) noexcept
        { return __byte.operator& (__value); }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator>> (IntType __value) const noexcept
        { return _M_value >> static_cast<value_type>(__value); }

        template <typename IntType>
        constexpr inline
        operand_type<IntType>& operator<< (IntType __value) const noexcept
        { return _M_value << static_cast<value_type>(__value); }

    private:

        value_type _M_value;
    };

    using byte = byte_type<unsigned char>;
    using word = byte_type<unsigned short>;
    using dword = byte_type<unsigned int>;
    using qword = byte_type<unsigned long long>;

} // namespace ntl

#endif