#include "helpers.h"

// C++ standard includes
#include <string>

// C standard includes
#include <cstdlib>

namespace Helpers
{
int getCore()
{
    const char *fennCore = std::getenv("FENN_CORE");
    return fennCore ? std::stoi(fennCore) : 0;
}
int getNumCores()
{
    const char *fennNumCores = std::getenv("FENN_NUM_CORES");
    return fennNumCores ? std::stoi(fennNumCores) : 1;
}
}
