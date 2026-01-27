#include "fenn/common/uio.h"

// Standard C++ includes
#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>

// Standard C includes
#include <cassert>
#include <cstring>

// POSIX includes
#ifdef __linux__ 
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <sys/mman.h>
#endif

// PLOG includes
#include <plog/Log.h>

// Anonymous namespace
namespace
{
std::optional<int> findUIO(const std::string &targetName)
{
    for(int i = 0;;i++) {
        // Open name file
        std::ifstream nameStream("/sys/class/uio/uio" + std::to_string(i) + "/name");
        if(!nameStream.good()) {
            return std::nullopt;
        }
        
        // Read name
        std::string name;
        nameStream >> name;
        
        if(name == targetName) {
            return i;
        }
    }
    
    return std::nullopt;
}
//----------------------------------------------------------------------------
std::optional<size_t> getUIOSize(int uioIndex)
{
    // Open size file
    std::ifstream sizeStream("/sys/class/uio/uio" + std::to_string(uioIndex) + "/maps/map0/size");
    if(sizeStream.good()) {
        size_t size;
        sizeStream >> std::hex >> size;
        return size;
    }
    else {
        return std::nullopt;
    }    
}
}

//----------------------------------------------------------------------------
// FeNN::Common::UIO
//----------------------------------------------------------------------------
namespace FeNN::Common
{
UIO::UIO(const std::string &uioName)
:   m_Data(nullptr), m_Size(0)
{
#ifdef __linux__ 
    // Find UIO by name
    const auto uio = findUIO(uioName);
    if(!uio) {
        throw std::runtime_error("UIO '" + uioName + "' not found");
    }

    LOGI << "Opening UIO " << uio.value() << "('" << uioName << "')";

    const auto size = getUIOSize(uio.value());
    if(!size) {
        throw std::runtime_error("Unable to read size of UIO " + std::to_string(*uio) + "'" + uioName + "' not found");
    }
    m_Size = size.value();
    LOGD << "Size " << m_Size << " bytes";

    const std::string uioFilename = "/dev/uio" + std::to_string(uio.value());
    int memory = open(uioFilename.c_str(), O_RDWR | O_SYNC);
    if(memory == -1) {
        throw std::runtime_error(uioFilename + " open failure (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }
    
    // Memory map instruction memory
    m_Data = mmap(nullptr, m_Size, PROT_WRITE, MAP_SHARED, 
                  memory, 0);
    if(m_Data == MAP_FAILED) {
        throw std::runtime_error("UIO map failed (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Close memory device
    close(memory);
#else
    throw std::runtime_error("Device interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
UIO::~UIO()
{
#ifdef __linux__ 
    // Unmap memory
    munmap(m_Data, m_Size);
#endif
}
}   // namespace FeNN::Common
