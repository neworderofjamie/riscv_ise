#pragma once

// Standard C++ includes
#include <map>
#include <optional>
#include <set>
#include <string>
#include <thread>
#include <type_traits>
#include <variant>
#include <vector>

// Standard C includes
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

// Platform includes
#ifdef _WIN32
#include <intrin.h>
#endif

// Boost includes
#include <sha1.hpp>

// Common includes
#include "common/common_export.h"

//----------------------------------------------------------------------------
// Common::Utils
//----------------------------------------------------------------------------
namespace Common::Utils
{
//! Boilerplate for overloading base std::visit
template<class... Ts> struct Overload : Ts... { using Ts::operator()...; };
template<class... Ts> Overload(Ts...) -> Overload<Ts...>; // line not needed in

//! Divide two integers, rounding up i.e. effectively taking ceil
template<typename A, typename B, typename = std::enable_if_t<std::is_integral_v<A>&& std::is_integral_v<B>>>
constexpr inline auto ceilDivide(A numerator, B denominator)
{
    return ((numerator + denominator - 1) / denominator);
}

//! Pad an integer to a multiple of another
template<typename A, typename B, typename = std::enable_if_t<std::is_integral_v<A>&& std::is_integral_v<B>>>
constexpr inline auto padSize(A size, B blockSize)
{
    return ceilDivide(size, blockSize) * blockSize;
}

template<typename A, typename = std::enable_if_t<std::is_integral_v<A>>>
constexpr inline bool isPOT(A value)
{
    return (value & (value - 1)) == 0;
}

inline int clz(uint32_t value)
{
#ifdef _WIN32
    unsigned long leadingZero = 0;
    if (_BitScanReverse(&leadingZero, value)) {
        return 31 - leadingZero;
    }
    else {
        return 32;
    }
#else
    return __builtin_clz(value);
#endif
}

inline int ctz(uint32_t value)
{
#ifdef _WIN32
    unsigned long trailingZero = 0;
    if (_BitScanForward(&trailingZero, value)) {
        return trailingZero;
    }
    else {
        return 32;
    }
#else
    return __builtin_ctz(value);
#endif
}

inline int popCount(uint32_t value)
{
#ifdef _WIN32
    return __popcnt(value);
#else
    return __builtin_popcount(value);
#endif
}

inline void breakPoint()
{
#ifdef _WIN32
    __debugbreak();
#elif defined(__x86_64__) || defined(_M_X64)
    asm("int3");
#else
    __builtin_trap();
#endif
}

inline int16_t convertFixedPoint(double x, uint32_t fixedPoint)
{
    const double rounded = std::round(x * (1u << fixedPoint));
    assert(rounded >= std::numeric_limits<int16_t>::min());
    assert(rounded <= std::numeric_limits<int16_t>::max());

    return static_cast<int16_t>(rounded);
}

//! Hash arithmetic types and enums
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
inline void updateHash(const T& value, boost::uuids::detail::sha1& hash)
{
    hash.process_bytes(&value, sizeof(T));
}

//! Hash monostate
inline void updateHash(std::monostate, boost::uuids::detail::sha1&)
{
}

//! Hash strings
inline void updateHash(const std::string &string, boost::uuids::detail::sha1 &hash)
{
    updateHash(string.size(), hash);
    hash.process_bytes(string.data(), string.size());
}

//! Hash arrays of types which can, themselves, be hashed
template<typename T, size_t N>
inline void updateHash(const std::array<T, N> &array, boost::uuids::detail::sha1 &hash)
{
    updateHash(array.size(), hash);
    for(const auto &v : array) {
        updateHash(v, hash);
    }
}

//! Hash vectors of types which can, themselves, be hashed
template<typename T>
inline void updateHash(const std::vector<T> &vector, boost::uuids::detail::sha1 &hash)
{
    updateHash(vector.size(), hash);
    for(const auto &v : vector) {
        updateHash(v, hash);
    }
}

//! Hash vectors of bools
inline void updateHash(const std::vector<bool> &vector, boost::uuids::detail::sha1 &hash)
{
    updateHash(vector.size(), hash);
    for(bool v : vector) {
        updateHash(v, hash);
    }
}


//! Hash unordered maps of types which can, themselves, be hashed
template<typename K, typename V>
inline void updateHash(const std::map<K, V> &map, boost::uuids::detail::sha1 &hash)
{
    updateHash(map.size(), hash);
    for(const auto &v : map) {
        updateHash(v.first, hash);
        updateHash(v.second, hash);
    }
}

//! Hash unordered sets of types which can, themselves, be hashed
template<typename V>
inline void updateHash(const std::set<V> set, boost::uuids::detail::sha1 &hash)
{
    updateHash(set.size(), hash);
    for(const auto &v : set) {
        updateHash(v, hash);
    }
}

//! Hash optional types which can, themeselves, be hashed
template<typename T>
inline void updateHash(const std::optional<T> &optional, boost::uuids::detail::sha1 &hash)
{
    updateHash(optional.has_value(), hash);
    if (optional) {
        updateHash(optional.value(), hash);
    }
}

//! Hash variants of types which can, themeselves, be hashed
template<typename... T>
inline void updateHash(const std::variant<T...> &variant, boost::uuids::detail::sha1 &hash)
{
    updateHash(variant.index(), hash);
    std::visit(
        [&hash](const auto &v)
        {
            updateHash(v, hash);
        },
        variant);
}

//! Functor for generating a hash suitable for use in std::unordered_map etc (i.e. size_t size) from a SHA1 digests
struct SHA1Hash
{
    size_t operator()(const boost::uuids::detail::sha1::digest_type &digest) const
    {
        size_t hash;
        memcpy(&hash, &digest[0], sizeof(size_t));
        return hash;
    };
};

COMMON_EXPORT void setThreadName(std::thread& thread, const std::string& name);
}   // namespace Common::Utils