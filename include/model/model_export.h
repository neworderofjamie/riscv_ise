#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_MODEL_DLL
        #define MODEL_EXPORT __declspec(dllexport)
    #elif defined(LINKING_MODEL_DLL)
        #define MODEL_EXPORT __declspec(dllimport)
    #else
        #define MODEL_EXPORT
    #endif
#else
    #define MODEL_EXPORT
#endif