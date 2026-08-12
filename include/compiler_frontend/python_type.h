#pragma once

// Standard C++ includes
#include <string>
#include <type_traits>

//----------------------------------------------------------------------------
// CompilerFrontend::PythonType
//----------------------------------------------------------------------------
namespace CompilerFrontend::PythonType
{
constexpr int log2(size_t n, int k = 0) { return (n <= 1) ? k : log2(n >> 1, k + 1); }

template<typename T,
         typename std::enable_if<std::is_arithmetic_v<T>, T>::type* = nullptr>
std::string getPythonTypeString()
{
    constexpr int index
        = std::is_same<T, bool>::value
        ? 0
        : 1
        + (std::is_integral<T>::value
           ? log2(sizeof(T)) * 2 + std::is_unsigned<T>::value
           : 8
           + (std::is_same<T, double>::value        ? 1
              : std::is_same<T, long double>::value ? 2
              : 0));
    return std::string(1, "?bBhHiIqQfdg"[index]);
}
}