#pragma once

// Platform includes
#ifdef _WIN32
#include <intrin.h>
#endif


//! Divide two integers, rounding up i.e. effectively taking ceil
template<typename A, typename B, typename = std::enable_if_t<std::is_integral_v<A> && std::is_integral_v<B>>>
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


inline int clz(uint32_t value)
{
#ifdef _WIN32
    unsigned long leadingZero = 0;
    if(_BitScanReverse(&leadingZero, value)) {
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
    if(_BitScanForward(&trailingZero, value)) {
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
#else
    asm("int3");
#endif
}