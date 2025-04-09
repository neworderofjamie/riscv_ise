#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_ISE_DLL
        #define ISE_EXPORT __declspec(dllexport)
    #elif defined(LINKING_ISE_DLL)
        #define ISE_EXPORT __declspec(dllimport)
    #else
        #define ISE_EXPORT
    #endif
#else
    #define ISE_EXPORT
#endif