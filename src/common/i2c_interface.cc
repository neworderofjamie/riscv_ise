#include "common/i2c_interface.h"

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

// PLOG includes
#include <plog/Log.h>

//----------------------------------------------------------------------------
// I2CInterface
//----------------------------------------------------------------------------
I2CInterface::I2CInterface(const std::string &path, int slaveAddress)
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
        LOGI << "I2C successfully initialized";
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

    LOGD << "I2C closed";
#else
    throw std::runtime_error("I2C interface only supports Linux");
#endif  // __linux__
}
//---------------------------------------------------------------------
uint8_t I2CInterface::readByteCommand(uint8_t address)
{
#ifdef __linux__ 
    const auto data = i2c_smbus_read_byte_data(m_I2C, address);
    if (data < 0) {
        throw std::runtime_error("Failed to read byte from i2c bus");
    } else {
        return static_cast<uint8_t>(data);
    }
#else
    throw std::runtime_error("I2C interface only supports Linux");
#endif  // __linux__
}
//---------------------------------------------------------------------
uint8_t I2CInterface::readByte()
{
#ifdef __linux__ 
    const auto data = i2c_smbus_read_byte(m_I2C);
    if (data < 0) {
        throw std::runtime_error("Failed to read byte from i2c bus");
    } else {
        return static_cast<uint8_t>(data);
    }
#else
    throw std::runtime_error("I2C interface only supports Linux");
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
void I2CInterface::writeByteCommand(uint8_t address, uint8_t byte)
{
#ifdef __linux__ 
    if (i2c_smbus_write_byte_data(m_I2C, address, byte) < 0) {
        throw std::runtime_error("Failed to write byte to i2c bus");
    }
#else
    throw std::runtime_error("I2C interface only supports Linux");
#endif  // __linux__
}
//---------------------------------------------------------------------
void I2CInterface::writeByte(uint8_t byte)
{
#ifdef __linux__ 
    if (i2c_smbus_write_byte(m_I2C, byte) < 0) {
        throw std::runtime_error("Failed to write byte to i2c bus");
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
