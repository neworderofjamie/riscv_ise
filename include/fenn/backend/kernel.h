#pragma once

// Model includes
#include "model/kernel.h"

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
    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;
};

//----------------------------------------------------------------------------
// FeNN::Backend::SimpleKernel
//----------------------------------------------------------------------------
class SimpleKernel : public KernelImplementation, public Model::SimpleKernel
{
public:
    using Model::SimpleKernel::SimpleKernel;

    //------------------------------------------------------------------------
    // GraphImplementation virtuals
    //------------------------------------------------------------------------
    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<SimpleKernel> create(const Model::ProcessGroupVector &processGroups,
                                                const std::string &name = "")
    {
        return std::make_shared<SimpleKernel>(Private(), processGroups, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::SimulationLoopKernel
//----------------------------------------------------------------------------
class SimulationLoopKernel : public KernelImplementation, public Model::SimulationLoopKernel
{
public:
    using Model::SimulationLoopKernel::SimulationLoopKernel;


    //------------------------------------------------------------------------
    // GraphImplementation virtuals
    //------------------------------------------------------------------------
    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<SimulationLoopKernel> create(uint32_t numTimesteps, Model::ProcessGroupVector &timestepProcessGroups, 
                                                        const Model::ProcessGroupVector &beginProcessGroups = {},
                                                        const Model::ProcessGroupVector &endProcessGroups = {},
                                                        const std::string &name = "")
    {
        return std::make_shared<SimulationLoopKernel>(Private(), numTimesteps, timestepProcessGroups,
                                                      beginProcessGroups, endProcessGroups, name);
    }
};
}
