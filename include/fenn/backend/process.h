#pragma once

// GeNN includes
#include "gennUtils.h"

// Frontend includes
#include "frontend/process.h"

// FeNN common includes
#include "fenn/common/isa.h"

// FeNN assembler includes
#include "fenn/assembler/register_allocator.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"
#include "fenn/backend/model.h"

// Forward declarations
namespace Frontend
{
class MergedProcess;
}
namespace FeNN::Assembler
{
class CodeGenerator;
}
namespace FeNN::Backend
{
class EnvironmentExternal;
class EnvironmentMergedField;
class EnvironmentLiteral;
class Model;
class MergedFields;
}


//----------------------------------------------------------------------------
// FeNN::Backend::ProcessImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class ProcessImplementation
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const = 0;

    //! Update the max-row length to support this process
    virtual void updateMaxDMABufferSize(size_t &maxRowLength) const
    {
    }

    //virtual void updateLUTObjectIDs(std::unordered_set<Model::StateObjectID> m_LUTObjectIDs;)

    //! Generate code to implement process
    virtual void generateCode(const Frontend::MergedProcess &mergedProcess, 
                              const Model &model, Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;
};

//----------------------------------------------------------------------------
// FeNN::Backend::TimeDrivenProcessImplementation
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT TimeDrivenProcessImplementation : public ProcessImplementation
{
public:
    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    virtual std::vector<Compiler::RegisterPtr> generateArchetypeCode(
        const Frontend::MergedProcess &mergedProcess, const Model &model, MergedFields &mergedFields,
        Assembler::ScalarRegisterAllocator::RegisterPtr fieldBaseReg,
        Assembler::CodeGenerator &processCodeGenerator, Assembler::CodeGenerator &sharedCodeGenerator,
        Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;

    //----------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //----------------------------------------------------------------------------
    virtual void generateCode(const Frontend::MergedProcess &mergedProcess,
                              const Model &model, Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
};


//----------------------------------------------------------------------------
// FeNN::Backend::ProcessImplementationBase
//----------------------------------------------------------------------------
//! Helper class which automatically adds memory space compatibility 
//! information for all process state to merge hash
template<typename P, typename B = TimeDrivenProcessImplementation>
class ProcessImplementationBase : public B, public P
{
public:
    using P::P;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override
    {
        // Superclass
        P::updateMergeHash(hash, model);

        // Get all  state associated with this process
        const auto allState = this->getAllState();

        // Update hash with memory space compatibility of all state
        // **NOTE** this is unaffected by whether DRAM is used or not so we can just set true
        const auto &fennModel = dynamic_cast<const Model&>(model);
        for(const auto &s : allState) {
            GeNN::Utils::updateHash(fennModel.getStateMemSpace(s, true), hash);
        }
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::NeuronUpdateProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT NeuronUpdateProcess : public ProcessImplementationBase<Frontend::NeuronUpdateProcess>
{
public:
    using ProcessImplementationBase<Frontend::NeuronUpdateProcess>::ProcessImplementationBase;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const override final;

    //------------------------------------------------------------------------
    // TimeDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------ 
    virtual std::vector<Compiler::RegisterPtr> generateArchetypeCode(
        const Frontend::MergedProcess &mergedProcess, const Model &model, MergedFields &mergedFields,
        Assembler::ScalarRegisterAllocator::RegisterPtr fieldBaseReg, Assembler::CodeGenerator &processCodeGenerator, 
        Assembler::CodeGenerator &sharedCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
        Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
   
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<NeuronUpdateProcess> create(const std::string &code, const Frontend::VariableMap &variables, 
                                                       const Frontend::EventContainerMap &outputEvents = {}, const std::string &name = "")
    {
        return std::make_shared<NeuronUpdateProcess>(Private(), code, variables, outputEvents, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventPropagationProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT EventPropagationProcess : public ProcessImplementationBase<Frontend::EventPropagationProcess, ProcessImplementation>
{
public:
    using ProcessImplementationBase<Frontend::EventPropagationProcess, ProcessImplementation>::ProcessImplementationBase;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state,
                                          MemSpace &compatibleMemSpaces) const override final;

    //! Update the maximum DMA buffer size to support this process
    virtual void updateMaxDMABufferSize(size_t &maxRowLength) const override final;

    //! Generate code to implement process
    virtual void generateCode(const Frontend::MergedProcess &mergedProcess, 
                              const Model &model, Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventPropagationProcess> create(std::shared_ptr<const Frontend::EventContainer> inputEvents, 
                                                           Frontend::VariablePtr weight, Frontend::VariablePtr target, 
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
class FENN_BACKEND_EXPORT RNGInitProcess : public ProcessImplementationBase<Frontend::RNGInitProcess>
{
public:
    RNGInitProcess(Private, Frontend::VariablePtr seed, const std::string &name);

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const override final;

    //------------------------------------------------------------------------
    // TimeDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------ 
    virtual std::vector<Compiler::RegisterPtr> generateArchetypeCode(
        const Frontend::MergedProcess &mergedProcess, const Model &model, MergedFields &mergedFields,
        Assembler::ScalarRegisterAllocator::RegisterPtr fieldBaseReg, Assembler::CodeGenerator &processCodeGenerator, 
        Assembler::CodeGenerator &sharedCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
        Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
    
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<RNGInitProcess> create(Frontend::VariablePtr seed, const std::string &name = "")
    {
        return std::make_shared<RNGInitProcess>(Private(), seed, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::MemsetProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT MemsetProcess : public ProcessImplementationBase<Frontend::MemsetProcess>
{
public:
    using ProcessImplementationBase<Frontend::MemsetProcess>::ProcessImplementationBase;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const override final;

    //------------------------------------------------------------------------
    // TimeDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------ 
   virtual std::vector<Compiler::RegisterPtr> generateArchetypeCode(
        const Frontend::MergedProcess &mergedProcess, const Model &model, MergedFields &mergedFields,
        Assembler::ScalarRegisterAllocator::RegisterPtr fieldBaseReg, Assembler::CodeGenerator &processCodeGenerator, 
        Assembler::CodeGenerator &sharedCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
        Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
    
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Frontend::MemsetProcess> create(Frontend::VariablePtr target,
                                                          const std::string &name = "")
    {
        return std::make_shared<MemsetProcess>(Private(), target, name);
    }

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void generateURAMMemset(Assembler::CodeGenerator &c,
                            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                            Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                            Assembler::ScalarRegisterAllocator::RegisterPtr targetReg,
                            Compiler::EnvironmentItem numElements) const;
    void generateLLMMemset(Assembler::CodeGenerator &c,
                           Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                           Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                           Assembler::ScalarRegisterAllocator::RegisterPtr targetReg,
                           Compiler::EnvironmentItem numElements) const;
};

//----------------------------------------------------------------------------
// FeNN::Backend::BroadcastProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT BroadcastProcess : public ProcessImplementationBase<Frontend::Process>
{
public:
    BroadcastProcess(Private, Frontend::VariablePtr source, Frontend::VariablePtr target, 
                     const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const Frontend::State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override;
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                 uint32_t &compatibleSplitDimensions) const override;


    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const override final;

    //------------------------------------------------------------------------
    // TimeDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------ 
    virtual std::vector<Compiler::RegisterPtr> generateArchetypeCode(
        const Frontend::MergedProcess &mergedProcess, const Model &model, MergedFields &mergedFields,
        Assembler::ScalarRegisterAllocator::RegisterPtr fieldBaseReg, Assembler::CodeGenerator &processCodeGenerator, 
        Assembler::CodeGenerator &sharedCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
        Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getSource() const{ return m_Source; }
    const auto getTarget() const{ return m_Target; }


    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<BroadcastProcess> create(Frontend::VariablePtr source, Frontend::VariablePtr target,
                                                    const std::string &name = "")
    {
        return std::make_shared<BroadcastProcess>(Private(), source, target, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Frontend::VariablePtr m_Source;
    Frontend::VariablePtr m_Target;
};
}
