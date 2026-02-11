#pragma once

// Model includes
#include "model/process.h"

// Assembler includes
#include "fenn/assembler/register_allocator.h"

// Forward declarations
namespace FeNN::Assembler
{
class CodeGenerator;
}

//----------------------------------------------------------------------------
// FeNN::Backend::IProcessImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class ProcessImplementation
{
public:
    //! Structure specifying which memory spaces a variable can be implemented in
    struct MemSpaceCompatibility
    {
        bool llm;
        bool uram;
        bool bram;
        bool dram;
        bool uramLLM;
    };

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(Model::VariablePtr variable, 
                                             MemSpaceCompatibility &memSpaceCompatibility) const = 0;

    //! Update the max-row length to support this process
    virtual void updateMaxRowLength(size_t &maxRowLength) const
    {
    }

    //virtual void updateLUTObjectIDs(std::unordered_set<Model::StateObjectID> m_LUTObjectIDs;)

    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;
};

//----------------------------------------------------------------------------
// FeNN::Backend::NeuronUpdateProcess
//----------------------------------------------------------------------------
class NeuronUpdateProcess : public ProcessImplementation, public Model::NeuronUpdateProcess
{
public:
    using Model::NeuronUpdateProcess::NeuronUpdateProcess;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(Model::VariablePtr variable, 
                                           MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<NeuronUpdateProcess> create(const std::string &code, const Model::ParameterMap &parameters, 
                                                       const Model::VariableMap &variables, const Model::EventContainerMap &outputEvents = {},
                                                       const std::string &name = "")
    {
        return std::make_shared<NeuronUpdateProcess>(Private(), code, parameters, variables, outputEvents, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventPropagationProcess
//----------------------------------------------------------------------------
class EventPropagationProcess : public ProcessImplementation, public Model::EventPropagationProcess
{
public:
    using Model::EventPropagationProcess::EventPropagationProcess;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(Model::VariablePtr variable,
                                           MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //! Update the max-row length to support this process
    virtual void updateMaxRowLength(size_t &maxRowLength) const override final;

    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventPropagationProcess> create(std::shared_ptr<const Model::EventContainer> inputEvents, 
                                                           Model::VariablePtr weight, Model::VariablePtr target, 
                                                           size_t numSparseConnectivityBits = 0,
                                                           size_t numDelayBits = 0,
                                                           const std::string &name = "")
    {
        return std::make_shared<EventPropagationProcess>(Private(), inputEvents, weight, target, 
                                                         numSparseConnectivityBits, numDelayBits, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::RNGInitProcess
//----------------------------------------------------------------------------
class RNGInitProcess : public ProcessImplementation, public Model::RNGInitProcess
{
public:
    using Model::RNGInitProcess::RNGInitProcess;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(Model::VariablePtr variable, 
                                           MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<RNGInitProcess> create(Model::VariablePtr seed, const std::string &name = "")
    {
        return std::make_shared<RNGInitProcess>(Private(), seed, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::MemsetProcess
//----------------------------------------------------------------------------
class MemsetProcess : public ProcessImplementation, public Model::MemsetProcess
{
public:
    using Model::MemsetProcess::MemsetProcess;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(Model::VariablePtr variable, 
                                           MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Model::MemsetProcess> create(Model::VariablePtrBackendState target, const std::string &name = "")
    {
        return std::make_shared<MemsetProcess>(Private(), target, name);
    }

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void generateURAMMemset(Assembler::CodeGenerator &codeGenerator,
                            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                            Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                            size_t numVectors, Assembler::ScalarRegisterAllocator::RegisterPtr targetReg) const;
    void generateLLMMemset(Assembler::CodeGenerator &codeGenerator,
                           Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                           Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                           size_t numVectors, Assembler::ScalarRegisterAllocator::RegisterPtr targetReg) const;
};

//----------------------------------------------------------------------------
// FeNN::Backend::BroadcastProcess
//----------------------------------------------------------------------------
class BroadcastProcess : public ProcessImplementation, public Model::BroadcastProcess
{
public:
    using Model::BroadcastProcess::BroadcastProcess;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(Model::VariablePtr variable, 
                                           MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //! Generate code to implement process
    virtual void generateCode(Assembler::CodeGenerator &codeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<BroadcastProcess> create(Model::VariablePtr source, Model::VariablePtrBackendState target,
                                                    const std::string &name = "")
    {
        return std::make_shared<BroadcastProcess>(Private(), source, target, name);
    }
};
}