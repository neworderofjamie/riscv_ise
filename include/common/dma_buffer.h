#pragma once

// C standard includes
#include <cstdint>

//----------------------------------------------------------------------------
// DMABuffer
//----------------------------------------------------------------------------
//! Thin wrapper around
class DMABuffer
{
public:
    DMABuffer(int index = 0);
    ~DMABuffer();

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    uint64_t getPhysicalAddress() const{ return m_PhysicalAddress; }
    uint64_t getSize() const{ return m_Size; }
    uint8_t *getData(){ return m_Data; }
    const uint8_t *getData() const{ return m_Data; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    int m_Memory;
    uint8_t *m_Data;
    uint64_t m_PhysicalAddress;
    uint64_t m_Size;
};