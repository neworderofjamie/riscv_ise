#pragma once

// Model includes
#include "model/process.h"

// FeNN common includes
#include "fenn/common/isa.h"

// FeNN assembler includes
#include "fenn/assembler/register_allocator.h"

// FeNN backend includes
#include "fenn/backend/model.h"

// Forward declarations
namespace Backend
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
    virtual void updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                             MemSpaceCompatibility &memSpaceCompatibility) const = 0;

    //! Update which dimension variable associated with this process can be split down
    virtual void updateDeviceSplit(std::shared_ptr<const ::Model::State> state,
                                   std::vector<bool> &splits) const = 0;

    //! Update the max-row length to support this process
    virtual void updateMaxDMABufferSize(size_t &maxRowLength) const
    {
    }

    //virtual void updateLUTObjectIDs(std::unordered_set<Model::StateObjectID> m_LUTObjectIDs;)

    //! Generate code to implement process
    virtual void generateCode(const ::Backend::MergedProcess &mergedProcess, 
                              const Model &model, Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;
};

//----------------------------------------------------------------------------
// FeNN::Backend::TimeDrivenProcessImplementation
//----------------------------------------------------------------------------
class TimeDrivenProcessImplementation : public ProcessImplementation
{
public:
    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    virtual void generateMergedPreambleCode(const ::Backend::MergedProcess &mergedProcess,
                                            const Model &model, EnvironmentExternal &environment, 
                                            Assembler::CodeGenerator &c,
                                            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                            Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
    {}

    virtual void generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                       const Model &model, EnvironmentExternal &environment, 
                                       MergedFields &fields, Assembler::CodeGenerator &c,
                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;

    //----------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //----------------------------------------------------------------------------
    virtual void generateCode(const ::Backend::MergedProcess &mergedProcess,
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
    // P virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const ::Model::Model &model) const override final
    {
        // Superclass
        P::updateMergeHash(hash, model);

        // Get all  state associated with this process
        const auto allState = this->getAllState();

        // Update hash with memory space compatibility of all state
        const auto &fennModel = dynamic_cast<const Model&>(model);
        for(const auto &s : allState) {
            fennModel.getStateMemSpaceCompatibility(s).updateHash(hash);
        }
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::NeuronUpdateProcess
//----------------------------------------------------------------------------
class NeuronUpdateProcess : public ProcessImplementationBase<::Model::NeuronUpdateProcess>
{
public:
    using ProcessImplementationBase<::Model::NeuronUpdateProcess>::ProcessImplementationBase;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                             MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //------------------------------------------------------------------------
    // TimeDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------ 
    virtual void generateMergedPreambleCode(const ::Backend::MergedProcess &mergedProcess,
                                            const Model &model, EnvironmentExternal &environment, 
                                            Assembler::CodeGenerator &c,
                                            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                            Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    virtual void generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                       const Model &model, EnvironmentExternal &environment, 
                                       MergedFields &fields, Assembler::CodeGenerator &c,
                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<NeuronUpdateProcess> create(const std::string &code, const ::Model::VariableMap &variables, 
                                                       const ::Model::EventContainerMap &outputEvents = {}, const std::string &name = "")
    {
        return std::make_shared<NeuronUpdateProcess>(Private(), code, variables, outputEvents, name);
    }

protected:
    void generateArchetype(Assembler::CodeGenerator &c, 
                           Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                           Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const;
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventPropagationProcess
//----------------------------------------------------------------------------
class EventPropagationProcess : public ProcessImplementationBase<::Model::EventPropagationProcess, ProcessImplementation>
{
public:
    using ProcessImplementationBase<::Model::EventPropagationProcess, ProcessImplementation>::ProcessImplementationBase;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state,
                                             MemSpaceCompatibility &memSpaceCompatibility) const override final;

                                             //! Update which dimension variable associated with this process can be split down
    virtual void updateDeviceSplit(std::shared_ptr<const ::Model::State> state,
                                   std::vector<bool> &splits) const override final;
    //! Update the maximum DMA buffer size to support this process
    virtual void updateMaxDMABufferSize(size_t &maxRowLength) const override final;

    //! Generate code to implement process
    virtual void generateCode(const ::Backend::MergedProcess &mergedProcess,
                              Common::Reg fieldBaseReg, Assembler::CodeGenerator &c,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventPropagationProcess> create(std::shared_ptr<const ::Model::EventContainer> inputEvents, 
                                                           ::Model::VariablePtr weight, ::Model::VariablePtr target, 
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
class RNGInitProcess : public ProcessImplementationBase<::Model::RNGInitProcess>
{
public:
    using ProcessImplementationBase<::Model::RNGInitProcess>::ProcessImplementationBase;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                             MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //------------------------------------------------------------------------
    // TimeDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------ 
    virtual void generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                       const Model &model, EnvironmentExternal &environment, 
                                       MergedFields &fields, Assembler::CodeGenerator &c,
                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<RNGInitProcess> create(::Model::VariablePtr seed, const std::string &name = "")
    {
        return std::make_shared<RNGInitProcess>(Private(), seed, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::MemsetProcess
//----------------------------------------------------------------------------
class MemsetProcess : public ProcessImplementationBase<::Model::MemsetProcess>
{
public:
    using ProcessImplementationBase<::Model::MemsetProcess>::ProcessImplementationBase;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                             MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //------------------------------------------------------------------------
    // TimeDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------ 
    virtual void generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                       const Model &model, EnvironmentExternal &environment, 
                                       MergedFields &fields, Assembler::CodeGenerator &c,
                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<::Model::MemsetProcess> create(::Model::VariablePtr target,
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
                            size_t numVectors, Assembler::ScalarRegisterAllocator::RegisterPtr targetReg) const;
    void generateLLMMemset(Assembler::CodeGenerator &c,
                           Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                           Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                           size_t numVectors, Assembler::ScalarRegisterAllocator::RegisterPtr targetReg) const;
};

//----------------------------------------------------------------------------
// FeNN::Backend::BroadcastProcess
//----------------------------------------------------------------------------
class BroadcastProcess : public ProcessImplementationBase<::Model::BroadcastProcess>
{
public:
    using ProcessImplementationBase<::Model::BroadcastProcess>::ProcessImplementationBase;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                             MemSpaceCompatibility &memSpaceCompatibility) const override final;

    //------------------------------------------------------------------------
    // TimeDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------ 
    virtual void generateMergedPreambleCode(const ::Backend::MergedProcess &mergedProcess,
                                            const Model &model, EnvironmentExternal &environment, 
                                            Assembler::CodeGenerator &c,
                                            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                            Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    virtual void generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                       const Model &model, EnvironmentExternal &environment, 
                                       MergedFields &fields, Assembler::CodeGenerator &c,
                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<BroadcastProcess> create(::Model::VariablePtr source, ::Model::VariablePtr target,
                                                    const std::string &name = "")
    {
        return std::make_shared<BroadcastProcess>(Private(), source, target, name);
    }
};
}