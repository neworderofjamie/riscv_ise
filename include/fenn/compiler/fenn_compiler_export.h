#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_FENN_COMPILER_DLL
        #define FENN_COMPILER_EXPORT __declspec(dllexport)
    #elif defined(LINKING_FENN_COMPILER_DLL)
        #define FENN_COMPILER_EXPORT __declspec(dllimport)
    #else
        #define FENN_COMPILER_EXPORT
    #endif
#else
    #define FENN_COMPILER_EXPORT
#endif