#include "fenn/backend/process.h"

// GeNN incl;udes
#include "type.h"
#include "transpiler/errorHandler.h"
#include "transpiler/parser.h"
#include "transpiler/typeChecker.h"

// Common include
#include "common/utils.h"

// Model includes
#include "model/event_container.h"
#include "model/variable.h"

// Backend includes
#include "backend/merged_model.h"

// FeNN common includes
#include "fenn/common/isa.h"

// FeNN Assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/assembler_utils.h"
#include "fenn/assembler/register_allocator.h"

// FeNN backend includes
#include "fenn/backend/environment.h"
#include "fenn/backend/fields.h"
#include "fenn/backend/model.h"
#include "fenn/backend/special_function.h"

using namespace FeNN;
using namespace FeNN::Backend;
using namespace GeNN;

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
Type::ResolvedType createFixedPointType(int numInt, bool saturating)
{
    const int numFrac = 15 - numInt;
    std::ostringstream name;
    name << "s" << numInt << "_" << numFrac;
    if(saturating) {
        name << "_sat_t";
    }
    else {
        name << "_t";
    }
    return Type::ResolvedType::createFixedPointNumeric<int16_t>(name.str(), 50 + numInt, saturating,
                                                                numFrac, &ffi_type_sint16, "");
}

void addStochMulFunctions(EnvironmentLibrary::Library &library) 
{
    // Loop through possible number of integer bits for operand a
    for(int aInt = 0; aInt < 16; aInt++) {
        // Create saturating and non-saturating fixed-point types 
        // **YUCK** these should go in GeNN::Type
        const auto aType = createFixedPointType(aInt, false);
        const auto aTypeSat = createFixedPointType(aInt, true);

        // Loop through possible number of integer bits for operand b
        for(int bInt = 0; bInt < 16; bInt++) {
            // Create saturating and non-saturating fixed-point types
            // **YUCK** these should go in GeNN::Type
            const auto bType = createFixedPointType(bInt, false);
            const auto bTypeSat = createFixedPointType(bInt, true);

            // Determine result type (HIGHEST ranked type)
            const auto &resultType = (aInt > bInt) ? aType : bType;
            const auto &resultTypeSat = (aInt > bInt) ? aTypeSat : bTypeSat;

            // Shift by number of fraction bits of LOWEST ranked type
            const int shift = 15 - ((aInt > bInt) ? bInt : aInt);

            using RegisterPtr = Assembler::VectorRegisterAllocator::RegisterPtr;
            library.emplace(
                "mul_rs",
                std::make_pair(Type::ResolvedType::createFunction(resultType, {aType, bType}),
                               [shift](auto &env, auto &vectorRegisterAllocator, auto &, auto, const auto &args)
                               {
                                   auto result = vectorRegisterAllocator.getRegister();
                                   env.getCodeGenerator().vmul_rs(shift, *result, *std::get<RegisterPtr>(args[0]),
                                                                  *std::get<RegisterPtr>(args[1]));
                                   return std::make_pair(result, true);
                               }));
            library.emplace(
                "mul_rs",
                std::make_pair(Type::ResolvedType::createFunction(resultTypeSat, {aTypeSat, bTypeSat}),
                               [shift](auto &env, auto &vectorRegisterAllocator, auto &, auto, const auto &args)
                               {
                                   auto result = vectorRegisterAllocator.getRegister();
                                   env.getCodeGenerator().vmul_rs(shift, *result, *std::get<RegisterPtr>(args[0]),
                                                                  *std::get<RegisterPtr>(args[1]));
                                   return std::make_pair(result, true);
                               }));
        }
    }
}

bool isExpCalled(const std::vector<Transpiler::Token> &tokens)
{
    // Loop through tokens
    for(auto t = tokens.cbegin(); t != tokens.cend(); t++) {
        // If token is an identifier with correct name
        if(t->type == Transpiler::Token::Type::IDENTIFIER && (t->lexeme == "exp" || t->lexeme.rfind("exp_") == 0)) {
            // If token isn't last in sequence and it's followed by a left bracket
            const auto tNext = std::next(t);
            if(tNext != tokens.cend() && tNext->type == Transpiler::Token::Type::LEFT_PAREN) {
                return true;
            }
        }
    }

    return false;
}

void compileStatements(const std::vector<Transpiler::Token> &tokens, const Type::TypeContext &typeContext,
                       const std::unordered_map<int16_t, Assembler::VectorRegisterAllocator::RegisterPtr> &literalPool,
                       Transpiler::TypeChecker::EnvironmentInternal &typeCheckEnv, Compiler::EnvironmentInternal &compilerEnv,
                       Transpiler::ErrorHandler &errorHandler, Transpiler::TypeChecker::StatementHandler forEachSynapseTypeCheckHandler,
                       Assembler::ScalarRegisterAllocator::RegisterPtr maskRegister, Compiler::RoundingMode roundingMode,
                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, Assembler::VectorRegisterAllocator &vectorRegisterAllocator)
{


    // Parse tokens as block item list (function body)
    auto updateStatements = Transpiler::Parser::parseBlockItemList(tokens, typeContext, errorHandler);
    if(errorHandler.hasError()) {
        throw std::runtime_error("Parse error " + errorHandler.getContext());
    }

    // Resolve types
    auto resolvedTypes = Transpiler::TypeChecker::typeCheck(updateStatements, typeCheckEnv, typeContext, 
                                                            errorHandler, forEachSynapseTypeCheckHandler);
    if(errorHandler.hasError()) {
        throw std::runtime_error("Type check error " + errorHandler.getContext());
    }

    // Compile
    compile(updateStatements, compilerEnv, typeContext, resolvedTypes,
            errorHandler, literalPool, maskRegister, roundingMode,
            scalarRegisterAllocator, vectorRegisterAllocator);
    if(errorHandler.hasError()) {
        throw std::runtime_error("Compiler error " + errorHandler.getContext());
    }
}

//------------------------------------------------------------------------
// NeuronVarBase
//------------------------------------------------------------------------
//! Base class for helper classes used to manage memory access to neuron variables
class NeuronVarBase
{
public:
    //! Generate code to load vector register reg from memory before unrolled loop iteration r
    virtual void genLoad(Compiler::EnvironmentBase &env, Assembler::VectorRegisterAllocator::RegisterPtr reg, uint32_t r) = 0;

    //! Generate code to store vector register reg to memory after unrolled loop iteration r
    virtual void genStore(Compiler::EnvironmentBase &env, Assembler::VectorRegisterAllocator::RegisterPtr reg, uint32_t r) = 0;

    //! Generate code to advance pointer after numUnrolls unrolled 
    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls, 
                              Assembler::VectorRegisterAllocator::RegisterPtr numUnrollBytesReg) = 0;
    virtual Assembler::ScalarRegisterAllocator::RegisterPtr getLoopCountScalarReg() const = 0;
    virtual bool needsNumUnrollBytesReg() const = 0;
};

//------------------------------------------------------------------------
// URAMNeuronVar
//------------------------------------------------------------------------
//! Implementation of NeuronVarBase to handle standard variables located in URAM
class URAMNeuronVar : public NeuronVarBase
{
public:
    URAMNeuronVar(const std::string &varName, std::shared_ptr<const ::Model::Variable> var, 
                  Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                  const ::Model::StateFields &stateFields, std::optional<uint32_t> numTimesteps,
                  Assembler::ScalarRegisterAllocator::RegisterPtr timeReg, 
                  Assembler::ScalarRegisterAllocator::RegisterPtr numVarBytesReg)
    {
        // Allocate scalar register to hold address of variable
        m_ReadBufferReg = scalarRegisterAllocator.getRegister((varName + "Buffer X").c_str());

        // Generate code to load address
        c.lw(*m_ReadBufferReg, *environment.getScalarRegister("_field_base"), targetFieldOffset);

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
    virtual void genLoad(Compiler::EnvironmentBase &env, Assembler::VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vloadv(*reg, *m_ReadBufferReg, 64 * r);
    }

    virtual void genStore(Compiler::EnvironmentBase &env, Assembler::VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vstore(*reg, (m_WriteBufferReg ? *m_WriteBufferReg : *m_ReadBufferReg), 64 * r);
    }

    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                              Assembler::VectorRegisterAllocator::RegisterPtr) final override
    {
        c.addi(*m_ReadBufferReg, *m_ReadBufferReg, 64 * numUnrolls);
        if(m_WriteBufferReg) {
            c.addi(*m_WriteBufferReg, *m_WriteBufferReg, 64 * numUnrolls);
        }
    }

    virtual Assembler::ScalarRegisterAllocator::RegisterPtr getLoopCountScalarReg() const final override
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
    Assembler::ScalarRegisterAllocator::RegisterPtr m_ReadBufferReg;
    Assembler::ScalarRegisterAllocator::RegisterPtr m_WriteBufferReg;
};

//------------------------------------------------------------------------
// LLMNeuronVar
//------------------------------------------------------------------------
//! Implementation of NeuronVarBase to handle variables stored in LLM.
//! Typically these are the output of sparse connectivity
class LLMNeuronVar : public NeuronVarBase
{
public:
    LLMNeuronVar(const std::string &varName, std::shared_ptr<const ::Model::Variable> var, 
                 Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                 Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                 const ::Model::StateFields &stateFields)
    {
        assert(var->getNumBufferTimesteps() == 1);

        // Allocate vector register to hold address of variable
        m_BufferReg = vectorRegisterAllocator.getRegister((varName + "Buffer V").c_str());

        // Generate code to load address
        ALLOCATE_SCALAR(STmp);
        c.lw(*STmp, FeNN::Common::Reg::X0, stateFields.at(var));
        c.vfill(*m_BufferReg, *STmp);
    }

    //--------------------------------------------------------------------
    // NeuronVarBase virtuals
    //--------------------------------------------------------------------
    virtual void genLoad(Compiler::EnvironmentBase &env, Assembler::VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vloadl(*reg, *m_BufferReg, 2 * r);   
    }

    virtual void genStore(Compiler::EnvironmentBase &env, Assembler::VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vstorel(*reg, *m_BufferReg, 2 * r);    
    }

    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t,
                              Assembler::VectorRegisterAllocator::RegisterPtr numUnrollBytesReg) final override
    {
        c.vadd(*m_BufferReg, *m_BufferReg, *numUnrollBytesReg);
    }

    virtual Assembler::ScalarRegisterAllocator::RegisterPtr getLoopCountScalarReg() const final override
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
    Assembler::VectorRegisterAllocator::RegisterPtr m_BufferReg;
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
    URAMLLMNeuronVar(const std::string &varName, std::shared_ptr<const ::Model::Variable> var, 
                     Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                     Assembler::VectorRegisterAllocator &vectorRegisterAllocator, const ::Model::StateFields &stateFields, 
                     Assembler::ScalarRegisterAllocator::RegisterPtr timeReg)
        :   m_DelayStride(2 * var->getNumBufferTimesteps()), m_VectorRegisterAllocator(vectorRegisterAllocator)
    {
        // Check number of buffer timesteps is P.O.T.
        if(!::Common::Utils::isPOT(var->getNumBufferTimesteps())) {
            throw std::runtime_error("When used as delayed event propagation targets, variables "
                                     "need to have a power-of-two number of buffer timesteps");
        }

        // Allocate scalar register to hold address of variable in URAM
        m_URAMBufferReg = scalarRegisterAllocator.getRegister((varName + "Buffer X").c_str());

        // Load URAM address from first word of field
        c.lw(*m_URAMBufferReg, FeNN::Common::Reg::X0, stateFields.at(var));

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
            c.lw(*STmp2, FeNN::Common::Reg::X0, stateFields.at(var) + 4);
            c.add(*STmp2, *STmp2, *STmp);
            c.vfill(*m_LLMBufferReg, *STmp2);
        }
    }

    //--------------------------------------------------------------------
    // NeuronVarBase virtuals
    //--------------------------------------------------------------------
    virtual void genLoad(Compiler::EnvironmentBase &env, Assembler::VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
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

    virtual void genStore(Compiler::EnvironmentBase &env, Assembler::VectorRegisterAllocator::RegisterPtr reg, uint32_t r) final override
    {
        env.getCodeGenerator().vstore(*reg, *m_URAMBufferReg, 64 * r);
    }

    virtual void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                              Compiler::VectorRegisterAllocator::RegisterPtr) final override
    {
        // Calculate how many bytes we need to advance LLM addresses
        // **TODO** VADDI instruction would save an instruction in this type of situation
        auto numUnrollBytesReg = m_VectorRegisterAllocator.get().getRegister("NumUnrollBytes V");
        c.vlui(*numUnrollBytesReg, numUnrolls * m_DelayStride);

        // Increment URAM and LLM pointers
        c.addi(*m_URAMBufferReg, *m_URAMBufferReg, 64 * numUnrolls);
        c.vadd(*m_LLMBufferReg, *m_LLMBufferReg, *numUnrollBytesReg);
    }

    virtual Assembler::ScalarRegisterAllocator::RegisterPtr getLoopCountScalarReg() const final override
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
    Assembler::ScalarRegisterAllocator::RegisterPtr m_URAMBufferReg;
    Assembler::VectorRegisterAllocator::RegisterPtr m_LLMBufferReg;
    std::reference_wrapper<Assembler::VectorRegisterAllocator> m_VectorRegisterAllocator;
};

//------------------------------------------------------------------------
// RowGeneratorBase
//------------------------------------------------------------------------
/*class RowGeneratorBase
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
};*/
}
//----------------------------------------------------------------------------
// FeNN::Backend::TimeDrivenProcessImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
void TimeDrivenProcessImplementation::generateCode(const ::Backend::MergedProcess &mergedProcess,
                                                   const Model &model, Assembler::CodeGenerator &c,
                                                   Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                                   Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Allocate base register
    ALLOCATE_SCALAR(SFieldBase);
    ALLOCATE_SCALAR(SFieldBaseEnd);

    // Create environment with base register
    EnvironmentExternal environment(c);
    environment.add(Type::Uint32.addConst(), "_field_base", SFieldBase);

    // Generate preamble code to setup state shared across merged group
    generateMergedPreambleCode(mergedProcess, model, environment, c, 
                               scalarRegisterAllocator, vectorRegisterAllocator);

    // Generate archetype code
    MergedFields mergedFields;
    Assembler::CodeGenerator archetypeCodeGenerator;
    generateArchetypeCode(mergedProcess, model, environment, mergedFields,
                          archetypeCodeGenerator, scalarRegisterAllocator, 
                          vectorRegisterAllocator);

    // Generate loop over merged groups
    c.li(*SFieldBase, TODO);
    c.addi(*SFieldBaseEnd, *SFieldBase, mergedProcess.getProcesses().size() * mergedFields.getSize());
    auto groupLoop = c.L();
    {
        // Insert generated code to simulate archetype
        c += archetypeCodeGenerator;

        // Advance to next group's fields
        c.addi(*SFieldBase, *SFieldBase, mergedFields.getSize());

        // Keep looping
        c.bne(*SFieldBase, *SFieldBaseEnd, groupLoop);
    }
}
//----------------------------------------------------------------------------
// FeNN::Backend::NeuronUpdateProcess
//----------------------------------------------------------------------------
void NeuronUpdateProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                      MemSpaceCompatibility &memSpaceCompatibility) const
{
    const auto var = std::find_if(getVariables().cbegin(), getVariables().cend(),
                                  [&state](const auto &v){ return v.second.getUnderlying() == state; });
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
void NeuronUpdateProcess::generateMergedPreambleCode(const ::Backend::MergedProcess &mergedProcess,
                                                     const Model &model, EnvironmentExternal &environment, 
                                                     Assembler::CodeGenerator &c,
                                                     Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                     Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    const auto &archetypeLiterals = mergedProcess.getArchetype<NeuronUpdateProcess>()->getLiterals();

    for(size_t i = 0; i < archetypeLiterals.size(); i++) {
        mergedProcess.forEachProcess<NeuronUpdateProcess>(
            [i](const auto &np)
            {
                assert(np->getLiterals().size() == archetypeLiterals.size());
            });
    }

    // Loop through literals shared across merged processes, check FeNN-compliance and load
    std::unordered_map<int16_t, Assembler::VectorRegisterAllocator::RegisterPtr> literalPool;
    for (const auto &l : literalValues.value()) {
        // If literal is an integer, use value directly
        int64_t integerResult;
        const auto &numericType = l.first.getNumeric();
        if(numericType.isIntegral) {
            integerResult = l.second.cast<int64_t>();
        }
        // Otherwise, if it is fixed point, convert
        else if(numericType.fixedPoint) {
            integerResult = std::round(l.second.cast<double>() * (1u << numericType.fixedPoint.value()));
        }
        else {
            throw std::runtime_error("FeNN does not support floating point types");
        }

        // Check integer value can fit within 16-bit signed type
        if(integerResult < std::numeric_limits<int16_t>::min() 
           || integerResult > std::numeric_limits<int16_t>::max())
        {
            throw std::runtime_error("Literal out of range for type '"
                                     + l.first.getName() + "'");
        }

        // Attempt to add literal to pool
        const auto l = literalPool.try_emplace(integerResult, vectorRegisterAllocator.getRegister((std::to_string(integerResult) + " V").c_str()));

        // If successful, generate code to load value into newly allocated vector register
        if(l.second) {
            c.vlui(*l.first->second, l.first->first);
        }
    }
}
//----------------------------------------------------------------------------
void NeuronUpdateProcess::generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                                const Model &model, EnvironmentExternal &environment, 
                                                MergedFields &fields, Assembler::CodeGenerator &c,
                                                Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{  
    // Define type for event-emitting function
    const auto emitEventFunctionType = Type::ResolvedType::createFunction(Type::Void, {});

    // For now, unrollVectorLoopBody requires SOME buffers
    assert(!neuronUpdateProcess->getVariables().empty());

    std::unordered_map<std::shared_ptr<const ::Model::Variable>, std::unique_ptr<NeuronVarBase>> varBuffers;
    {
        // If any variables have buffering, calculate stride in bytes
        // **TODO** make set of non-1 bufferings and pre-multiply time by this
        ALLOCATE_SCALAR(SNumVariableBytes);
        if (std::any_of(getVariables().cbegin(), getVariables().cend(),
                        [](const auto e) { return e.second->getNumBufferTimesteps() != 1; }))
        {
            c.li(*SNumVariableBytes, ::Common::Utils::ceilDivide(neuronUpdateProcess->getNumNeurons(), 32) * 64);
        }

        // Loop through neuron variables
        for(const auto &v : getVariables()) {
            // If variable can be implemented in URAM
            const auto &varMemSpaceCompatibility = model.getStateMemSpaceCompatibility(v.second.getUnderlying());
            if(varMemSpaceCompatibility.uram) {
                varBuffers.emplace(v.second, 
                                    std::make_unique<URAMNeuronVar>(v.first, v.second, c, scalarRegisterAllocator,
                                                                    stateFields, m_NumTimesteps, m_TimeRegister, SNumVariableBytes));
            }
            // Otherwise, if variable is purely implemented in LLM
            else if(varMemSpaceCompatibility.llm){
                varBuffers.emplace(v.second, 
                                    std::make_unique<LLMNeuronVar>(v.first, v.second, c, scalarRegisterAllocator,
                                                                   vectorRegisterAllocator, stateFields));
            }
            else if(varMemSpaceCompatibility.uramLLM) {
                varBuffers.emplace(v.second, 
                                    std::make_unique<URAMLLMNeuronVar>(v.first, v.second, c, scalarRegisterAllocator,
                                                                       vectorRegisterAllocator, stateFields, m_TimeRegister));
            }
            // Otherwise, 
            else {
                assert(false);
            }
        }
    }

    std::unordered_map<std::shared_ptr<const ::Model::EventContainer>, 
                       Assembler::ScalarRegisterAllocator::RegisterPtr> eventBufferRegisters;
    {
        // If any output events have buffering, calculate stride in bytes
        // **TODO** make set of non-1 bufferings and pre-multiply time by this
        ALLOCATE_SCALAR(SNumEventBytes);
        if(std::any_of(getOutputEvents().cbegin(), getOutputEvents().cend(),
                        [](const auto e){ return e.second->getNumBufferTimesteps() != 1; }))
        {
            c.li(*SNumEventBytes, ::Common::Utils::ceilDivide(neuronUpdateProcess->getNumNeurons(), 32) * 4);
        }

        // Loop through neuron event outputs
        for(const auto &e : getOutputEvents()) {
            // Allocate scalar register to hold address of variable
            const auto reg = scalarRegisterAllocator.getRegister((e.first + "Buffer X").c_str());

            // Add register to map
            eventBufferRegisters.try_emplace(e.second.getUnderlying(), reg);

            // Generate code to load address
            c.lw(*reg, FeNN::Common::Reg::X0, stateFields.at(e.second));

            // If there are multiple timesteps, multiply timestep by stride and add to register
            // **TODO** currently this just handles providing entire simulation kernel worth of event data or
            // recording variables for entire simulation - extend to support axonal delays and ring-buffer recording
            const size_t numBufferTimesteps = e.second->getNumBufferTimesteps();
            if (numBufferTimesteps != 1) {
                // Check there is a buffer entry for each timestep with one extra
                // **NOTE** variables get read from timestep and written to timestep + 1 so extra buf
                if(numBufferTimesteps < (m_NumTimesteps.value() + 1)) {
                    throw std::runtime_error("Events need to be buffered for " + std::to_string(m_NumTimesteps.value() + 1u) + " timesteps");
                }

                // reg = stride * (time + 1)
                ALLOCATE_SCALAR(STmp);
                c.addi(*STmp, *m_TimeRegister, 1);
                c.mul(*STmp, *STmp, *SNumEventBytes);
                c.add(*reg, *reg, *STmp);
            }
        }
    }
    // Create code generation environment
    Backend::EnvironmentExternal env(c);

    // Loop through neuron parameters
    /*for(const auto &p : getParameters()) {
        const auto &numericType = p.second->getType().getNumeric();
        int64_t integerResult;
        if(numericType.isIntegral) {
            integerResult = p.second->getValue().cast<int64_t>();
        }
        // Otherwise, if it is fixed point
        else if(numericType.fixedPoint) {
            integerResult = std::round(p.second->getValue().cast<double>() * (1u << numericType.fixedPoint.value()));
        }
        else {
            throw std::runtime_error("FeNN does not support floating point types");
        }

        // Check integer value can fit within 16-bit signed type
        if(integerResult < std::numeric_limits<int16_t>::min() 
            || integerResult > std::numeric_limits<int16_t>::max())
        {
            throw std::runtime_error("Parameter '" + p.first + "' out of range for type '"
                                        +p.second->getType().getName() + "'");
        }

        // If we should keep parameters in registers
        if(m_KeepParamsInRegisters) {
            // Allocate vector register for parameter
            const auto reg = m_VectorRegisterAllocator.get().getRegister((p.first + " V").c_str());

            // Add to environment
            env.add(p.second->getType(), p.first, reg);

            // Generate code to load parameter
            c.vlui(*reg, (uint16_t)integerResult);
        }
        // Otherwise
        else {
            // Generate code to VLUI parameter when required
            // **YUCK** this is very innefficient
            env.add(p.second->getType(), p.first,
                    [&p, integerResult](auto &env, auto &vectorRegisterAllocator, auto&, auto, const auto&)
                    {
                        auto result = vectorRegisterAllocator.getRegister((p.first + " = V").c_str());
                        env.getCodeGenerator().vlui(*result, (uint16_t)integerResult);
                        return std::make_pair(result, true);
                    }); 
        }

    }*/

    env.add(Type::S8_7, "_zero", literalPool.at(0));

    // Build library with fennrand function and stochastic multiplication
    Backend::EnvironmentLibrary::Library functionLibrary;
    functionLibrary.emplace(
        "fennrand",
        std::make_pair(Type::ResolvedType::createFunction(Type::S0_15, {}),
                        [](auto &env, auto &vectorRegisterAllocator, auto&, auto, const auto&)
                        {
                            auto result = vectorRegisterAllocator.getRegister("fennrand = V");
                            env.getCodeGenerator().vrng(*result);
                            return std::make_pair(result, true);
                        }));
    addStochMulFunctions(functionLibrary);
        
    // If exp is called, add special function to environment
    if(isExpCalled(getTokens())) {
        SpecialFunctions::Exp::add(c, scalarRegisterAllocator, vectorRegisterAllocator.get(),
                                    env, functionLibrary, getBackendFieldOffset(StateObjectID::LUT_EXP));
    }
    // Insert environment with this library
    EnvironmentLibrary envLibrary(env, functionLibrary);
        
    // **YUCK** find first scalar-addressed variable to use for loop check
    auto firstScalarAddressRegister = std::find_if(varBuffers.cbegin(), varBuffers.cend(),
                                                    [](const auto &v){ return v.second->getLoopCountScalarReg(); });
    assert(firstScalarAddressRegister != varBuffers.cend());

    // Build vectorised neuron loop
    Assembler::Utils::unrollVectorLoopBody(
        envLibrary.getCodeGenerator(), scalarRegisterAllocator,
        neuronUpdateProcess->getNumNeurons(), 4, *firstScalarAddressRegister->second->getLoopCountScalarReg(),
        [this, &envLibrary, &eventBufferRegisters, &literalPool, &emitEventFunctionType, &varBuffers, &vectorRegisterAllocator]
        (autop&, uint32_t r, bool, auto maskReg)
        {
            EnvironmentExternal unrollEnv(envLibrary);

            // Loop through variables
            for(const auto &v : getVariables()) {
                // Allocate vector register
                const auto reg = vectorRegisterAllocator.getRegister((v.first + " V").c_str());

                // Add to environment
                unrollEnv.add(v.second.getUnderlying()->getType(), v.first, reg);

                // Generate load
                varBuffers.at(v.second.getUnderlying())->genLoad(unrollEnv, reg, r);
            }

            // Loop through neuron event outputs
            for(const auto &e : getOutputEvents()) {
                // Add function to environment to store current mask (inherently which neurons are spiking) to scalar memory
                unrollEnv.add(emitEventFunctionType, e.first, 
                                [e, maskReg, r, &eventBufferRegisters](auto &env, auto&, auto &scalarRegisterAllocator, auto spikeMaskReg, const auto&)
                                {
                                    // If this loop iteration has a mask, AND it with spike mask and store word
                                    if(maskReg) {
                                        ALLOCATE_SCALAR(STmp);
                                        env.getCodeGenerator().and_(*STmp, *spikeMaskReg, *maskReg);
                                        env.getCodeGenerator().sw(*STmp, *eventBufferRegisters.at(e.second), 4 * r);
                                    }
                                    // Otherwise, just store spike mask register
                                    else {
                                        env.getCodeGenerator().sw(*spikeMaskReg, *eventBufferRegisters.at(e.second), 4 * r);
                                    }
                                      
                                    return std::make_pair(RegisterPtr{}, false);
                                });
            }

            // **HACK** if you're unlucky, there can be a RAW hazard between last load and first instruction so nop
            unrollEnv.getCodeGenerator().nop();

            // Compile tokens
            // **NOTE** we don't pass mask register through here - aside from blocking 
            // spike generation, there's no need to mask EVERY assignement etc
            {
                Transpiler::TypeChecker::EnvironmentInternal typeCheckEnv(unrollEnv);
                EnvironmentInternal compilerEnv(unrollEnv);
                Transpiler::ErrorHandler errorHandler("Neuron update process '" + neuronUpdateProcess->getName() + "'");        
                compileStatements(neuronUpdateProcess->getTokens(), {}, literalPool, typeCheckEnv, compilerEnv,
                                    errorHandler, nullptr, nullptr, m_NeuronUpdateRoundingMode, 
                                    scalarRegisterAllocator, vectorRegisterAllocator);
            }

            // Loop through variables
            for(const auto &v : neuronUpdateProcess->getVariables()) {
                // Get register
                const auto reg = std::get<Assembler::VectorRegisterAllocator::RegisterPtr>(unrollEnv.getRegister(v.first));
                    
                // Generate store
                varBuffers.at(v.second)->genStore(unrollEnv, reg, r);
            }
        },
        [this, &eventBufferRegisters, &varBuffers, &vectorRegisterAllocator]
        (auto &c, uint32_t numUnrolls)
        {
            // If any variables have vector addresses i.e. are stored in LLM, load number of bytes to unroll
            Assembler::VectorRegisterAllocator::RegisterPtr numUnrollBytesReg;
            if(std::any_of(varBuffers.cbegin(), varBuffers.cend(),
                            [](const auto &v){ return v.second->needsNumUnrollBytesReg(); }))
            {
                numUnrollBytesReg = vectorRegisterAllocator.getRegister("NumUnrollBytes V");
                c.vlui(*numUnrollBytesReg, numUnrolls * 2);
            }
            
            // Loop through variables and increment buffers
            for(const auto &v : neuronUpdateProcess->getVariables()) {
                varBuffers.at(v.second)->genIncrement(c, numUnrolls, numUnrollBytesReg);
            }

            // Loop through output events and increment buffers
            for(const auto &e : neuronUpdateProcess->getOutputEvents()) {
                const auto bufferReg = eventBufferRegisters.at(e.second);
                c.addi(*bufferReg, *bufferReg, 4 * numUnrolls);
            }
        });
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventPropagationProcess
//----------------------------------------------------------------------------
void EventPropagationProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                          MemSpaceCompatibility &memSpaceCompatibility) const
{
    // If variable is weight, it can  be located in URAM or DRAM
    if(state == getWeight()) {
        memSpaceCompatibility.llm = false;
        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.uramLLM = false;

        if(!memSpaceCompatibility.dram || !memSpaceCompatibility.uram) {
            throw std::runtime_error("Event propagation process '" + getName() 
                                        + "' weight array '" + getWeight()->getName()
                                        + "' shared with incompatible processes");
        }
    }
    // Otherwise, if variable's target
    else if(state == getTarget().getUnderlying()) {
        // It can't be located in BRAM or DRAM
        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.dram = false;

        // If it's sparse, it must be located in LLM
        if(getNumSparseConnectivityBits() > 0) {
            memSpaceCompatibility.uram = false;
            memSpaceCompatibility.uramLLM = false;

            if(!memSpaceCompatibility.llm) {
                throw std::runtime_error("Event propagation process '" + getName()
                                         + "' target array '" + getTarget().getUnderlying()->getName()
                                         + "' shared with incompatible processes");
            }
        }
        // Otherwise, if it's delayed, it must be located in URAMLLM
        else if(getNumDelayBits() > 0) {
            memSpaceCompatibility.uram = false;
            memSpaceCompatibility.llm = false;

            if(!memSpaceCompatibility.uramLLM) {
                throw std::runtime_error("Event propagation process '" + getName()
                                         + "' target array '" + getTarget().getUnderlying()->getName()
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
void EventPropagationProcess::generateCode(const ::Backend::MergedProcess &mergedProcess,
                                           Common::Reg fieldBaseReg, Assembler::CodeGenerator &c,
                                           Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                           Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    /*    // Register allocation
    ALLOCATE_SCALAR(SEventBuffer);
    ALLOCATE_SCALAR(SEventBufferEnd);

    // Generate code to load address of input event buffer
    const auto &processFields = m_Model.get().getStatefulFields();
    c.lw(*SEventBuffer, Reg::X0, processFields.at(processes.front()).at(processes.front()->getInputEvents()));

    {
        // Load immediate with number of words required to represent one timestep of input spikes
        ALLOCATE_SCALAR(STmp);
        c.li(*STmp, ceilDivide(processes.front()->getNumSourceNeurons(), 32) * 4);

        // If there are multiple timesteps, multiply timestep by stride and add to event start pointer
        // **TODO** currently this just handles providing entire simulation kernel worth of event data or
        // recording events for entire simulation - extend to support axonal delays and ring-buffer recording
        const size_t numBufferTimesteps = processes.front()->getInputEvents()->getNumBufferTimesteps();
        if (numBufferTimesteps != 1) {
            // Check there is a buffer entry for each timestep
            // **NOTE** because event propagation processes only READ 
            // events, there's no need for extra buffer entry to write into this timestep
            if(numBufferTimesteps < m_NumTimesteps.value()) {
                throw std::runtime_error("Events need to be buffered for " + std::to_string(m_NumTimesteps.value()) + " timesteps");
            }

            // Multiply time by stride and add to address
            ALLOCATE_SCALAR(STmp2);
            c.mul(*STmp2, *m_TimeRegister, *STmp);
            c.add(*SEventBuffer, *SEventBuffer, *STmp2);
        }

        // Get address of end of input event buffer        
        c.add(*SEventBufferEnd, *STmp, *SEventBuffer);
    }

    // If any processes have delay, load lower 16-bits of time into vector register
    VectorRegisterAllocator::RegisterPtr vectorTimeReg;
    if(std::any_of(processes.cbegin(), processes.cend(), 
        [](const auto &p){ return (p->getNumDelayBits() > 0); }))
    {
        vectorTimeReg = vectorRegisterAllocator.getRegister("VTime V");
        c.vfill(*vectorTimeReg, *m_TimeRegister);
        c.vslli(1, *vectorTimeReg, *vectorTimeReg);
    }

    // Loop through postsynaptic targets and create appropriate row generator objects
    std::vector<std::unique_ptr<RowGeneratorBase>> rowGenerators;
    for(const auto &p : processes) {
        if(p->getNumSparseConnectivityBits() > 0) { 
            rowGenerators.emplace_back(
                std::make_unique<SparseRowGenerator>(c, p, processFields.at(p), scalarRegisterAllocator, 
                                                        vectorRegisterAllocator));
        }
        else if(p->getNumDelayBits() > 0) {
            rowGenerators.emplace_back(
                std::make_unique<DelayedRowGenerator>(c, p, processFields.at(p), vectorTimeReg,
                                                        scalarRegisterAllocator, vectorRegisterAllocator));
        }
        else {
            rowGenerators.emplace_back(
                std::make_unique<DenseRowGenerator>(c, p, processFields.at(p), scalarRegisterAllocator, 
                                                    vectorRegisterAllocator));
        }
    }
    
    // Generate correct loop depending on whether weights are in DRAM or URAM
    if(m_UseDRAMForWeights) {
        generateDRAMWordLoop(rowGenerators, SEventBuffer, SEventBufferEnd);
    }
    else {
        generateURAMWordLoop(rowGenerators, SEventBuffer, SEventBufferEnd);
    }*/
}
/*void generateURAMWordLoop(const std::vector<std::unique_ptr<RowGeneratorBase>> &rowGenerators, 
                              ScalarRegisterAllocator::RegisterPtr eventBufferReg, 
                              ScalarRegisterAllocator::RegisterPtr eventBufferEndReg)
{
    // Make some friendlier-named references
    auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
    auto &c = m_CodeGenerator.get();

    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SEventWord);

    // Labels
    auto wordLoop = createLabel();
    auto bitLoopStart = createLabel();
    auto bitLoopBody = createLabel();
    auto bitLoopEnd = createLabel();
    auto zeroSpikeWord = createLabel();
    auto wordEnd = createLabel();

    // Load some useful constants
    c.li(*SConst1, 1);

    // SWordNStart = 31
    c.li(*SWordNStart, 31);
        
    // Outer word loop
    c.L(wordLoop);
    {
        // Register allocation
        ALLOCATE_SCALAR(SN);

        // SEventWord = *SEventBuffer++
        c.lw(*SEventWord, *eventBufferReg);
        c.addi(*eventBufferReg, *eventBufferReg, 4);

        // If SEventWord == 0, goto bitloop end
        c.beq(*SEventWord, Reg::X0, bitLoopEnd);

        // SN = SWordNStart
        c.mv(*SN, *SWordNStart);

        // Inner bit loop
        c.L(bitLoopStart);
        {
            // Register allocation
            ALLOCATE_SCALAR(SNumLZ);
            ALLOCATE_SCALAR(SNumLZPlusOne);

            // CNumLZ = clz(SEventWord);
            c.clz(*SNumLZ, *SEventWord);

            // If SEventWord == 1  i.e. CNumLZ == 31, goto zeroSpikeWord
            c.beq(*SEventWord, *SConst1, zeroSpikeWord);
            
            // CNumLZPlusOne = CNumLZ + 1
            c.addi(*SNumLZPlusOne, *SNumLZ, 1);

            // SEventWord <<= CNumLZPlusOne
            c.sll(*SEventWord, *SEventWord, *SNumLZPlusOne);

            // SN -= SNumLZ
            c.L(bitLoopBody);
            c.sub(*SN, *SN, *SNumLZ);

            // Loop through row generators and generate code to process rows
            for(auto &r : rowGenerators) {
                auto weightBufferReg = r->loadWeightBuffer(c, SN);
                r->generateRow(c, weightBufferReg);
            }

            // SN --
            c.addi(*SN, *SN, -1);
            
            // If SEventWord != 0, goto bitLoopStart
            c.bne(*SEventWord, Reg::X0, bitLoopStart);
        }

        // SWordNStart += 32
        c.L(bitLoopEnd);
        c.addi(*SWordNStart, *SWordNStart, 32);
            
        // If SEventBuffer != SEventBufferEnd, goto wordloop
        c.bne(*eventBufferReg, *eventBufferEndReg, wordLoop);

        // Goto wordEnd
        //c.j_(wordEnd);
        c.beq(Reg::X0, Reg::X0, wordEnd);
    }

    // Zero event word
    {
        c.L(zeroSpikeWord);
        c.li(*SEventWord, 0);
        //c.j_(bitLoopBody);
        c.beq(Reg::X0, Reg::X0, bitLoopBody);
    }
    
    c.L(wordEnd);
}

void generateDRAMWordLoop(const std::vector<std::unique_ptr<RowGeneratorBase>> &rowGenerators, 
                          ScalarRegisterAllocator::RegisterPtr eventBufferReg, 
                          ScalarRegisterAllocator::RegisterPtr eventBufferEndReg)
{
    // Make some friendlier-named references
    auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
    auto &c = m_CodeGenerator.get();

    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SCurrentEventWord);
    ALLOCATE_SCALAR(SCurrentWordStartID);
    ALLOCATE_SCALAR(SIDPre);
    ALLOCATE_SCALAR(SPrevIDPre);
    ALLOCATE_SCALAR(SRowBufferA);
    ALLOCATE_SCALAR(SRowBufferB);

    // Labels
    auto tail = createLabel();
    auto end = createLabel();

    // Do we have an even number of rows? This dictates whether swap is required
    const bool evenNumRows = ((rowGenerators.size() % 2) == 0);

    // Load row buffer pointers
    c.lw(*SRowBufferA, Reg::X0, getBackendFieldOffset(StateObjectID::ROW_BUFFER_A));
    c.lw(*SRowBufferB, Reg::X0, getBackendFieldOffset(StateObjectID::ROW_BUFFER_B));
        
    // Load some useful constants
    c.li(*SConst1, 1);
    c.li(*SCurrentWordStartID, 31);

    //--------------------------------------------------------------------
    // Prefetch
    //--------------------------------------------------------------------
    {
        // Register allocation
        ALLOCATE_SCALAR(SPrefetchEventWord);
        ALLOCATE_SCALAR(SPrefetchCurrentWordStartID);

        // Labels
        auto prefetchLoop = createLabel();
        auto prefetchWord = createLabel();
        auto prefetchZeroEventWord = createLabel();

        c.L(prefetchLoop);

        // PrefetcEventWord = *eventBufferReg++
        c.lw(*SPrefetchEventWord, *eventBufferReg);
        c.addi(*eventBufferReg, *eventBufferReg, 4);

        c.addi(*SPrefetchCurrentWordStartID, *SCurrentWordStartID, 32);

        // If PrefetchEventWord != 0, goto prefetchWord
        c.bne(*SPrefetchEventWord, Reg::X0, prefetchWord);

        c.mv(*SIDPre, *SCurrentWordStartID);

        // If eventWord == eventWordEnd, goto end
        c.beq(*eventBufferReg, *eventBufferEndReg, end);

        // CurrentWordStartID = PrefetchCurrentWordStartID
        c.mv(*SCurrentWordStartID, *SPrefetchCurrentWordStartID);
            
        // Goto prefetch loop
        // **YUCK** jump
        c.beq(Reg::X0, Reg::X0, prefetchLoop);
            
        c.L(prefetchWord);

        {
            ALLOCATE_SCALAR(SNumLZ);
            
            // Zero current spike word
            c.li(*SCurrentEventWord, 0);
                
            // Count leading zeros in prefetched word
            c.clz(*SNumLZ, *SPrefetchEventWord);
                
            // If the word we prefetched is one, skip shifting and leave current spike word at 0
            c.beq(*SPrefetchEventWord, *SConst1, prefetchZeroEventWord);
 
            // SCurrentEventWord = SPrefetchEventWord << (NumLZ + 1)
            c.addi(*SCurrentEventWord, *SNumLZ, 1);
            c.sll(*SCurrentEventWord, *SPrefetchEventWord, *SCurrentEventWord);

            c.L(prefetchZeroEventWord);

            // PrevIDPre = CurrentWordStartID - NumLZ
            c.sub(*SPrevIDPre, *SCurrentWordStartID, *SNumLZ);
        }
            
        {
            // Calculate weight buffer
            auto prefetchWeightBuffer = rowGenerators[0]->loadWeightBuffer(c, SPrevIDPre);

            // Start DMA write into RowBufferA
            AssemblerUtils::generateDMAStartWrite(c, *SRowBufferA, *prefetchWeightBuffer, 
                                                    *rowGenerators[0]->getStrideReg());
        }

        // IdPre = PrevIDPre -1
        c.addi(*SIDPre, *SPrevIDPre, -1);

        // WordStartID = PrefetchWordStartID
        c.mv(*SCurrentWordStartID, *SPrefetchCurrentWordStartID);

        // Goto tail
        // **YUCK** jump
        c.beq(Reg::X0, Reg::X0, tail);
    }

    //--------------------------------------------------------------------
    // Iterate
    //--------------------------------------------------------------------
    {
        // Labels
        auto zeroEventWord = createLabel();
        auto wordLoop = createLabel();
        auto processBit = createLabel();
        auto nextEventWord = createLabel();

        c.L(zeroEventWord);
            
        // Zero event work and goto processBit
        // **YUCK** jump
        c.li(*SCurrentEventWord, 0);
        c.beq(Reg::X0, Reg::X0, processBit);
            
        c.L(wordLoop);

        // Loop through all but last row
        for(size_t r = 0; r < (rowGenerators.size() - 1); r++) {
            const bool evenRow = ((r % 2) == 0);
            {
                // Start DMA from weight buffer into correct buffer
                auto fetchRowBuffer = evenRow ? SRowBufferB : SRowBufferA;
                auto fetchWeightBuffer = rowGenerators[r + 1]->loadWeightBuffer(c, SPrevIDPre);
                AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);
                AssemblerUtils::generateDMAStartWrite(c, *fetchRowBuffer, *fetchWeightBuffer, 
                                                        *rowGenerators[r + 1]->getStrideReg());
            }

            // Generate code to process row in other buffer
            {
                ALLOCATE_SCALAR(SRowBuffer);
                c.mv(*SRowBuffer, evenRow ? *SRowBufferA : *SRowBufferB);
                rowGenerators[r]->generateRow(c, SRowBuffer);
            }
        }    

        {
            ALLOCATE_SCALAR(SNumLZ);

            // If CurrentSpikeWord == 1 i.e. NumLZ == 31, goto zeroEventWord
            c.clz(*SNumLZ, *SCurrentEventWord);
            c.beq(*SCurrentEventWord, *SConst1, zeroEventWord);

            // CurrentEventWord = CurrentEventWord << (NumLZ + 1)
            {
                ALLOCATE_SCALAR(STmp);
                c.addi(*STmp, *SNumLZ, 1);
                c.sll(*SCurrentEventWord, *SCurrentEventWord, *STmp);
            }
                
            c.L(processBit);

            // IDPre -= NumLZ
            c.sub(*SIDPre, *SIDPre, *SNumLZ);
        }

        {
            // Start DMA write into correct buffer
            auto fetchRowBuffer = evenNumRows ? SRowBufferA : SRowBufferB;
            auto fetchWeightBuffer = rowGenerators[0]->loadWeightBuffer(c, SIDPre);
            AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);
            AssemblerUtils::generateDMAStartWrite(c, *fetchRowBuffer, *fetchWeightBuffer, 
                                                    *rowGenerators[0]->getStrideReg());
        }

        // Generate code to process row in other buffer
        {
            ALLOCATE_SCALAR(SRowBuffer);
            c.mv(*SRowBuffer, evenNumRows ? *SRowBufferB : *SRowBufferA);
            rowGenerators.back()->generateRow(c, SRowBuffer);
        }

        // If we have an odd number of rows, swap buffers
        if(!evenNumRows) {
            ALLOCATE_SCALAR(STmp);
            c.mv(*STmp, *SRowBufferA);
            c.mv(*SRowBufferA, *SRowBufferB);
            c.mv(*SRowBufferB, *STmp);
        }
            
        // PrevIDPre = IDPre
        c.mv(*SPrevIDPre, *SIDPre);

        {
            // Register allocation
            ALLOCATE_SCALAR(SPrevWordStartID);
            ALLOCATE_SCALAR(SNextEventBuffer);

            c.mv(*SPrevWordStartID, *SCurrentWordStartID);
            c.addi(*SCurrentWordStartID, *SIDPre, -1);

            c.mv(*SNextEventBuffer, *eventBufferReg);
                
            c.L(nextEventWord);

            c.mv(*SIDPre, *SCurrentWordStartID);
            c.mv(*SCurrentWordStartID, *SPrevWordStartID);
            c.mv(*eventBufferReg, *SNextEventBuffer);

            c.L(tail);

            // If CurrentEventWord != 0, goto wordLoop
            c.bne(*SCurrentEventWord, Reg::X0, wordLoop);
                
            c.addi(*SNextEventBuffer, *eventBufferReg, 4);
            c.lw(*SCurrentEventWord, *eventBufferReg);
            c.addi(*SPrevWordStartID, *SCurrentWordStartID, 32);
                
            // If nextEventWord < eventWordEnd i.e. there is a next goto nextEventWord
            c.bgeu(*eventBufferEndReg, *SNextEventBuffer, nextEventWord);
        }
            
        // Loop through all but last row
        for(size_t r = 0; r < (rowGenerators.size() - 1); r++) {
            const bool evenRow = ((r % 2) == 0);
            {
                // Start DMA write into correct buffer
                auto fetchRowBuffer = evenRow ? SRowBufferB : SRowBufferA;
                auto fetchWeightBuffer = rowGenerators[r + 1]->loadWeightBuffer(c, SPrevIDPre);

                // Start DMA write into RowBufferA
                AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);
                AssemblerUtils::generateDMAStartWrite(c, *fetchRowBuffer, *fetchWeightBuffer, 
                                                    *rowGenerators[r + 1]->getStrideReg());
            }

            // Generate code to process row in other buffer
            {
                ALLOCATE_SCALAR(SRowBuffer);
                c.mv(*SRowBuffer, evenRow ? *SRowBufferA : *SRowBufferB);
                rowGenerators[r]->generateRow(c, SRowBuffer);
            }

        }

        // Generate code to process final row
        AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);
        rowGenerators.back()->generateRow(c, evenNumRows ? SRowBufferB : SRowBufferA);

    }

    c.L(end);
}
*/

//----------------------------------------------------------------------------
// FeNN::Backend::RNGInitProcess
//----------------------------------------------------------------------------
void RNGInitProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                 MemSpaceCompatibility &memSpaceCompatibility) const
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
void RNGInitProcess::generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                           EnvironmentExternal &environment, 
                                           MergedFields &fields, Assembler::CodeGenerator &c,
                                           Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                           Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Allocate fields
    const uint32_t seedFieldOffset = fields.addPointerField<RNGInitProcess>(
        mergedProcess, [](const auto &p){ return p->getSeed(); });

    // Allocate scalar register to hold address of seed buffer
    ALLOCATE_SCALAR(SReg);

    // Generate code to load address of seed
    c.lw(*SReg, *environment.getScalarRegister("_field_base"), seedFieldOffset);

    // Load seed into RNG registers
    c.vloadr0(*SReg);
    c.vloadr1(*SReg, 64);
}

//----------------------------------------------------------------------------
// FeNN::Backend::MemsetProcess
//----------------------------------------------------------------------------
void MemsetProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                MemSpaceCompatibility &memSpaceCompatibility) const
{
    assert(state == getTarget().getUnderlying());

    // **TODO** memset could handle anything
    memSpaceCompatibility.bram = false;
    memSpaceCompatibility.dram = false;

    if(!memSpaceCompatibility.llm && !memSpaceCompatibility.uram && !memSpaceCompatibility.uramLLM) {
        throw std::runtime_error("Memset process '" + getName()
                                 + "' target array '" + getTarget().getUnderlying()->getName()
                                 + "' shared with incompatible processes");
    }
}
//----------------------------------------------------------------------------
void MemsetProcess::generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                          const Model &model, EnvironmentExternal &environment, 
                                          MergedFields &fields, Assembler::CodeGenerator &c,
                                          Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                          Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Allocate fields
    const uint32_t targetFieldOffset = fields.addPointerField<MemsetProcess>(
        mergedProcess, [](const auto &p){ return p->getTarget().getUnderlying(); });
    
    // **TODO** should get value for all cores AND all groups
    const uint32_t numVecsFieldOffset = fields.addValueField<MemsetProcess>(
        mergedProcess, [](const auto &p)
        { 
            return ::Utils::ceilDivide(p->getTarget().getShape().getFlattenedSize(), 32);
        });
    // Determine how many vectors we're memsetting
    //const size_t numVecsOneTimestep = Utils::ceilDivide(target->getShape().getFlattenedSize(), 32);
    //const size_t numVecs = numVecsOneTimestep * target->getNumBufferTimesteps();

    // Allocate register for target address
    ALLOCATE_SCALAR(STargetBuffer);

    const auto &targetMemSpaceCompatibility = model.getStateMemSpaceCompatibility(getTarget().getUnderlying());
    if(targetMemSpaceCompatibility.uram) {
        codeGenerator.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
        generateURAMMemset(numVecs, STargetBuffer);
    }
    else if(targetMemSpaceCompatibility.llm) {
        codeGenerator.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
        generateLLMMemset(numVecs, STargetBuffer);
    }
    else if(targetMemSpaceCompatibility.uramLLM) {
        codeGenerator.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
        generateURAMMemset(numVecsOneTimestep, STargetBuffer);

        codeGenerator.lw(*STargetBuffer, Reg::X0, stateFields.at(target) + 4);
        generateLLMMemset(numVecs, STargetBuffer);
    }
    else {
        throw std::runtime_error("Memspace process incompatible with target memory space");
    }
}
//----------------------------------------------------------------------------
void MemsetProcess::generateLLMMemset(Assembler::CodeGenerator &c,
                                      Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                      Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                      size_t numVectors, Assembler::ScalarRegisterAllocator::RegisterPtr targetReg) const
{
    ALLOCATE_VECTOR(VValue);
    ALLOCATE_VECTOR(VLLMAddress);
    ALLOCATE_VECTOR(VNumUnrollBytes);

    // Load value to memset
    c.vlui(*VValue, 0);

    // Broadcast address
    c.vfill(*VLLMAddress, *targetReg);

    // Generate unrolled loop 
    Assembler::Utils::unrollVectorLoopBody(
        c, scalarRegisterAllocator, numVectors * 32, 4, *targetReg,
        [VLLMAddress, VValue]
        (auto &c, uint32_t r, uint32_t, auto)
        {
            c.vstorel(*VValue, *VLLMAddress, r * 2);                  
        },
        [targetReg, VLLMAddress, VNumUnrollBytes, &vectorRegisterAllocator]
        (auto &c, uint32_t numUnrolls)
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
void MemsetProcess::generateURAMMemset(Assembler::CodeGenerator &c,
                                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                       Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                       size_t numVectors, Assembler::ScalarRegisterAllocator::RegisterPtr targetReg) const
{
    ALLOCATE_VECTOR(VValue);

    // Load value to memset and calculate unroll bytes
    // **TODO** parameterise
    c.vlui(*VValue, 0);

    // Generate unrolled loop 
    Assembler::Utils::unrollVectorLoopBody(
        c, scalarRegisterAllocator, numVectors * 32, 4, *targetReg,
        [targetReg, VValue]
        (auto &c, uint32_t r, uint32_t, auto)
        {
            c.vstore(*VValue, *targetReg, r * 64);                  
        },
        [targetReg]
        (auto &c, uint32_t numUnrolls)
        {
            c.addi(*targetReg, *targetReg, 64 * numUnrolls);
        });
}

//----------------------------------------------------------------------------
// FeNN::Backend::BroadcastProcess
//----------------------------------------------------------------------------
void BroadcastProcess::updateMemSpaceCompatibility(std::shared_ptr<const ::Model::State> state, 
                                                   MemSpaceCompatibility &memSpaceCompatibility) const
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
        assert(state == getTarget());

        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.uram = false;
        memSpaceCompatibility.dram = false;

        if(!memSpaceCompatibility.llm && !memSpaceCompatibility.uramLLM) {
            throw std::runtime_error("Broadcast process '" + getName()
                                     + "' target array '" + getTarget()->getName()
                                     + "' shared with incompatible processes");
        }
    }
}
//----------------------------------------------------------------------------
void BroadcastProcess::generateMergedPreambleCode(const ::Backend::MergedProcess &mergedProcess,
                                                  const Model&, EnvironmentExternal &environment, 
                                                  Assembler::CodeGenerator &c,
                                                  Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                  Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    ALLOCATE_VECTOR(VTwo);
    c.vlui(*VTwo, 2);
    environment.add(Type::Uint16.addConst(), "_v_two", VTwo);
}
//----------------------------------------------------------------------------
void BroadcastProcess::generateArchetypeCode(const ::Backend::MergedProcess &mergedProcess,
                                             const Model&, EnvironmentExternal &environment, 
                                             MergedFields &fields, Assembler::CodeGenerator &c,
                                             Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                             Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Allocate fields
    const uint32_t sourceFieldOffset = fields.addPointerField<BroadcastProcess>(
        mergedProcess, [](const auto &p){ return p->getSource(); });
    const uint32_t targetFieldOffset = fields.addPointerField<BroadcastProcess>(
        mergedProcess, [](const auto &p){ return p->getTarget(); });

    // **TODO** should get value for all cores AND all groups
    const uint32_t numBytesFieldOffset = fields.addValueField<BroadcastProcess>(
        mergedProcess, [](const auto &p){ return 2 * p->getSource()->getShape().getDims()[0]; });

    // Register allocation
    ALLOCATE_SCALAR(SDataBuffer);
    ALLOCATE_SCALAR(SDataBufferEnd);
    ALLOCATE_VECTOR(VAddress)

    // If target is a variable reference
    {
        ALLOCATE_SCALAR(SLLMAddress)
        c.lw(*SLLMAddress, *environment.getScalarRegister("_field_base"), targetFieldOffset);

        // Fill vector register with LLM address
        c.vfill(*VAddress, *SLLMAddress);
    }

    // SDataBuffer = scalarPtr
    c.lw(*SDataBuffer, *environment.getScalarRegister("_field_base"), sourceFieldOffset);

    // Load size in bytes and add to scalar buffer to get end
    c.lw(*SDataBufferEnd, *environment.getScalarRegister("_field_base"), numBytesFieldOffset);
    c.add(*SDataBufferEnd, *SDataBufferEnd, *SDataBuffer);

    // Loop over vectors
    auto halfWordLoop = c.L();
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
        c.vadd(*VAddress, *VAddress, *environment.getVectorRegister("_v_two"));

        // Loop
        c.bne(*SDataBuffer, *SDataBufferEnd, halfWordLoop);
    }
}
}