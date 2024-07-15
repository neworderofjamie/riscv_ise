#pragma once

// Standard C++ includes
#include <memory>
#include <stdexcept>

// Standard C includes
#include <cstdint>

// PLOG includes
#include <plog/Log.h>

// RISC-V utils include
#include "common/isa.h"
#include "common/utils.h"

#define ALLOCATE_SCALAR(NAME) const auto NAME = scalarRegisterAllocator.getRegister(#NAME" = X");
#define ALLOCATE_VECTOR(NAME) const auto NAME = vectorRegisterAllocator.getRegister(#NAME" = V");

//----------------------------------------------------------------------------
// RegisterAllocator
//----------------------------------------------------------------------------
//! Automatic register allocator which uses std::shared_ptr to track register lifetime
template<typename T>
class RegisterAllocator
{
public:
    //------------------------------------------------------------------------
    // RegisterAllocator::Handle
    //------------------------------------------------------------------------
    class Handle
    {
    public:
        Handle(T reg, RegisterAllocator<T> &parent, const char *context = nullptr)
        :   m_Reg(reg), m_Parent(parent)
        {
            if(context) {
                PLOGD << "Allocating " << context << static_cast<uint32_t>(reg);
            }
        }

        ~Handle()
        {
            m_Parent.releaseRegister(m_Reg);
        }
    
        operator T() const
        {
            return m_Reg;
        }

    private:
        T m_Reg;
        RegisterAllocator<T> &m_Parent;
    };

    RegisterAllocator(uint32_t initialFreeRegisters = 0xFFFFFFFFu)
    :   m_FreeRegisters(initialFreeRegisters)
    {
    }

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    std::shared_ptr<Handle> getRegister(const char *context = nullptr)
    {
        if(m_FreeRegisters == 0) {
            throw std::runtime_error("Out of registers");
        }
        else {
            const int n = clz(m_FreeRegisters);
            m_FreeRegisters &= ~(0x80000000 >> n);
            return std::make_shared<Handle>(static_cast<T>(n), *this, context);
        }
    }
    
    void releaseRegister(T reg)
    {
        const uint32_t regNum = static_cast<uint32_t>(reg);

        if((m_FreeRegisters & (0x80000000 >> regNum)) != 0) {
            throw std::runtime_error("Releasing unused register");
        }
        else {
            m_FreeRegisters |= (0x80000000 >> regNum); 
        }
    }

private:
    //------------------------------------------------------------------------
    // Private members
    //------------------------------------------------------------------------
    uint32_t m_FreeRegisters;
};

using VectorRegisterAllocator = RegisterAllocator<VReg>;

class ScalarRegisterAllocator : public RegisterAllocator<Reg>
{
public:
    ScalarRegisterAllocator() : RegisterAllocator<Reg>(0x7FFFFFFFu)
    {}
};