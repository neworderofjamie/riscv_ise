#include "common/utils.h"

// Platform includes
#ifdef _WIN32
#include <processthreadsapi.h>
#else
 #include <pthread.h>
#endif

void setThreadName(std::thread &thread, const std::string &name)
{
    if (
#ifdef _WIN32
    FAILED(SetThreadDescription(thread.native_handle(), name.c_str()))
#else
    pthread_setname_np(thread.native_handle(), name.c_str()) != 0
#endif
    ) {
        throw std::runtime_error("Unable to set thread name");
    }
}