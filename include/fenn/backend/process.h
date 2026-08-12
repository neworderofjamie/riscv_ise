#pragma once

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/process.h"
#include "frontend/runtime.h"

// FeNN assembler includes
#include "fenn/assembler/register_allocator.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"
#include "fenn/backend/model.h"

// Forward declarations
namespace FeNN::Assembler
{
class CodeGenerator;
}
namespace FeNN::Backend
{
class EnvironmentExternal;
class EnvironmentMergedField;
class EnvironmentLiteral;
class KernelImplementation;
class MergedFields;
class Runtime;
}

//----------------------------------------------------------------------------
// FeNN::Backend::ProcessImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class FENN_BACKEND_EXPORT ProcessImplementation
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const = 0;

    //! Update the max-row length to support this process
    virtual void updateMaxDMABufferSize(size_t&) const
    {
    }

    //virtual void updateLUTObjectIDs(std::unordered_set<Model::StateObjectID> m_LUTObjectIDs;)

    //! Generate code to implement process
    virtual void generateCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                              const KernelImplementation &kernel, MergedFields &mergedFields, 
                              Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg, 
                              Assembler::ScalarRegisterPtr groupIndReg, std::optional<uint32_t> numTimesteps, 
                              uint32_t &fieldBase, Assembler::CodeGenerator &c, 
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;
    
    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    uint32_t getMaxUnroll() const{ return m_MaxUnroll; }
    void setMaxUnroll(uint32_t maxUnroll){ m_MaxUnroll = maxUnroll; }

protected:
    ProcessImplementation() : m_MaxUnroll(4)
    {}

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    uint32_t m_MaxUnroll;
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
        const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
        const KernelImplementation &kernel, MergedFields &mergedFields,
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        std::optional<uint32_t> numTimesteps, Assembler::CodeGenerator &processCodeGenerator, Assembler::CodeGenerator &sharedCodeGenerator,
        Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;

    //----------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //----------------------------------------------------------------------------
    virtual void generateCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime,
                              const KernelImplementation &kernel, MergedFields &mergedFields, 
                              Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg,
                              Assembler::ScalarRegisterPtr groupIndReg, std::optional<uint32_t> numTimesteps, 
                              uint32_t &fieldBase, Assembler::CodeGenerator &c, 
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
};

//----------------------------------------------------------------------------
// FeNN::Backend::EventDrivenProcessImplementation
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT EventDrivenProcessImplementation : public ProcessImplementation
{
public:
    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    virtual void generateArchetypeCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                       const KernelImplementation &kernel, MergedFields &mergedFields,
                                       Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg, 
                                       Assembler::ScalarRegisterPtr preIndReg, std::optional<uint32_t> numTimesteps, 
                                       Assembler::CodeGenerator &processCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const = 0;

    //----------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //----------------------------------------------------------------------------
    virtual void generateCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime,
                              const KernelImplementation &kernel, MergedFields &mergedFields, 
                              Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg, 
                              Assembler::ScalarRegisterPtr groupIndReg, std::optional<uint32_t> numTimesteps, 
                              uint32_t &fieldBase, Assembler::CodeGenerator &c, 
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                              Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
};

//----------------------------------------------------------------------------
// FeNN::Backend::NeuronUpdateProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT NeuronUpdateProcess : public Frontend::NeuronUpdateProcess, public TimeDrivenProcessImplementation
{
public:
    using Frontend::NeuronUpdateProcess::NeuronUpdateProcess;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override final;
    
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
        const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
        const KernelImplementation &kernel, MergedFields &mergedFields,
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        std::optional<uint32_t> numTimesteps, Assembler::CodeGenerator &processCodeGenerator, 
        Assembler::CodeGenerator &sharedCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
        Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
   
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<NeuronUpdateProcess> create(const std::string &code, const Frontend::VariableMap &variables, 
                                                       const Frontend::EventSinkMap &outputEventSinks = {}, 
                                                       const CompilerFrontend::Type::ResolvedType &defaultScalarLiteralType = CompilerFrontend::Type::S8_7,
                                                       const std::string &name = "")
    {
        return std::make_shared<NeuronUpdateProcess>(Private(), code, variables, outputEventSinks, defaultScalarLiteralType, name);
    }
};

//----------------------------------------------------------------------------
// FeNN::Backend::DenseEventPropagationProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT DenseEventPropagationProcess : public Frontend::EventPropagationProcess, public EventDrivenProcessImplementation
{
public:
    DenseEventPropagationProcess(Private, std::shared_ptr<const Frontend::EventSource> inputEventSource, 
                                 Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                 const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const Frontend::State>> getAllState() const override final;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override final;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                 uint32_t &compatibleSplitDimensions,
                                                 uint32_t &compatibleIndexDimensions) const override final;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const override final;

    //! Update the max-row length to support this process
    virtual void updateMaxDMABufferSize(size_t&) const override final;

    //------------------------------------------------------------------------
    // EventDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------
    virtual void generateArchetypeCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                       const KernelImplementation &kernel, MergedFields &mergedFields,
                                       Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
                                       Assembler::ScalarRegisterPtr preIndReg, std::optional<uint32_t> numTimesteps, 
                                       Assembler::CodeGenerator &processCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getWeight() const{ return m_Weight; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<DenseEventPropagationProcess> create(std::shared_ptr<const Frontend::EventSource> inputEventSource, 
                                                                Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                                                const std::string &name = "")
    {
        return std::make_shared<DenseEventPropagationProcess>(Private(), inputEventSource, weight, target, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Frontend::VariablePtr m_Weight;
};

//----------------------------------------------------------------------------
// FeNN::Backend::SparseEventPropagationProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT SparseEventPropagationProcess : public Frontend::EventPropagationProcess, public EventDrivenProcessImplementation
{
public:
    SparseEventPropagationProcess(Private, std::shared_ptr<const Frontend::EventSource> inputEventSource, 
                                  Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                  size_t numSparseConnectivityBits, const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const Frontend::State>> getAllState() const override final;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override final;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                 uint32_t &compatibleSplitDimensions,
                                                 uint32_t &compatibleIndexDimensions) const override final;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const override final;

    //! Update the max-row length to support this process
    virtual void updateMaxDMABufferSize(size_t&) const override final;

    //------------------------------------------------------------------------
    // EventDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------
    virtual void generateArchetypeCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                       const KernelImplementation &kernel, MergedFields &mergedFields,
                                       Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
                                       Assembler::ScalarRegisterPtr preIndReg, std::optional<uint32_t> numTimesteps, 
                                       Assembler::CodeGenerator &processCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getWeight() const{ return m_Weight; }
    size_t getNumSparseConnectivityBits() const{ return m_NumSparseConnectivityBits; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<SparseEventPropagationProcess> create(std::shared_ptr<const Frontend::EventSource> inputEventSource, 
                                                                 Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                                                 size_t numSparseConnectivityBits, const std::string &name = "")
    {
        return std::make_shared<SparseEventPropagationProcess>(Private(), inputEventSource, weight, target, numSparseConnectivityBits, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Frontend::VariablePtr m_Weight;
    size_t m_NumSparseConnectivityBits;
};

//----------------------------------------------------------------------------
// FeNN::Backend::DelayEventPropagationProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT DelayEventPropagationProcess : public Frontend::EventPropagationProcess, public EventDrivenProcessImplementation
{
public:
    DelayEventPropagationProcess(Private, std::shared_ptr<const Frontend::EventSource>inputEventSource, 
                                 Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                 size_t numDelayBits, const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const Frontend::State>> getAllState() const override final;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override final;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                 uint32_t &compatibleSplitDimensions,
                                                 uint32_t &compatibleIndexDimensions) const override final;

    //------------------------------------------------------------------------
    // ProcessImplementation virtuals
    //------------------------------------------------------------------------
    //! Update the memory compatibility of a variable associated with this process
    virtual void updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                          MemSpace &compatibleMemSpaces) const override final;

    //! Update the max-row length to support this process
    virtual void updateMaxDMABufferSize(size_t&) const override final;

    //------------------------------------------------------------------------
    // EventDrivenProcessImplementation virtuals
    //------------------------------------------------------------------------
    virtual void generateArchetypeCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                       const KernelImplementation &kernel, MergedFields &mergedFields,
                                       Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
                                       Assembler::ScalarRegisterPtr preIndReg, std::optional<uint32_t> numTimesteps, 
                                       Assembler::CodeGenerator &processCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getWeight() const{ return m_Weight; }
    size_t getNumDelayBits() const{ return m_NumDelayBits; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<DelayEventPropagationProcess> create(std::shared_ptr<const Frontend::EventSource>inputEventSource, 
                                                                Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                                                size_t numDelayBits, const std::string &name = "")
    {
        return std::make_shared<DelayEventPropagationProcess>(Private(), inputEventSource, weight, target, numDelayBits, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Frontend::VariablePtr m_Weight;
    size_t m_NumDelayBits;
};


//----------------------------------------------------------------------------
// FeNN::Backend::RNGInitProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT RNGInitProcess : public Frontend::RNGInitProcess, public TimeDrivenProcessImplementation
{
public:
    RNGInitProcess(Private, Frontend::VariablePtr seed, const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override final;
    
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
        const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
        const KernelImplementation &kernel, MergedFields &mergedFields,
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        std::optional<uint32_t> numTimesteps, Assembler::CodeGenerator &processCodeGenerator, 
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
class FENN_BACKEND_EXPORT MemsetProcess : public Frontend::MemsetProcess, public TimeDrivenProcessImplementation
{
public:
    using Frontend::MemsetProcess::MemsetProcess;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override final;
    
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
        const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
        const KernelImplementation &kernel, MergedFields &mergedFields,
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        std::optional<uint32_t> numTimesteps, Assembler::CodeGenerator &processCodeGenerator, 
        Assembler::CodeGenerator &sharedCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
        Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const override final;
    
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<MemsetProcess> create(Frontend::VariablePtr target,
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
                            Assembler::ScalarRegisterPtr targetReg,
                            std::variant<Assembler::ScalarRegisterPtr, int, std::monostate> stride) const;
    void generateLLMMemset(Assembler::CodeGenerator &c,
                           Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                           Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                           Assembler::ScalarRegisterPtr targetReg,
                           std::variant<Assembler::ScalarRegisterPtr, int, std::monostate> stride) const;
};

//----------------------------------------------------------------------------
// FeNN::Backend::BroadcastProcess
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT BroadcastProcess : public Frontend::Process, public TimeDrivenProcessImplementation
{
public:
    BroadcastProcess(Private, Frontend::VariablePtr source, Frontend::VariablePtr target, 
                     const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const Frontend::State>> getAllState() const override final;

    //! Get vector of event source objects used by this process
    virtual std::vector<std::shared_ptr<const Frontend::EventSource>> getAllEventSources() const override final;

    //! Get vector of event source objects used by this process
    virtual std::vector<Frontend::Sliced<Frontend::EventSink>> getAllEventSinks() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const override final;
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                 uint32_t &compatibleSplitDimensions,
                                                 uint32_t &compatibleIndexDimensions) const override;


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
        const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
        const KernelImplementation &kernel, MergedFields &mergedFields,
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        std::optional<uint32_t> numTimesteps, Assembler::CodeGenerator &processCodeGenerator, 
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
