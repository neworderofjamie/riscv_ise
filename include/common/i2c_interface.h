#pragma once

// Standard C includes
#include <cstdint>

// Standard C++ includes
#include <stdexcept>

// Common includes
#include "common/common_export.h"

//----------------------------------------------------------------------------
// I2CInterface
//----------------------------------------------------------------------------
//! Class for communicating over I2C
class COMMON_EXPORT I2CInterface
{
public:
    I2CInterface(const std::string &path, int slaveAddress);
    ~I2CInterface();

    // Make non-moveable and non-copyable
    I2CInterface(I2CInterface &&) = delete;

    //---------------------------------------------------------------------
    // Public API
    //---------------------------------------------------------------------
    uint8_t readByteCommand(uint8_t address);
    uint8_t readByte();

    template<typename T, size_t N>
    void read(T (&data)[N])
    {
        read(&data[0], N * sizeof(T));
    }

    template<typename T,
             typename std::enable_if<std::is_pod<T>::value, T>::type* = nullptr>
    void read(T &data)
    {
        read(&data, sizeof(T));
    }

    void writeByteCommand(uint8_t address, uint8_t byte);
    void writeByte(uint8_t byte);

    // writes data
    template<typename T, size_t N>
    void write(const T (&data)[N])
    {
        write(&data[0], N * sizeof(T));
    }

    template<typename T,
             typename std::enable_if<std::is_pod<T>::value, T>::type* = nullptr>
    void write(const T &data)
    {
        write(&data, sizeof(T));
    }

private:
    //---------------------------------------------------------------------
    // Members
    //---------------------------------------------------------------------
    int m_I2C; // i2c file

    void read(void *data, size_t size);
    void write(const void *data, size_t size);
};
