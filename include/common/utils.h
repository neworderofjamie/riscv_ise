#pragma once

// Standard C++ includes
#include <string>
#include <thread>

// Standard C includes
#include <cassert>
#include <cmath>
#include <cstdint>

// Platform includes
#ifdef _WIN32
#include <intrin.h>
#endif

// Common includes
#include "common/common_export.h"

//----------------------------------------------------------------------------
// Common::Utils
//----------------------------------------------------------------------------
namespace Common::Utils
{
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

COMMON_EXPORT void setThreadName(std::thread& thread, const std::string& name);
}   // namespace Common::Utils