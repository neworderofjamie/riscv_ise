#pragma once

// C standard includes
#include <cstdint>

// FeNN common includes
#include "fenn/common/fenn_common_export.h"

//----------------------------------------------------------------------------
// FeNN::Common::DMABuffer
//----------------------------------------------------------------------------
//! Thin wrapper around
namespace FeNN::Common
{
class FENN_COMMON_EXPORT DMABuffer
{
public:
    // 
    enum class AccessMode
    {
        CPU,
        FENN,
    };

    DMABuffer(int index = 0);
    DMABuffer(DMABuffer &parent, uint64_t physicalStartAddress, uint64_t physicalEndAddress);
    ~DMABuffer();

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    uint64_t getPhysicalAddress() const{ return m_PhysicalAddress; }
    uint64_t getSize() const{ return m_Size; }
    uint8_t *getData(){ return m_Data; }
    const uint8_t *getData() const{ return m_Data; }

    void setAccessMode(AccessMode mode);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    int m_Memory;
    uint8_t *m_Data;
    uint64_t m_PhysicalAddress;
    uint64_t m_Size;
    DMABuffer *m_Parent;
};
}   // namespace FeNN::Common