#include "common/utils.h"

// Standard C++ includes
#include <stdexcept>

// Platform includes
#ifdef _WIN32
#include <windows.h>
#else
 #include <pthread.h>
#endif

void setThreadName(std::thread &thread, const std::string &name)
{
#ifdef _WIN32
    const std::wstring wstr(name.cbegin(), name.cend());
    if (FAILED(SetThreadDescription(thread.native_handle(), wstr.c_str())))
#else
    if(pthread_setname_np(thread.native_handle(), name.c_str()) != 0)
#endif
    {
        throw std::runtime_error("Unable to set thread name");
    }
}