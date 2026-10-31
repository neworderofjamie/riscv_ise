#pragma once

// Standard C++ includes
#include <iostream>

// GeNN CUDA backend includes
#include "logging.h"

#define CHECK_CU_ERRORS(call)                                                                               \
    do {                                                                                                    \
        CUresult result = call;                                                                             \
        if (result != CUDA_SUCCESS) {                                                                       \
            const char *errStr;                                                                             \
            cuGetErrorName(result, &errStr);                                                                \
            LOGE_BACKEND << __FILE__ << ": " <<  __LINE__ << ": CUDA error " << result << ": " << errStr;   \
            exit(EXIT_FAILURE);                                                                             \
        }                                                                                                   \
    } while(false)


#define CHECK_NVRTC_ERRORS(call)                                                                                                \
    do {                                                                                                                        \
        nvrtcResult  result = call;                                                                                             \
        if (result != NVRTC_SUCCESS) {                                                                                          \
            LOGE_BACKEND << __FILE__ << ": " <<  __LINE__ << ": NVRTC error " << error << ": " << nvrtcGetErrorString(result);  \
            exit(EXIT_FAILURE);                                                                                                 \
        }                                                                                                                       \
    } while(false)

#define CHECK_NVJITLINK_ERRORS(handle, call)                                                \
    do {                                                                                    \
        nvJitLinkResult  result = call;                                                     \
        if (result != NVJITLINK_SUCCESS) {                                                  \
            LOGE_BACKEND << __FILE__ << ": " <<  __LINE__ << ": NVJITLINK error " << error; \
            char *log = new char[lsize];                                                    \
            if (nvJitLinkGetErrorLog(handle, log) == NVJITLINK_SUCCESS) {                   \
              LOGE_BACKEND << "error: " << log << '\n';                                     \
            }                                                                               \
            delete [] log;                                                                  \
            exit(EXIT_FAILURE);                                                             \
        }                                                                                   \
    } while(false)