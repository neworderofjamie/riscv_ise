#pragma once

// Platform includes
#ifdef _WIN32
#include <intrin.h>
#endif

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