#pragma once

// Standard C++ includes
#include <memory>
#include <stdexcept>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/utils.h"

// FeNN common include
#include "fenn/common/isa.h"
#include "fenn/common/logging.h"

#define ALLOCATE_SCALAR(NAME) const auto NAME = scalarRegisterAllocator.getRegister(#NAME" = X")
#define ALLOCATE_VECTOR(NAME) const auto NAME = vectorRegisterAllocator.getRegister(#NAME" = V")

#define ALLOCATE_SCALAR_AND_MASK(NAME)                                              \
    const auto NAME = scalarRegisterAllocator.getRegister(#NAME" = X");             \
    FeNN::Assembler::ScalarRegisterAllocator::updateMask(*NAME, scalarRegisterMask)
#define ALLOCATE_VECTOR_AND_MASK(NAME)                                              \
    const auto NAME = vectorRegisterAllocator.getRegister(#NAME" = V")              \
    FeNN::Assembler::VectorRegisterAllocator::updateMask(*NAME, scalarRegisterMask)


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
                LOGD_FENN_ASSEMBLER << "Allocating " << context << static_cast<uint32_t>(reg);
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
    RegisterAllocator(const RegisterAllocator<T> &) = delete;
    RegisterAllocator(RegisterAllocator<T> &&) = delete;

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
    
    void maskRegisters(uint32_t mask) 
    {
        // Give error if all masked registers aren't already free
        if ((m_FreeRegisters & mask) != mask) {
            throw std::runtime_error("Masking register which is already in use");
        }

        // Mark all masked registers as not free
        m_FreeRegisters &= ~mask; 
    }

    void unmaskRegisters(uint32_t mask)
    {
        // Mark all masked registers as free
        m_FreeRegisters |= mask;
    }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static void updateMask(T r, uint32_t &mask) {
        mask |= (0x80000000 >> static_cast<uint32_t>(r));
    }

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

using VectorRegisterPtr = VectorRegisterAllocator::RegisterPtr;
using ScalarRegisterPtr = ScalarRegisterAllocator::RegisterPtr;
}   // namespace FeNN::Assembler 