#pragma once

// Standard C++ includes
#include <functional>
#include <optional>

// Frontend includes
#include "frontend/events.h"
#include "frontend/kernel.h"
#include "frontend/merged_model.h"

// Assembler includes
#include "fenn/assembler/register_allocator.h"

// Forward declarations
namespace Frontend
{
class EventSink;
}
namespace FeNN::Assembler
{
class CodeGenerator;
}

//----------------------------------------------------------------------------
// FeNN::Backend::KernelImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class KernelImplementation
{
public:
    using GenerateProcessGroupFn = std::function<void(std::shared_ptr<const Frontend::ProcessGroup>,
                                                      Assembler::ScalarRegisterPtr,
                                                      std::optional<uint32_t>, Assembler::CodeGenerator&,
                                                      Assembler::ScalarRegisterAllocator&,
                                                      Assembler::VectorRegisterAllocator&)>;
    
    KernelImplementation(const Frontend::ProcessGroupVector &processGroups);

    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                              GenerateProcessGroupFn generateProcessGroup) const = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Get map of event sinks to their IDs
    const auto &getEventSinkIDs() const{ return m_EventSinkIDs; }

    //! Get the base ID of this event sink
    uint32_t getEventSinkIDBase(std::shared_ptr<const Frontend::EventSink> eventSink) const;
    
    //! Get the process group in this kernel 
    auto getEventSourceProcessGroup() const{ return m_EventSourceProcessGroup; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const Frontend::ProcessGroup> m_EventSourceProcessGroup;

    std::unordered_map<std::shared_ptr<const Frontend::EventSink>, uint32_t> m_EventSinkIDs;

    std::vector<Frontend::Merged<Frontend::EventSource>> m_MergedEventSources;

    uint32_t m_NumNeuronIDBits;
    uint32_t m_NumPopulationIDBits;
};

//----------------------------------------------------------------------------
// FeNN::Backend::SimpleKernel
//----------------------------------------------------------------------------
class SimpleKernel : public KernelImplementation, public Frontend::SimpleKernel
{
public:
    SimpleKernel(Private, const Frontend::ProcessGroupVector &processGroups, const std::string &name)
    :   KernelImplementation(processGroups), Frontend::SimpleKernel(Private(), processGroups, name)
    {}

    //------------------------------------------------------------------------
    // GraphImplementation virtuals
    //------------------------------------------------------------------------
    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                              GenerateProcessGroupFn generateProcessGroup) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<SimpleKernel> create(const Frontend::ProcessGroupVector &processGroups,
                                                const std::string &name = "")
    {
        return std::make_shared<SimpleKernel>(Private(), processGroups, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::SimulationLoopKernel
//----------------------------------------------------------------------------
class SimulationLoopKernel : public KernelImplementation, public Frontend::SimulationLoopKernel
{
public:
    SimulationLoopKernel(Private, unsigned int numTimesteps, 
                         const Frontend::ProcessGroupVector &timestepProcessGroups, 
                         const Frontend::ProcessGroupVector &beginProcessGroups,
                         const Frontend::ProcessGroupVector &endProcessGroups, const std::string &name)
    :   KernelImplementation(timestepProcessGroups), 
        Frontend::SimulationLoopKernel(Private(), numTimesteps, timestepProcessGroups, 
                                       beginProcessGroups, endProcessGroups, name)
    {}

    //------------------------------------------------------------------------
    // GraphImplementation virtuals
    //------------------------------------------------------------------------
    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                              GenerateProcessGroupFn generateProcessGroup) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<SimulationLoopKernel> create(uint32_t numTimesteps, const Frontend::ProcessGroupVector &timestepProcessGroups, 
                                                        const Frontend::ProcessGroupVector &beginProcessGroups = {},
                                                        const Frontend::ProcessGroupVector &endProcessGroups = {},
                                                        const std::string &name = "")
    {
        return std::make_shared<SimulationLoopKernel>(Private(), numTimesteps, timestepProcessGroups,
                                                      beginProcessGroups, endProcessGroups, name);
    }
};
}
