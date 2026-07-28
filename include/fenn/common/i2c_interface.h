#pragma once

// Standard C includes
#include <cstdint>

// Standard C++ includes
#include <stdexcept>

// Common includes
#include "fenn/common/fenn_common_export.h"

//----------------------------------------------------------------------------
// FeNN::Common::I2CInterface
//----------------------------------------------------------------------------
//! Class for communicating over I2C
namespace FeNN::Common
{
class FENN_COMMON_EXPORT I2CInterface
{
public:
    I2CInterface(const std::string &path, int slaveAddress);
    ~I2CInterface();

    // Make non-moveable and non-copyable
    I2CInterface(I2CInterface &&) = delete;

    //---------------------------------------------------------------------
    // Public API
    //---------------------------------------------------------------------
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

    template<typename W, typename R>
    void writeRead(const W &writeData, R &readData)
    {
        writeRead(reinterpret_cast<const uint8_t*>(&writeData), sizeof(W),
                  reinterpret_cast<uint8_t*>(&readData), sizeof(R));
    }

    template<typename W, typename R, size_t N, size_t M>
    void writeRead(const W (&writeData)[N], R (&readData)[M])
    {
        writeRead(reinterpret_cast<const uint8_t*>(&writeData[0]), N * sizeof(W),
                  reinterpret_cast<uint8_t*>(&readData[0]), M * sizeof(R));
    }

private:
    void read(void *data, size_t size);
    void write(const void *data, size_t size);

    void writeRead(const uint8_t *writeData, size_t writeSize,
                   uint8_t *readData, size_t readSize);
    
    //---------------------------------------------------------------------
    // Members
    //---------------------------------------------------------------------
    int m_SlaveAddress;
    int m_I2C; // i2c file
};
}