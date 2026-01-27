#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_ASSEMBLER_DLL
        #define ASSEMBLER_EXPORT __declspec(dllexport)
    #elif defined(LINKING_ASSEMBLER_DLL)
        #define ASSEMBLER_EXPORT __declspec(dllimport)
    #else
        #define ASSEMBLER_EXPORT
    #endif
#else
    #define ASSEMBLER_EXPORT
#endif