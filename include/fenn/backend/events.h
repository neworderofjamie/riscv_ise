#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/events.h"

// FeNN assembler includes
#include "fenn/assembler/register_allocator.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"

// Forward declarations
namespace FeNN
{
namespace Assembler
{
class CodeGenerator;
}
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class EventSourceImplementation
{
public:
    //! Generate code to implement process
    virtual void generateCode() const{}
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceImplementation
//----------------------------------------------------------------------------
class EventSinkImplementation
{
public:
    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    virtual std::vector<Assembler::ScalarRegisterPtr> genPreamble(
        Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
        uint32_t eventFieldOffset, std::optional<uint32_t> numTimesteps, 
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        Assembler::ScalarRegisterPtr numEventBytes, bool hasTime) const = 0;
    
     virtual void genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                          Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t r, 
                          const std::vector<Assembler::ScalarRegisterPtr> &state) const = 0;

    //! Generate code to advance pointer after numUnrolls unrolled 
    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls, 
                              const std::vector<Assembler::ScalarRegisterPtr> &state) const = 0;
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceBuffer
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT EventSourceBuffer : public Frontend::EventSourceBuffer, public EventSourceImplementation
{
public:
    EventSourceBuffer(Private, const Frontend::Shape &shape, const std::string &name)
    :   State(name), Frontend::EventSourceBuffer(Private(), shape, name)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventSourceBuffer> create(const Frontend::Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventSourceBuffer>(Private(), shape, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventSinkBuffer
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT EventSinkBuffer : public Frontend::EventSinkBuffer, public EventSinkImplementation
{
public:
    EventSinkBuffer(Private, const Frontend::Shape &shape, const std::string &name)
    :   State(name), Frontend::EventSinkBuffer(Private(), shape, name)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //------------------------------------------------------------------------
    // EventSinkImplementation virtuals
    //------------------------------------------------------------------------
    virtual std::vector<Assembler::ScalarRegisterPtr> genPreamble(
        Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
        uint32_t eventFieldOffset, std::optional<uint32_t> numTimesteps, 
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        Assembler::ScalarRegisterPtr numEventBytes, bool hasTime) const override final;

    virtual void genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                         Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t r, 
                         const std::vector<Assembler::ScalarRegisterPtr> &state) const override final;

    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls, 
                              const std::vector<Assembler::ScalarRegisterPtr> &state) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventSinkBuffer> create(const Frontend::Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventSinkBuffer>(Private(), shape, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventChannel
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT EventChannel : public Frontend::EventChannel, public EventSourceImplementation, public EventSinkImplementation
{
public:
    EventChannel(Private, const Frontend::Shape &shape, const std::string &name)
    :   State(name), Frontend::EventChannel(Private(), shape, name)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //------------------------------------------------------------------------
    // EventSinkImplementation virtuals
    //------------------------------------------------------------------------
    virtual std::vector<Assembler::ScalarRegisterPtr> genPreamble(
        Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
        uint32_t eventFieldOffset, std::optional<uint32_t> numTimesteps, 
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        Assembler::ScalarRegisterPtr numEventBytes, bool hasTime) const override final;

    virtual void genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                         Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t r, 
                         const std::vector<Assembler::ScalarRegisterPtr> &state) const override final;

    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls, 
                              const std::vector<Assembler::ScalarRegisterPtr> &state) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Frontend::EventChannel> create(const Frontend::Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventChannel>(Private(), shape, name);
    }
};
}