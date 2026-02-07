#pragma once

// Standard C++ includes
#include <memory>
#include <stdexcept>

// Standard C includes
#include <cstdint>

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// FeNN common include
#include "fenn/common/isa.h"

#define ALLOCATE_SCALAR(NAME) const auto NAME = scalarRegisterAllocator.getRegister(#NAME" = X");
#define ALLOCATE_VECTOR(NAME) const auto NAME = vectorRegisterAllocator.getRegister(#NAME" = V");

//----------------------------------------------------------------------------
// FeNN::Assembler::RegisterAllocator
//----------------------------------------------------------------------------
namespace FeNN::Assembler
{
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
        Handle(T reg, RegisterAllocator<T>& parent, const char* context = nullptr)
            : m_Reg(reg), m_Parent(parent)
        {
            if (context) {
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
        RegisterAllocator<T>& m_Parent;
    };

    using RegisterPtr = std::shared_ptr<Handle>;

    RegisterAllocator(uint32_t initialFreeRegisters = 0xFFFFFFFFu)
        : m_FreeRegisters(initialFreeRegisters), m_MaxUsedRegisters(0)
    {
    }

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    RegisterPtr getRegister(const char* context = nullptr)
    {
        using namespace ::Common::Utils;

        if (m_FreeRegisters == 0) {
            throw std::runtime_error("Out of registers");
        }
        else {
            const int n = clz(m_FreeRegisters);
            m_FreeRegisters &= ~(0x80000000 >> n);
            m_MaxUsedRegisters = std::max(m_MaxUsedRegisters, 
                                          32 - popCount(m_FreeRegisters));
            return std::make_shared<Handle>(static_cast<T>(n), *this, context);
        }
    }

    void releaseRegister(T reg)
    {
        const uint32_t regNum = static_cast<uint32_t>(reg);

        if ((m_FreeRegisters & (0x80000000 >> regNum)) != 0) {
            throw std::runtime_error("Releasing unused register");
        }
        else {
            m_FreeRegisters |= (0x80000000 >> regNum);
        }
    }

    int getMaxUsedRegisters() const { return m_MaxUsedRegisters; }

private:
    //------------------------------------------------------------------------
    // Private members
    //------------------------------------------------------------------------
    uint32_t m_FreeRegisters;
    int m_MaxUsedRegisters;
};

class VectorRegisterAllocator : public RegisterAllocator<Common::VReg>
{
public:
    using RegisterAllocator<Common::VReg>::RegisterAllocator;
};

class ScalarRegisterAllocator : public RegisterAllocator<Common::Reg>
{
public:
    ScalarRegisterAllocator() : RegisterAllocator<Common::Reg>(0x7FFFFFFFu)
    {
    }
};
}   // namespace FeNN::Assembler