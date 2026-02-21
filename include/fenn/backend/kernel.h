#pragma once

// Model includes
#include "model/graph.h"

// Assembler includes
#include "fenn/assembler/register_allocator.h"

// Forward declarations
namespace FeNN::Assembler
{
class CodeGenerator;
}

//----------------------------------------------------------------------------
// FeNN::Backend::GraphImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class GraphImplementation
{
public:
    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;
};

//----------------------------------------------------------------------------
// FeNN::Backend::SimpleGraph
//----------------------------------------------------------------------------
class SimpleGraph : public GraphImplementation, public Model::SimpleGraph
{
public:
    using Model::SimpleGraph::SimpleGraph;

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
    static std::shared_ptr<SimpleGraph> create(const Model::ProcessGroupVector &processGroups,
                                               const std::string &name = "")
    {
        return std::make_shared<SimpleGraph>(Private(), processGroups, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::SimulationLoopGraph
//----------------------------------------------------------------------------
class SimulationLoopGraph : public GraphImplementation, public Model::SimulationLoopGraph
{
public:
    using Model::SimulationLoopGraph::SimulationLoopGraph;


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
    static std::shared_ptr<SimulationLoopGraph> create(uint32_t numTimesteps, Model::ProcessGroupVector &timestepProcessGroups, 
                                                       const Model::ProcessGroupVector &beginProcessGroups = {},
                                                       const Model::ProcessGroupVector &endProcessGroups = {},
                                                       const std::string &name = "")
    {
        return std::make_shared<SimulationLoopGraph>(Private(), numTimesteps, timestepProcessGroups,
                                                     beginProcessGroups, endProcessGroups, name);
    }
};
}
