#include "fenn/common/i2c_interface.h"

// Standard C++ includes
#include <string>
#include <vector>

// Standard C includes
#include <cstring>

// POSIX includes
#ifdef __linux__ 
#include <cstdlib>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

extern "C"
{
// I2C includes
#include <linux/i2c-dev.h>

// This extra header is needed after Ubuntu 16.04 (newer kernel?)
#include <i2c/smbus.h>
}
#endif  // __linux__

// FeNN common includes
#include "fenn/common/logging.h"

//----------------------------------------------------------------------------
// I2CInterface
//----------------------------------------------------------------------------
namespace FeNN::Common
{
I2CInterface::I2CInterface(const std::string &path, int slaveAddress)
:   m_SlaveAddress(slaveAddress)
{
#ifdef __linux__ 
    m_I2C = open(path.c_str(), O_RDWR);
    if (m_I2C < 0) {
        throw std::runtime_error("Error in setup: " + std::string(strerror(errno)) + "\n" +
                                 "The error is usually permission error which, on Ubuntu, can be fixed by" +
                                 "creating a file /etc/udev/rules.d/90-i2c.rules and adding the following line:\n" +
                                 "   KERNEL==\"i2c-[0-7]\",MODE=\"0666\"");
    }

    if (ioctl(m_I2C, I2C_SLAVE, slaveAddress) < 0) {
        throw std::runtime_error("Cannot connect to I2C slave");
    } else {
        LOGI_FENN_COMMON << "I2C successfully initialized";
    }
#else
    throw std::runtime_error("I2C interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
I2CInterface::~I2CInterface()
{
#ifdef __linux__ 
    // Close I2C device
    if (m_I2C >= 0) {
        close(m_I2C);
    }

    LOGD_FENN_COMMON << "I2C closed";
#endif  // __linux__
}
//---------------------------------------------------------------------
void I2CInterface::read(void *data, size_t size)
{
#ifdef __linux__ 
    if (::read(m_I2C, data, size) != (int) size) {
        throw std::runtime_error("Failed to read from i2c bus: " +
                                 std::string(strerror(errno)) +
                                 " (" + std::to_string(errno) + ")");
    }
#else
    throw std::runtime_error("I2C interface only supports Linux");
#endif  // __linux__
}
//---------------------------------------------------------------------
void I2CInterface::write(const void *data, size_t size)
{
#ifdef __linux__ 
    if (::write(m_I2C, data, size) != (int) size) {
        throw std::runtime_error("Failed to write to i2c bus: " +
                                 std::string(strerror(errno)) +
                                 " (" + std::to_string(errno) + ")");
    }
#else
    throw std::runtime_error("I2C interface only supports Linux");
#endif  // __linux__
}
//---------------------------------------------------------------------
void I2CInterface::writeRead(const uint8_t *writeData, size_t writeSize,
                             uint8_t *readData, size_t readSize)
{
#ifdef __linux__ 
    i2c_msg messages[2];
    messages[0].addr = m_SlaveAddress;
    messages[0].buf = const_cast<uint8_t*>(writeData);
    messages[0].len = writeSize;
    messages[0].flags = 0;
    
    messages[1].addr = m_SlaveAddress;
    messages[1].buf = readData;
    messages[1].len = readSize;
    messages[1].flags = I2C_M_RD;

    i2c_rdwr_ioctl_data data;
    data.msgs = &messages[0];
    data.nmsgs = 2;
    
    if (ioctl(m_I2C, I2C_RDWR, &data) < 0) {
        throw std::runtime_error("Cannot send combined R/W transfer"
                                 + std::string(strerror(errno)) +
                                 " (" + std::to_string(errno) + ")");
    } 
#else
    throw std::runtime_error("I2C interface only supports Linux");
#endif  // __linux__
}
}