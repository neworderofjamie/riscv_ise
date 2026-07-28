#pragma once

// Standard C++ includes
#include <functional>
#include <memory>
#include <vector>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/events.h"
#include "frontend/runtime.h"

// FeNN assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/register_allocator.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"
#include "fenn/backend/fields.h"

// Forward declarations
namespace FeNN
{
namespace Assembler
{
class CodeGenerator;
}
namespace Backend
{
class KernelImplementation;
class Model;
class NeuronUpdateProcess;
class Runtime;
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
    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    //! Generate code to implement event loop
    virtual void generateEventLoop(const Frontend::Merged<Frontend::EventSource> &mergedEventSource, const Runtime &runtime, 
                                   const KernelImplementation &kernel, MergedFields &mergedFields, 
                                   Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg, 
                                   Assembler::ScalarRegisterPtr spikeReturnReg, Assembler::Label jumpTable, 
                                   const std::unordered_map<std::shared_ptr<const Frontend::EventSource>, Assembler::Label> &eventSourceLabels,
                                   uint32_t &fieldBase, Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator) const = 0;
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceImplementation
//----------------------------------------------------------------------------
class EventSinkImplementation
{
protected:
    using AddScalarConstantFn = std::function<Assembler::ScalarRegisterPtr(Assembler::CodeGenerator&, MergedFields::GetFieldConstantFunc<NeuronUpdateProcess>)>;
    using AddFieldFn = std::function<Assembler::ScalarRegisterPtr(Assembler::CodeGenerator&)>;

public:
    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    virtual std::vector<Assembler::ScalarRegisterPtr> genPreamble(
        const Model &model, const KernelImplementation &kernel,
        Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
        std::optional<uint32_t> numTimesteps, bool hasTime, size_t numDevices,
        Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes,
        AddScalarConstantFn addScalarConstant, AddFieldFn addField) const = 0;
    
    virtual void genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                         Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t r, 
                         const std::vector<Assembler::ScalarRegisterPtr> &state) const = 0;

    //! Generate code to advance pointer after numUnrolls unrolled 
    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls, 
                              const std::vector<Assembler::ScalarRegisterPtr> &state) const = 0;

protected:
    //----------------------------------------------------------------------------
    // Protected API
    //----------------------------------------------------------------------------
    std::unique_ptr<Frontend::ArrayBase> createBitArray(const Frontend::Shape &deviceShape, Frontend::DeviceBase &device) const;

    Assembler::ScalarRegisterPtr genBitArrayPreamble(
        Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
        std::optional<uint32_t> numTimesteps, bool hasTime, const Frontend::Shape &shape,
        Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes, 
        AddFieldFn addField) const;
    
    void genBitArrayEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterPtr spikeMaskReg, 
                         uint32_t r, Assembler::ScalarRegisterPtr stateReg) const;

    void genBitArrayIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                              Assembler::ScalarRegisterPtr stateReg) const;
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventSourceBuffer
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT EventSourceBuffer : public Frontend::EventSourceBuffer, public EventSourceImplementation
{
public:
    EventSourceBuffer(Private, const Frontend::Shape &shape, size_t maxEvents, const std::string &name)
    :   State(name), Frontend::EventSourceBuffer(Private(), shape, maxEvents, name)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //----------------------------------------------------------------------------
    // EventSourceImplementation virtuals
    //----------------------------------------------------------------------------
    //! Generate code to implement event loop
    virtual void generateEventLoop(const Frontend::Merged<Frontend::EventSource> &mergedEventSource, const Runtime &runtime, 
                                   const KernelImplementation &kernel, MergedFields &mergedFields, 
                                   Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg, 
                                   Assembler::ScalarRegisterPtr spikeReturnReg, Assembler::Label jumpTable, 
                                   const std::unordered_map<std::shared_ptr<const Frontend::EventSource>, Assembler::Label> &eventSourceLabels,
                                   uint32_t &fieldBase, Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventSourceBuffer> create(const Frontend::Shape &shape, size_t maxEvents, const std::string &name = "")
    {
        return std::make_shared<EventSourceBuffer>(Private(), shape, maxEvents, name);
    }

private:
    void generateArchetypeEventLoop(MergedFields &mergedFields, 
                                    Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
                                    Assembler::ScalarRegisterPtr preIndReg, Assembler::ScalarRegisterPtr spikeReturnReg, 
                                    const std::vector<uint32_t> &mergedLabelAddresses, Assembler::CodeGenerator &c, 
                                    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator) const;
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
        const Model &model, const KernelImplementation &kernel,
        Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
        std::optional<uint32_t> numTimesteps, bool hasTime, size_t numDevices,
        Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes, 
        AddScalarConstantFn addScalarConstant, AddFieldFn addField) const override final;

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
    EventChannel(Private, const Frontend::Shape &shape, bool record, const std::string &name)
    :   State(name), Frontend::EventChannel(Private(), shape, record, name)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const Frontend::Shape &deviceShape, const Frontend::Model &model,
                                                             Frontend::DeviceBase &device) const override final;

    //----------------------------------------------------------------------------
    // EventSourceImplementation virtuals
    //----------------------------------------------------------------------------
    //! Generate code to implement event loop
    virtual void generateEventLoop(const Frontend::Merged<Frontend::EventSource> &mergedEventSource, const Runtime &runtime, 
                                   const KernelImplementation &kernel, MergedFields &mergedFields, 
                                   Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg, 
                                   Assembler::ScalarRegisterPtr spikeReturnReg, Assembler::Label jumpTable, 
                                   const std::unordered_map<std::shared_ptr<const Frontend::EventSource>, Assembler::Label> &eventSourceLabels,
                                   uint32_t &fieldBase, Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // EventSinkImplementation virtuals
    //------------------------------------------------------------------------
    virtual std::vector<Assembler::ScalarRegisterPtr> genPreamble(
        const Model &model, const KernelImplementation &kernel,
        Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
        std::optional<uint32_t> numTimesteps, bool hasTime, size_t numDevices,
        Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr numEventBytes, 
        AddScalarConstantFn addScalarConstant, AddFieldFn addField) const override final;

    virtual void genEmit(Compiler::EnvironmentBase &env, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                         Assembler::ScalarRegisterPtr spikeMaskReg, uint32_t r, 
                         const std::vector<Assembler::ScalarRegisterPtr> &state) const override final;

    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls, 
                              const std::vector<Assembler::ScalarRegisterPtr> &state) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventChannel> create(const Frontend::Shape &shape, bool record = false, 
                                                const std::string &name = "")
    {
        return std::make_shared<EventChannel>(Private(), shape, record, name);
    }
};
}