#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_COMPILER_DLL
        #define COMPILER_EXPORT __declspec(dllexport)
    #elif defined(LINKING_COMPILER_DLL)
        #define COMPILER_EXPORT __declspec(dllimport)
    #else
        #define COMPILER_EXPORT
    #endif
#else
    #define COMPILER_EXPORT
#endif