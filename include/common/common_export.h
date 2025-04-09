#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_COMMON_DLL
        #define COMMON_EXPORT __declspec(dllexport)
    #elif defined(LINKING_COMMON_DLL)
        #define COMMON_EXPORT __declspec(dllimport)
    #else
        #define COMMON_EXPORT
    #endif
#else
    #define COMMON_EXPORT
#endif