#pragma once

// If we're building on Windows
#if defined(_WIN32)
    #ifdef BUILDING_FENN_DISASSEMBLER_DLL
        #define FENN_DISASSEMBLER_EXPORT __declspec(dllexport)
    #elif defined(LINKING_FENN_DISASSEMBLER_DLL)
        #define FENN_DISASSEMBLER_EXPORT __declspec(dllimport)
    #else
        #define FENN_DISASSEMBLER_EXPORT
    #endif
#else
    #define FENN_DISASSEMBLER_EXPORT
#endif