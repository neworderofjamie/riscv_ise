#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_DISASSEMBLER_DLL
        #define DISASSEMBLER_EXPORT __declspec(dllexport)
    #elif defined(LINKING_DISASSEMBLER_DLL)
        #define DISASSEMBLER_EXPORT __declspec(dllimport)
    #else
        #define DISASSEMBLER_EXPORT
    #endif
#else
    #define DISASSEMBLER_EXPORT
#endif