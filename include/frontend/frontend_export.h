#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_FRONTEND_DLL
        #define FRONTEND_EXPORT __declspec(dllexport)
    #elif defined(LINKING_FRONTEND_DLL)
        #define FRONTEND_EXPORT __declspec(dllimport)
    #else
        #define FRONTEND_EXPORT
    #endif
#else
    #define FRONTEND_EXPORT
#endif