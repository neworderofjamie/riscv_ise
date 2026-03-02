#pragma once

// Standard C++ includes
#include <functional>

// Frontend includes
#include "frontend/kernel.h"

// Assembler includes
#include "fenn/assembler/register_allocator.h"

// Forward declarations
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
                                                      Assembler::CodeGenerator&,
                                                      Assembler::ScalarRegisterAllocator&,
                                                      Assembler::VectorRegisterAllocator&)>;
    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                              GenerateProcessGroupFn generateProcessGroup) const = 0;
};

//----------------------------------------------------------------------------
// FeNN::Backend::SimpleKernel
//----------------------------------------------------------------------------
class SimpleKernel : public KernelImplementation, public Frontend::SimpleKernel
{
public:
    using Frontend::SimpleKernel::SimpleKernel;

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
    using Frontend::SimulationLoopKernel::SimulationLoopKernel;


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
    static std::shared_ptr<SimulationLoopKernel> create(uint32_t numTimesteps, Frontend::ProcessGroupVector &timestepProcessGroups, 
                                                        const Frontend::ProcessGroupVector &beginProcessGroups = {},
                                                        const Frontend::ProcessGroupVector &endProcessGroups = {},
                                                        const std::string &name = "")
    {
        return std::make_shared<SimulationLoopKernel>(Private(), numTimesteps, timestepProcessGroups,
                                                      beginProcessGroups, endProcessGroups, name);
    }
};
}
