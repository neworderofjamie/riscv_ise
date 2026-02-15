#include "fenn/backend/process.h"

// Common include
#include "common/utils.h"

// FeNN common includes
#include "fenn/common/isa.h"

// Assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/assembler_utils.h"
#include "fenn/assembler/register_allocator.h"

// Model includes
#include "model/variable.h"

using namespace FeNN::Assembler;
using namespace FeNN::Common;

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
/*namespace
{
//------------------------------------------------------------------------
// NeuronVarBase
//------------------------------------------------------------------------
//! Base class for helper classes used to manage memory access to neuron variables
class NeuronVarBase
{
public:
    //! Generate code to load vector register reg from memory before unrolled loop iteration r
    virtual void genLoad(::EnvironmentBase &env, VectorRegisterAllocator::RegisterPtr reg, uint32_t r) = 0;

    //! Generate code to store vector register reg to memory after unrolled loop iteration r
    virtual void genStore(::EnvironmentBase &env, VectorRegisterAllocator::RegisterPtr reg, uint32_t r) = 0;

    //! Generate code to advance pointer after numUnrolls unrolled 
    virtual void genIncrement(CodeGenerator &c, uint32_t numUnrolls, 
                              VectorRegisterAllocator::RegisterPtr numUnrollBytesReg) = 0;
    virtual ScalarRegisterAllocator::RegisterPtr getLoopCountScalarReg() const = 0;
    virtual bool needsNumUnrollBytesReg() const = 0;
};

//------------------------------------------------------------------------
// URAMNeuronVar
//------------------------------------------------------------------------
//! Implementation of NeuronVarBase to handle standard variables located in URAM
class URAMNeuronVar : public NeuronVarBase
{
public:
    URAMNeuronVar(const std::string &varName, std::shared_ptr<const Model::Variable> var, 
                  CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                  const Model::StateFields &stateFields, std::optional<uint32_t> numTimesteps,
                  ScalarRegisterAllocator::RegisterPtr timeReg, ScalarRegisterAllocator::RegisterPtr numVarBytesReg)
    {
        // Allocate scalar register to hold address of variable
        m_ReadBufferReg = scalarRegisterAllocator.getRegister((varName + "Buffer X").c_str());

        // Generate code to load address
        c.lw(*m_ReadBufferReg, Reg::X0, stateFields.at(var));

        // **TODO** currently this just handles providing entire simulation kernel worth of variable data or
        // recording variables for entire simulation - extend to support axonal delays and ring-buffer recording
        const size_t numBufferTimesteps = var->getNumBufferTimesteps();
        if (numBufferTimesteps != 1) {
            // Check there is a buffer entry for each timestep with one extra
            // **NOTE** variables get read from timestep and written to timestep + 1 hence extra buf
            if(numBufferTimesteps < (numTimesteps.value() + 1)) {
                throw std::runtime_error("Variables need to be buffered for " + std::to_string(numTimesteps.value() + 1u) + " timesteps");
            }

            ALLOCATE_SCALAR(STmp);
            c.mul(*STmp, *timeReg, *numVarBytesReg);
            c.add(*m_ReadBufferReg, *m_ReadBufferReg, *STmp);

            // Allocate additional register for writing variable
            // **TODO** should be lazy
            m_WriteBufferReg = scalarRegisterAllocator.getRegister((varName + "BufferWrite X").c_str());
            c.add(*m_WriteBufferReg, *m_ReadBufferReg, *numVarBytesReg);
        }
    }

    //--------------------------------------------------------------------
    // NeuronVarBase virtuals
    //--------------------------------------------------------------------
    virtual void genLoad(::EnvironmentBase &env, VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vloadv(*reg, *m_ReadBufferReg, 64 * r);
    }

    virtual void genStore(::EnvironmentBase &env, VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vstore(*reg, (m_WriteBufferReg ? *m_WriteBufferReg : *m_ReadBufferReg), 64 * r);
    }

    virtual void genIncrement(CodeGenerator &c, uint32_t numUnrolls,
                              VectorRegisterAllocator::RegisterPtr) final override
    {
        c.addi(*m_ReadBufferReg, *m_ReadBufferReg, 64 * numUnrolls);
        if(m_WriteBufferReg) {
            c.addi(*m_WriteBufferReg, *m_WriteBufferReg, 64 * numUnrolls);
        }
    }

    virtual ScalarRegisterAllocator::RegisterPtr getLoopCountScalarReg() const final override
    {
        return m_ReadBufferReg;
    }

    virtual bool needsNumUnrollBytesReg() const final override
    {
        return false;
    }

private:
    //--------------------------------------------------------------------
    // Members
    //--------------------------------------------------------------------
    ScalarRegisterAllocator::RegisterPtr m_ReadBufferReg;
    ScalarRegisterAllocator::RegisterPtr m_WriteBufferReg;
};

//------------------------------------------------------------------------
// LLMNeuronVar
//------------------------------------------------------------------------
//! Implementation of NeuronVarBase to handle variables stored in LLM.
//! Typically these are the output of sparse connectivity
class LLMNeuronVar : public NeuronVarBase
{
public:
    LLMNeuronVar(const std::string &varName, std::shared_ptr<const Model::Variable> var, 
                 CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                 VectorRegisterAllocator &vectorRegisterAllocator,
                 const Model::StateFields &stateFields)
    {
        assert(var->getNumBufferTimesteps() == 1);

        // Allocate vector register to hold address of variable
        m_BufferReg = vectorRegisterAllocator.getRegister((varName + "Buffer V").c_str());

        // Generate code to load address
        ALLOCATE_SCALAR(STmp);
        c.lw(*STmp, Reg::X0, stateFields.at(var));
        c.vfill(*m_BufferReg, *STmp);
    }

    //--------------------------------------------------------------------
    // NeuronVarBase virtuals
    //--------------------------------------------------------------------
    virtual void genLoad(::EnvironmentBase &env, VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vloadl(*reg, *m_BufferReg, 2 * r);   
    }

    virtual void genStore(::EnvironmentBase &env, VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vstorel(*reg, *m_BufferReg, 2 * r);    
    }

    virtual void genIncrement(CodeGenerator &c, uint32_t,
                              VectorRegisterAllocator::RegisterPtr numUnrollBytesReg) final override
    {
        c.vadd(*m_BufferReg, *m_BufferReg, *numUnrollBytesReg);
    }

    virtual ScalarRegisterAllocator::RegisterPtr getLoopCountScalarReg() const final override
    {
        return nullptr;
    }

    virtual bool needsNumUnrollBytesReg() const final override
    {
        return true;
    }

private:
    //--------------------------------------------------------------------
    // Members
    //--------------------------------------------------------------------
    VectorRegisterAllocator::RegisterPtr m_BufferReg;
};

//------------------------------------------------------------------------
// URAMLLMNeuronVar
//------------------------------------------------------------------------
//! Implementation of NeuronVarBase to handle variables with a delayed 
//! input buffer stored in LLM but the variable itself in URAM.
//! Typically these are the output of delayed connectivity
class URAMLLMNeuronVar : public NeuronVarBase
{
public:
    URAMLLMNeuronVar(const std::string &varName, std::shared_ptr<const Model::Variable> var, 
                     CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                     VectorRegisterAllocator &vectorRegisterAllocator, const Model::StateFields &stateFields, 
                     ScalarRegisterAllocator::RegisterPtr timeReg)
        :   m_DelayStride(2 * var->getNumBufferTimesteps()), m_VectorRegisterAllocator(vectorRegisterAllocator)
    {
        // Check number of buffer timesteps is P.O.T.
        if(!::Utils::isPOT(var->getNumBufferTimesteps())) {
            throw std::runtime_error("When used as delayed event propagation targets, variables "
                                     "need to have a power-of-two number of buffer timesteps");
        }

        // Allocate scalar register to hold address of variable in URAM
        m_URAMBufferReg = scalarRegisterAllocator.getRegister((varName + "Buffer X").c_str());

        // Load URAM address from first word of field
        c.lw(*m_URAMBufferReg, Reg::X0, stateFields.at(var));

        // Allocate vector registers to hold address of variable in LLM and delay stride
        m_LLMBufferReg = vectorRegisterAllocator.getRegister((varName + "Buffer V").c_str());

        {
            ALLOCATE_SCALAR(STmp);
            ALLOCATE_SCALAR(STmp2);

            // Calculate time modulo delay buffer size
            c.andi(*STmp, *timeReg, var->getNumBufferTimesteps() - 1);

            // Double to get starting offset in bytes
            c.slli(*STmp, *STmp, 1);

            // Load LLM address from second word of field, add offset and broadcast
            c.lw(*STmp2, Reg::X0, stateFields.at(var) + 4);
            c.add(*STmp2, *STmp2, *STmp);
            c.vfill(*m_LLMBufferReg, *STmp2);
        }
    }

    //--------------------------------------------------------------------
    // NeuronVarBase virtuals
    //--------------------------------------------------------------------
    virtual void genLoad(::EnvironmentBase &env, VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        // Load from LLM buffer into temporary register
        auto denDelayFrontReg = m_VectorRegisterAllocator.get().getRegister("DenDelayFront V");
        env.getCodeGenerator().vloadl(*denDelayFrontReg, *m_LLMBufferReg, m_DelayStride * r);   

        // Load from URAM buffer into state register
        env.getCodeGenerator().vloadv(*reg, *m_URAMBufferReg, 64 * r);

        // Write zero back to LLM
        env.getCodeGenerator().vstorel(*env.getVectorRegister("_zero"), *m_LLMBufferReg, m_DelayStride * r);

        // Add new input from LLM to state register
        env.getCodeGenerator().vadd_s(*reg, *reg, *denDelayFrontReg);
    }

    virtual void genStore(::EnvironmentBase &env, VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vstore(*reg, *m_URAMBufferReg, 64 * r);
    }

    virtual void genIncrement(CodeGenerator &c, uint32_t numUnrolls,
                              VectorRegisterAllocator::RegisterPtr) final override
    {
        // Calculate how many bytes we need to advance LLM addresses
        // **TODO** VADDI instruction would save an instruction in this type of situation
        auto numUnrollBytesReg = m_VectorRegisterAllocator.get().getRegister("NumUnrollBytes V");
        c.vlui(*numUnrollBytesReg, numUnrolls * m_DelayStride);

        // Increment URAM and LLM pointers
        c.addi(*m_URAMBufferReg, *m_URAMBufferReg, 64 * numUnrolls);
        c.vadd(*m_LLMBufferReg, *m_LLMBufferReg, *numUnrollBytesReg);
    }

    virtual ScalarRegisterAllocator::RegisterPtr getLoopCountScalarReg() const final override
    {
        return m_URAMBufferReg;
    }

    virtual bool needsNumUnrollBytesReg() const final override
    {
        return false;
    }

private:
    //--------------------------------------------------------------------
    // Members
    //--------------------------------------------------------------------
    size_t m_DelayStride;
    ScalarRegisterAllocator::RegisterPtr m_URAMBufferReg;
    VectorRegisterAllocator::RegisterPtr m_LLMBufferReg;
    std::reference_wrapper<VectorRegisterAllocator> m_VectorRegisterAllocator;
};

//------------------------------------------------------------------------
// RowGeneratorBase
//------------------------------------------------------------------------
class RowGeneratorBase
{
public:
    RowGeneratorBase(CodeGenerator &c, std::shared_ptr<const Model::EventPropagationProcess> process,
                     const Model::StateFields &stateFields,
                     ScalarRegisterAllocator &scalarRegisterAllocator, 
                     VectorRegisterAllocator &vectorRegisterAllocator)
        :   m_Process(process), m_StateFields(stateFields), m_ScalarRegisterAllocator(scalarRegisterAllocator), 
        m_VectorRegisterAllocator(vectorRegisterAllocator)
    {
        // Allocate register for stride, calculate and load as immediate
        m_StrideReg = scalarRegisterAllocator.getRegister("SStride = X");
        c.li(*m_StrideReg, ceilDivide(process->getMaxRowLength(), 32) * 64);

    }

    //--------------------------------------------------------------------
    // Declared virtuals
    //--------------------------------------------------------------------
    virtual void generateRow(CodeGenerator &cg, ScalarRegisterAllocator::RegisterPtr weightBufferReg) = 0;

    //--------------------------------------------------------------------
    // Public API
    //--------------------------------------------------------------------
    auto getStrideReg() const{ return m_StrideReg; }

protected:
    //--------------------------------------------------------------------
    // Protected API
    //--------------------------------------------------------------------
    auto getProcess() const{ return m_Process; }

    auto &getStateFields(){ return m_StateFields.get(); }
    auto &getScalarRegisterAllocator(){ return m_ScalarRegisterAllocator.get(); }
    auto &getVectorRegisterAllocator(){ return m_VectorRegisterAllocator.get(); }

public:
    //--------------------------------------------------------------------
    // Public API
    //--------------------------------------------------------------------
    auto loadWeightBuffer(CodeGenerator &c, ScalarRegisterAllocator::RegisterPtr idPreReg)
    {
        auto &scalarRegisterAllocator = getScalarRegisterAllocator();

        // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
        ALLOCATE_SCALAR(SWeightBuffer);
        c.lw(*SWeightBuffer, Reg::X0, getStateFields().at(getProcess()->getWeight()));
        {
            ALLOCATE_SCALAR(STemp);
            c.mul(*STemp, *idPreReg, *m_StrideReg);
            c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
        }

        return SWeightBuffer;
    }
private:
    //--------------------------------------------------------------------
    // Members
    //--------------------------------------------------------------------
    std::shared_ptr<const Model::EventPropagationProcess> m_Process;
    ScalarRegisterAllocator::RegisterPtr m_StrideReg;
    std::reference_wrapper<const Model::StateFields> m_StateFields;
    std::reference_wrapper<ScalarRegisterAllocator> m_ScalarRegisterAllocator;
    std::reference_wrapper<VectorRegisterAllocator> m_VectorRegisterAllocator;
};

//------------------------------------------------------------------------
// DenseRowGenerator
//------------------------------------------------------------------------
class DenseRowGenerator : public RowGeneratorBase
{
public:
    using RowGeneratorBase::RowGeneratorBase;

    //--------------------------------------------------------------------
    // RowGeneratorBase virtuals
    //--------------------------------------------------------------------
    virtual void generateRow(CodeGenerator &c, ScalarRegisterAllocator::RegisterPtr weightBufferReg) final override
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = getScalarRegisterAllocator();
        auto &vectorRegisterAllocator = getVectorRegisterAllocator();

        ALLOCATE_VECTOR(VWeight);
        ALLOCATE_VECTOR(VTarget1);
        ALLOCATE_VECTOR(VTarget2);
        ALLOCATE_VECTOR(VTargetNew);
        ALLOCATE_SCALAR(STargetBuf);

        // Load target register from state fields
        // **NOTE** no point in caching this as it needs resetting every row
        c.lw(*STargetBuf, Reg::X0, getStateFields().at(getProcess()->getTarget()));

        // Preload first ISyn to avoid stall
        c.vloadv(*VTarget1, *STargetBuf, 0);

        Utils::unrollVectorLoopBody(
            c, scalarRegisterAllocator, getProcess()->getNumTargetNeurons(), 4, *STargetBuf,
            [this, weightBufferReg, STargetBuf, VWeight, VTarget1, VTarget2, VTargetNew]
            (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr maskReg)
            {
                // Load vector of weights
                c.vloadv(*VWeight, *weightBufferReg, r * 64);

                // Load NEXT vector of target to avoid stall
                // **YUCK** in last iteration, while this may not be accessed, it may be out of bounds                  
                c.vloadv(even ? *VTarget2 : *VTarget1, *STargetBuf, (r + 1) * 64);

                // Add weights to ISyn
                auto VTarget = even ? VTarget1 : VTarget2;
                if(maskReg) {
                    c.vadd_s(*VTargetNew, *VTarget, *VWeight);
                    c.vsel(*VTarget, *maskReg, *VTargetNew);
                }
                else {
                    c.vadd_s(*VTarget, *VTarget, *VWeight);
                }

                // Write back target
                c.vstore(*VTarget, *STargetBuf, r * 64);
            },
            [this, weightBufferReg, STargetBuf](CodeGenerator &c, uint32_t numUnrolls)
            {
                // Increment pointers 
                c.addi(*STargetBuf, *STargetBuf, 64 * numUnrolls);
                c.addi(*weightBufferReg, *weightBufferReg, 64 * numUnrolls);
            });
    }   
};

//------------------------------------------------------------------------
// SparseRowGenerator
//------------------------------------------------------------------------
class SparseRowGenerator : public RowGeneratorBase
{
public:
    SparseRowGenerator(CodeGenerator &c, std::shared_ptr<const Model::EventPropagationProcess> process,
                       const Model::StateFields &stateFields,
                       ScalarRegisterAllocator &scalarRegisterAllocator, 
                       VectorRegisterAllocator &vectorRegisterAllocator)
        :   RowGeneratorBase(c, process, stateFields, scalarRegisterAllocator, vectorRegisterAllocator)
    {

        // Allocate register for target address and load address
        m_TargetAddrReg = getScalarRegisterAllocator().getRegister("STargetAddr X");
        c.lw(*m_TargetAddrReg, Reg::X0, stateFields.at(process->getTarget()));
    }

    //--------------------------------------------------------------------
    // RowGeneratorBase virtuals
    //--------------------------------------------------------------------
    virtual void generateRow(CodeGenerator &c, ScalarRegisterAllocator::RegisterPtr weightBufferReg) final override
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = getScalarRegisterAllocator();
        auto &vectorRegisterAllocator = getVectorRegisterAllocator();

        // Loop over postsynaptic neurons
        ALLOCATE_VECTOR(VAccum)
            ALLOCATE_VECTOR(VWeightInd1);
        ALLOCATE_VECTOR(VWeightInd2);
        ALLOCATE_VECTOR(VPostAddr);
        ALLOCATE_VECTOR(VWeight);

        // Preload first weights and indices to avoid stall
        c.vloadv(*VWeightInd1, *weightBufferReg, 0);

        Utils::unrollVectorLoopBody(
            c, scalarRegisterAllocator, getProcess()->getMaxRowLength(), 4, *weightBufferReg,
            [this, weightBufferReg,
            VAccum, VPostAddr, VWeight, VWeightInd1, VWeightInd2]
            (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr)
            {
                // Load NEXT vector of weights and indices
                c.vloadv(even ? *VWeightInd2 : *VWeightInd1, *weightBufferReg, (r + 1) * 64);

                // Extract postsynaptic index and add base address
                auto VWeightInd = even ? VWeightInd1 : VWeightInd2;
                c.vandadd(getProcess()->getNumSparseConnectivityBits(), *VPostAddr, *VWeightInd,
                          *m_TargetAddrReg);

                // Load accumulator
                c.vloadl(*VAccum, *VPostAddr);

                // Extract weight
                c.vsrai(getProcess()->getNumSparseConnectivityBits(), *VWeight, *VWeightInd);

                // Add weights to accumulator loaded in previous iteration
                c.vadd_s(*VAccum, *VAccum, *VWeight);

                // Write back accumulator
                c.vstorel(*VAccum, *VPostAddr);
            },
            [this, weightBufferReg]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                // Increment pointers 
                c.addi(*weightBufferReg, *weightBufferReg, 64 * numUnrolls);
            });
    }

private:
    ScalarRegisterAllocator::RegisterPtr m_TargetAddrReg;
};


//------------------------------------------------------------------------
// DelayedRowGenerator
//------------------------------------------------------------------------
class DelayedRowGenerator : public RowGeneratorBase
{
public:
    DelayedRowGenerator(CodeGenerator &c, std::shared_ptr<const Model::EventPropagationProcess> process,
                        const Model::StateFields &stateFields, VectorRegisterAllocator::RegisterPtr vectorTimeReg,
                        ScalarRegisterAllocator &scalarRegisterAllocator, 
                        VectorRegisterAllocator &vectorRegisterAllocator)
        :   RowGeneratorBase(c, process, stateFields, scalarRegisterAllocator, vectorRegisterAllocator),
        m_DelayStride(2 * process->getTarget()->getNumBufferTimesteps()), 
        m_TargetAddress(stateFields.at(process->getTarget()) + 4), m_VectorTimeReg(vectorTimeReg)
    {
        // Check number of buffer timesteps is P.O.T.
        if(!isPOT(process->getTarget()->getNumBufferTimesteps())) {
            throw std::runtime_error("When used as delayed event propagation targets, variables "
                                     "need to have a power-of-two number of buffer timesteps");
        }
    }

    //--------------------------------------------------------------------
    // RowGeneratorBase virtuals
    //--------------------------------------------------------------------
    virtual void generateRow(CodeGenerator &c, ScalarRegisterAllocator::RegisterPtr weightBufferReg) final override
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = getScalarRegisterAllocator();
        auto &vectorRegisterAllocator = getVectorRegisterAllocator();

        // Loop over postsynaptic neurons
        ALLOCATE_SCALAR(STargetReg);
        ALLOCATE_VECTOR(VAccum)
            ALLOCATE_VECTOR(VWeightInd1);
        ALLOCATE_VECTOR(VWeightInd2);
        ALLOCATE_VECTOR(VPostAddr);
        ALLOCATE_VECTOR(VWeight);

        // Reload target register from scalar register
        // **NOTE** LLM address is offset by 4 bytes in field
        c.lw(*STargetReg, Reg::X0, m_TargetAddress);

        // Preload first weights and indices to avoid stall
        c.vloadv(*VWeightInd1, *weightBufferReg, 0);

        Utils::unrollVectorLoopBody(
            c, scalarRegisterAllocator, getProcess()->getMaxRowLength(), 4, *weightBufferReg,
            [this, weightBufferReg,
            STargetReg, VAccum, VPostAddr, VWeight, VWeightInd1, VWeightInd2]
            (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr)
            {
                // Load NEXT vector of weights and indices
                c.vloadv(even ? *VWeightInd2 : *VWeightInd1, *weightBufferReg, (r + 1) * 64);

                // Add time to delay
                auto VWeightInd = even ? VWeightInd1 : VWeightInd2;
                c.vadd(*VPostAddr, *VWeightInd, *m_VectorTimeReg);

                // Extract delay and add base address
                c.vandadd(getProcess()->getNumDelayBits(), *VPostAddr, *VPostAddr, *STargetReg);

                // Load accumulator
                c.vloadl(*VAccum, *VPostAddr, m_DelayStride * r);

                // Extract weight
                c.vsrai(getProcess()->getNumDelayBits(), *VWeight, *VWeightInd);

                // Add weights to accumulator loaded in previous iteration
                c.vadd_s(*VAccum, *VAccum, *VWeight);

                // Write back accumulator
                c.vstorel(*VAccum, *VPostAddr, m_DelayStride * r);
            },
            [this, STargetReg, weightBufferReg, &vectorRegisterAllocator]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                // Increment pointers 
                c.addi(*weightBufferReg, *weightBufferReg, 64 * numUnrolls);
                c.addi(*STargetReg, *STargetReg, numUnrolls * m_DelayStride);
            });
    }

private:
    //--------------------------------------------------------------------
    // Members
    //--------------------------------------------------------------------
    size_t m_DelayStride;
    uint32_t m_TargetAddress;
    VectorRegisterAllocator::RegisterPtr m_VectorTimeReg;
};
}*/
//----------------------------------------------------------------------------
// FeNN::Backend::NeuronUpdateProcess
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
void NeuronUpdateProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model::Model &model) const
{
    // Superclass
    Model::NeuronUpdateProcess::updateMergeHash(hash, model);

    const auto allState = getAllState();
    for (const auto &s : allState) {

    }
}
//----------------------------------------------------------------------------
void NeuronUpdateProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                      bool, MemSpaceCompatibility &memSpaceCompatibility) const
{
    const auto var = std::find_if(getVariables().cbegin(), getVariables().cend(),
                                  [&state](const auto &v){ return v.second == state; });
    assert(var != neuronUpdateProcess->getVariables().cend());

    // Neuron variables can be located in URAM, LLM or both
    memSpaceCompatibility.bram = false;
    memSpaceCompatibility.dram = false;

    if(!memSpaceCompatibility.uram && !memSpaceCompatibility.llm && !memSpaceCompatibility.uram) {
        throw std::runtime_error("Neuron update process '" + getName()
                                 + "' variable array '" + state->getName()
                                 + "' shared with incompatible processes");
    }
}
//----------------------------------------------------------------------------
/*void NeuronUpdateProcess::generateCode(Assembler::CodeGenerator &codeGenerator,
                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
}*/

//----------------------------------------------------------------------------
// FeNN::Backend::EventPropagationProcess
//----------------------------------------------------------------------------
void EventPropagationProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                          bool useDRAMForWeights, MemSpaceCompatibility &memSpaceCompatibility) const
{
    // If variable is weight, it can only be located in URAM
    if(state == getWeight()) {
        memSpaceCompatibility.llm = false;
        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.uramLLM = false;

        if(useDRAMForWeights) {
            memSpaceCompatibility.uram = false;

            if(!memSpaceCompatibility.dram) {
                throw std::runtime_error("Event propagation process '" + getName() 
                                         + "' weight array '" + getWeight()->getName()
                                         + "' shared with incompatible processes");
            }
        }
        else {
            memSpaceCompatibility.dram = false;

            if(!memSpaceCompatibility.uram) {
                throw std::runtime_error("Event propagation process '" + getName() 
                                         + "' weight array '" + getWeight()->getName()
                                         + "' shared with incompatible processes");
            }
        }
    }
    // Otherwise, if variable's target
    else if(state == getTarget()) {
        // It can't be located in BRAM or DRAM
        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.dram = false;

        // If it's sparse, it must be located in LLM
        if(getNumSparseConnectivityBits() > 0) {
            memSpaceCompatibility.uram = false;
            memSpaceCompatibility.uramLLM = false;

            if(!memSpaceCompatibility.llm) {
                throw std::runtime_error("Event propagation process '" + getName()
                                         + "' target array '" + getTarget()->getName()
                                         + "' shared with incompatible processes");
            }
        }
        // Otherwise, if it's delayed, it must be located in URAMLLM
        else if(getNumDelayBits() > 0) {
            memSpaceCompatibility.uram = false;
            memSpaceCompatibility.llm = false;

            if(!memSpaceCompatibility.uramLLM) {
                throw std::runtime_error("Event propagation process '" + getName()
                                         + "' target array '" + getTarget()->getName()
                                         + "' shared with incompatible processes");
            }
        }
    }
    else {
        assert(false);
    }
}
//----------------------------------------------------------------------------
void EventPropagationProcess::updateMaxDMABufferSize(size_t &maxRowLength) const
{
    maxRowLength = std::max(maxRowLength, getMaxRowLength());
}
//----------------------------------------------------------------------------
/*void EventPropagationProcess::generateCode(Assembler::CodeGenerator &codeGenerator,
                                           Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                           Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
}*/

//----------------------------------------------------------------------------
// FeNN::Backend::RNGInitProcess
//----------------------------------------------------------------------------
void RNGInitProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                 bool, MemSpaceCompatibility &memSpaceCompatibility) const
{
    assert (state == rngInitProcess->getSeed());

    // Seeds can only be stored in URAM
    memSpaceCompatibility.llm = false;
    memSpaceCompatibility.bram = false;
    memSpaceCompatibility.dram = false;
    memSpaceCompatibility.uramLLM = false;

    if(!memSpaceCompatibility.uram) {
        throw std::runtime_error("RNG init process '" + getName() 
                                 + "' seed array '" + getSeed()->getName()
                                 + "' shared with incompatible processes");
    }
}
//----------------------------------------------------------------------------
/*void RNGInitProcess::generateCode(Assembler::CodeGenerator &codeGenerator,
                                  Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                  Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    auto &c = codeGenerator;

    //**TODO** check seed shape

    // Get fields associated with this process
    const auto &stateFields = m_Model.get().getStatefulFields().at(rngInitProcess);

    // Allocate scalar register to hold address of seed buffer
    ALLOCATE_SCALAR(SReg);

    // Generate code to load address of seed
    c.lw(*SReg, Reg::X0, stateFields.at(getSeed()));

    // Load seed into RNG registers
    c.vloadr0(*SReg);
    c.vloadr1(*SReg, 64);
}*/

//----------------------------------------------------------------------------
// FeNN::Backend::MemsetProcess
//----------------------------------------------------------------------------
void MemsetProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                bool, MemSpaceCompatibility &memSpaceCompatibility) const
{
    const auto target = std::get<Model::VariablePtr>(getTarget());
    assert(state == target);

    // **TODO** memset could handle anything
    memSpaceCompatibility.bram = false;
    memSpaceCompatibility.dram = false;

    if(!memSpaceCompatibility.llm && !memSpaceCompatibility.uram && !memSpaceCompatibility.uramLLM) {
        throw std::runtime_error("Memset process '" + getName()
                                 + "' target array '" + target->getName()
                                 + "' shared with incompatible processes");
    }
}
//----------------------------------------------------------------------------
/*void MemsetProcess::generateCode(Assembler::CodeGenerator &codeGenerator,
                                 Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                 Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Get fields associated with this process
    const auto &stateFields = m_Model.get().getStatefulFields().at(this);

    // If target is a variable reference
    if(std::holds_alternative<Model::VariablePtr>(getTarget())) {
        auto target = std::get<Model::VariablePtr>(getTarget());

        // Visit all users of this variable to determine how it has been be implemented
        VariableImplementerVisitor visitor(target, m_Model.get().getStateProcesses().at(target),
                                           m_UseDRAMForWeights);

        // Determine how many vectors we're memsetting
        const size_t numVecsOneTimestep = Utils::ceilDivide(target->getShape().getFlattenedSize(), 32);
        const size_t numVecs = numVecsOneTimestep * target->getNumBufferTimesteps();

        // Allocate register for target address
        ALLOCATE_SCALAR(STargetBuffer);

        if(visitor.isURAMCompatible()) {
            codeGenerator.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
            generateURAMMemset(numVecs, STargetBuffer);
        }
        else if(visitor.isLLMCompatible()) {
            codeGenerator.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
            generateLLMMemset(numVecs, STargetBuffer);
        }
        else if(visitor.isURAMLLMCompatible()) {
            codeGenerator.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
            generateURAMMemset(numVecsOneTimestep, STargetBuffer);

            codeGenerator.lw(*STargetBuffer, Reg::X0, stateFields.at(target) + 4);
            generateLLMMemset(numVecs, STargetBuffer);
        }
        //else if(visitor.isBRAMCompatible()) {
        //}
        else {
            assert(false);
        }
    }
    // Otherwise
    else {
        // Get state object ID
        const int target = std::get<int>(getTarget());

        // Get target field
        // **NOTE** model already checks validity of taret
        const auto &targetField = m_Model.get().getBackendFields().at(target);

        // If target is a LUT i.e. in lane-local memory
        if(static_cast<StateObjectID>(target) == StateObjectID::LUT_EXP) {    
            // Load target address
            ALLOCATE_SCALAR(STargetBuffer);
            c.lw(*STargetBuffer, Reg::X0, std::get<1>(targetField));

            // **YUCK** cast to variable and generate lane-local memory memset
            auto targetVar = std::static_pointer_cast<const Variable>(std::get<0>(targetField));
            generateLLMMemset(ceilDivide(targetVar->getShape().getFlattenedSize(), 32),
                              STargetBuffer);
        }
        else {
            throw std::runtime_error("Memset process '" + getName() 
                                     + "' targets incompatible backend state object "
                                     + std::to_string(target));
        }
    }
}*/
//----------------------------------------------------------------------------
void MemsetProcess::generateLLMMemset(Assembler::CodeGenerator &codeGenerator,
                                      Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                      Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                      size_t numVectors, Assembler::ScalarRegisterAllocator::RegisterPtr targetReg) const
{
    ALLOCATE_VECTOR(VValue);
    ALLOCATE_VECTOR(VLLMAddress);
    ALLOCATE_VECTOR(VNumUnrollBytes);

    // Load value to memset
    codeGenerator.vlui(*VValue, 0);

    // Broadcast address
    codeGenerator.vfill(*VLLMAddress, *targetReg);

    // Generate unrolled loop 
    Assembler::Utils::unrollVectorLoopBody(
        codeGenerator, scalarRegisterAllocator, numVectors * 32, 4, *targetReg,
        [VLLMAddress, VValue]
        (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr)
        {
            c.vstorel(*VValue, *VLLMAddress, r * 2);                  
        },
        [targetReg, VLLMAddress, VNumUnrollBytes, &vectorRegisterAllocator]
        (CodeGenerator &c, uint32_t numUnrolls)
        {
            // Calculate how many bytes we need to advance LLM addresses
            // **TODO** VADDI instruction would save an instruction in this type of situation
            ALLOCATE_VECTOR(VNumUnrollBytes);
            c.vlui(*VNumUnrollBytes, numUnrolls * 2);

            c.vadd(*VLLMAddress, *VLLMAddress, *VNumUnrollBytes);
            c.addi(*targetReg, *targetReg, 64 * numUnrolls);
        });
}
//----------------------------------------------------------------------------
void MemsetProcess::generateURAMMemset(Assembler::CodeGenerator &codeGenerator,
                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                       size_t numVectors, Assembler::ScalarRegisterAllocator::RegisterPtr targetReg) const
{
    ALLOCATE_VECTOR(VValue);

    // Load value to memset and calculate unroll bytes
    // **TODO** parameterise
    codeGenerator.vlui(*VValue, 0);

    // Generate unrolled loop 
    Assembler::Utils::unrollVectorLoopBody(
        codeGenerator, scalarRegisterAllocator, numVectors * 32, 4, *targetReg,
        [targetReg, VValue]
        (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr)
        {
            c.vstore(*VValue, *targetReg, r * 64);                  
        },
        [targetReg]
        (CodeGenerator &c, uint32_t numUnrolls)
        {
            c.addi(*targetReg, *targetReg, 64 * numUnrolls);
        });
}

//----------------------------------------------------------------------------
// FeNN::Backend::BroadcastProcess
//----------------------------------------------------------------------------
void BroadcastProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                   bool, MemSpaceCompatibility &memSpaceCompatibility) const
{
    // If variable is source, it can only be located in BRAM
    if(state == getSource()) {
        memSpaceCompatibility.llm = false;
        memSpaceCompatibility.uram = false;
        memSpaceCompatibility.dram = false;
        memSpaceCompatibility.uramLLM = false;

        if(!memSpaceCompatibility.bram) {
            throw std::runtime_error("Broadcast process '" + getName() 
                                     + "' source array '" + getSource()->getName()
                                     + "' shared with incompatible processes");
        }
    }
    else {
        // Otherwise, if variable's target, it can only located in LLM or URAMLLM
        // **TODO** URAM would also be sensible IN THEORY 
        const auto target = std::get<Model::VariablePtr>(getTarget());
        assert(state == target);

        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.uram = false;
        memSpaceCompatibility.dram = false;

        if(!memSpaceCompatibility.llm && !memSpaceCompatibility.uramLLM) {
            throw std::runtime_error("Broadcast process '" + getName()
                                     + "' target array '" + target->getName()
                                     + "' shared with incompatible processes");
        }
    }
}
//----------------------------------------------------------------------------
/*void BroadcastProcess::generateCode(Assembler::CodeGenerator &codeGenerator,
                                    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                    Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    auto &c = codeGenerator;

    // Register allocation
    ALLOCATE_SCALAR(SDataBuffer);
    ALLOCATE_SCALAR(SDataBufferEnd);
    ALLOCATE_VECTOR(VAddress)
        ALLOCATE_VECTOR(VTwo);

    // Labels
    auto halfWordLoop = createLabel();

    // Get fields associated with this process
    const auto &stateFields = m_Model.get().getStatefulFields().at(broadcastProcess);

    // Load increment
    c.vlui(*VTwo, 2);

    // If target is a variable reference
    {
        ALLOCATE_SCALAR(SLLMAddress)

        if(std::holds_alternative<Model::VariablePtr>(getTarget())) {
            auto target = std::get<Model::VariablePtr>(getTarget());

            c.lw(*SLLMAddress, Reg::X0, stateFields.at(target));
        }
        // Otherwise
        else {
            // Get state object ID
            const int target = std::get<int>(broadcastProcess->getTarget());

            // Get target field
            // **NOTE** model already checks validity of taret
            const auto &targetField = m_Model.get().getBackendFields().at(target);

            // If target is a LUT i.e. in lane-local memory, load target field
            if(static_cast<StateObjectID>(target) == StateObjectID::LUT_EXP) {    
                c.lw(*SLLMAddress, Reg::X0, std::get<1>(targetField));
            }
            else {
                throw std::runtime_error("Broadcast process '" + broadcastProcess->getName() 
                                            + "' targets incompatible backend state object "
                                            + std::to_string(target));
            }
        }

        // Fill vector register with LLM address
        c.vfill(*VAddress, *SLLMAddress);
    }

    // SDataBuffer = scalarPtr
    c.lw(*SDataBuffer, Reg::X0, stateFields.at(getSource()));

    // Load size in bytes and add to scalar buffer to get end
    c.li(*SDataBufferEnd, 2 * getSource()->getShape().getDims().at(0));
    c.add(*SDataBufferEnd, *SDataBufferEnd, *SDataBuffer);

    // Loop over vectors
    c.L(halfWordLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VVector);
        ALLOCATE_SCALAR(SVal);

        // Load halfword
        c.lh(*SVal, *SDataBuffer);

        // Increment pointer
        c.addi(*SDataBuffer, *SDataBuffer, 2);

        // Fill vector register
        c.vfill(*VVector, *SVal);

        // Write to all lane local memories and increment address
        c.vstorel(*VVector, *VAddress);
        c.vadd(*VAddress, *VAddress, *VTwo);

        // Loop
        c.bne(*SDataBuffer, *SDataBufferEnd, halfWordLoop);
    }
}*/
}