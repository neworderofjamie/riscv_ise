#include "fenn/backend/backend_fenn.h"

// Standard C++ include
#include <functional>
#include <numeric>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Standard C includes
#include <cstdint>

// PLOG includes
#include <plog/Log.h>

// FastFloat includes
#include <fast_float/fast_float.h>

// GeNN includes
#include "type.h"
#include "gennUtils.h"

// GeNN transpiler includes
#include "transpiler/errorHandler.h"
#include "transpiler/parser.h"
#include "transpiler/scanner.h"
#include "transpiler/typeChecker.h"

// RISC-V assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/assembler_utils.h"
#include "fenn/assembler/register_allocator.h"

// Compiler includes
#include "fenn/compiler/compiler.h"
#include "fenn/compiler/event_container.h"
#include "fenn/compiler/parameter.h"
#include "fenn/compiler/process.h"
#include "fenn/compiler/process_group.h"
#include "fenn/compiler/variable.h"

// Backend includes
#include "fenn/backend/environment.h"
#include "fenn/backend/memory_allocator.h"
#include "fenn/backend/special_function.h"

using namespace GeNN;
using namespace GeNN::Transpiler;

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
enum class StateObjectID : int
{
    ROW_BUFFER_A,
    ROW_BUFFER_B,

    LUT_EXP,
};

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

            library.emplace(
                "mul_rs",
                std::make_pair(Type::ResolvedType::createFunction(resultType, {aType, bType}),
                               [shift](auto &env, auto &vectorRegisterAllocator, auto &, auto, const auto &args)
                               {
                                   auto result = vectorRegisterAllocator.getRegister();
                                   env.getCodeGenerator().vmul_rs(shift, *result, *std::get<VectorRegisterAllocator::RegisterPtr>(args[0]),
                                                                  *std::get<VectorRegisterAllocator::RegisterPtr>(args[1]));
                                   return std::make_pair(result, true);
                               }));
            library.emplace(
                "mul_rs",
                std::make_pair(Type::ResolvedType::createFunction(resultTypeSat, {aTypeSat, bTypeSat}),
                               [shift](auto &env, auto &vectorRegisterAllocator, auto &, auto, const auto &args)
                               {
                                   auto result = vectorRegisterAllocator.getRegister();
                                   env.getCodeGenerator().vmul_rs(shift, *result, *std::get<VectorRegisterAllocator::RegisterPtr>(args[0]),
                                                                  *std::get<VectorRegisterAllocator::RegisterPtr>(args[1]));
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

void updateLiteralPool(const std::vector<Token> &tokens, VectorRegisterAllocator &vectorRegisterAllocator, 
                       std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool)
{
    // Loop through tokens
    for(const auto &t : tokens) {
        // If token is a number
        if(t.type == Token::Type::NUMBER) {
            // Get start and end of lexeme
            const auto lexeme = t.lexeme;
            const char *lexemeBegin = lexeme.c_str();
            const char *lexemeEnd = lexemeBegin + lexeme.size();
            
            // Get it's type (scalar if not specified)
            int64_t integerResult;
            const auto &numericType = t.numberType.value().getNumeric();
            if(numericType.isIntegral) {
                if(numericType.isSigned) {
                    int result;
                    auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                    assert(answer.ec == std::errc());
                    integerResult = result;
                }
                else {
                    unsigned int result;
                    auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                    assert(answer.ec == std::errc());
                    integerResult = result;
                }
            }
            // Otherwise, if it is fixed point
            else if(numericType.fixedPoint) {
                float result;
                auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                assert(answer.ec == std::errc());
                integerResult = std::round(result * (1u << numericType.fixedPoint.value()));
            }
            else {
                throw std::runtime_error("FeNN does not support floating point types");
            }

            // Check integer value can fit within 16-bit signed type
            if(integerResult < std::numeric_limits<int16_t>::min() 
               || integerResult > std::numeric_limits<int16_t>::max())
            {
                throw std::runtime_error("Literal '" + lexeme + "' out of range for type '"
                                         + t.numberType.value().getName() + "'");
            }
            // Check integer is in range and insert in pool
            assert(integerResult >= std::numeric_limits<int16_t>::min());
            assert(integerResult <= std::numeric_limits<int16_t>::max());

            // If this literal isn't already in the pool, add it
            const auto l = literalPool.find(integerResult);
            if(l == literalPool.cend()) {
                literalPool.emplace(integerResult, vectorRegisterAllocator.getRegister((std::to_string(integerResult) + " V").c_str()));
            }
        }
    }

    // Always add zero to literal pool
    // **THINK** why not hardcode V0 
    literalPool.try_emplace(0, vectorRegisterAllocator.getRegister("0 V"));
}

bool arePerformanceCountersRequired(const std::vector<std::shared_ptr<const ProcessGroup>> &processGroups)
{
    return std::any_of(processGroups.cbegin(), processGroups.cend(),
                       [](const auto &p){ return p->getPerformanceCounter(); });
}
void compileStatements(const std::vector<Token> &tokens, const Type::TypeContext &typeContext,
                       const std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool,
                       TypeChecker::EnvironmentInternal &typeCheckEnv, EnvironmentInternal &compilerEnv,
                       ErrorHandler &errorHandler, Transpiler::TypeChecker::StatementHandler forEachSynapseTypeCheckHandler,
                       ScalarRegisterAllocator::RegisterPtr maskRegister, RoundingMode roundingMode,
                       ScalarRegisterAllocator &scalarRegisterAllocator, VectorRegisterAllocator &vectorRegisterAllocator)
{
    

    // Parse tokens as block item list (function body)
    auto updateStatements = Parser::parseBlockItemList(tokens, typeContext, errorHandler);
    if(errorHandler.hasError()) {
        throw std::runtime_error("Parse error " + errorHandler.getContext());
    }

    // Resolve types
    auto resolvedTypes = TypeChecker::typeCheck(updateStatements, typeCheckEnv, typeContext, 
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

//----------------------------------------------------------------------------
// VariableImplementerVisitor
//----------------------------------------------------------------------------
class VariableImplementerVisitor : public ModelComponentVisitor
{
public:
    VariableImplementerVisitor(std::shared_ptr<const Variable> variable, const Model::StateProcesses::mapped_type &processes,
                               bool useDRAMForWeights)
    :   m_Variable(variable), m_UseDRAMForWeights(useDRAMForWeights), m_LLMCompatible(true), 
        m_URAMCompatible(true), m_BRAMCompatible(true), m_DRAMCompatible(true), m_URAMLLMCompatible(true)
    {
        // Visit all processes
        for(auto p : processes) {
            p->accept(*this);
        }
    }

    bool isLLMCompatible() const{ return m_LLMCompatible; }
    bool isURAMCompatible() const{ return m_URAMCompatible; }
    bool isBRAMCompatible() const{ return m_BRAMCompatible; }
    bool isDRAMCompatible() const{ return m_DRAMCompatible; }
    bool isURAMLLMCompatible() const{ return m_URAMLLMCompatible; }

private:
    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const NeuronUpdateProcess> neuronUpdateProcess)
    {
        const auto var = std::find_if(neuronUpdateProcess->getVariables().cbegin(), neuronUpdateProcess->getVariables().cend(),
                                      [this](const auto &v){ return v.second == m_Variable; });
        assert(var != neuronUpdateProcess->getVariables().cend());
     
        // Neuron variables can be located in URAM, LLM or both
        m_BRAMCompatible = false;
        m_DRAMCompatible = false;

        if(!m_URAMCompatible && !m_LLMCompatible && !m_URAMLLMCompatible) {
            throw std::runtime_error("Neuron update process '" + neuronUpdateProcess->getName()
                                    + "' variable array '" + m_Variable->getName()
                                    + "' shared with incompatible processes");
        }
    }

    virtual void visit(std::shared_ptr<const EventPropagationProcess> eventPropagationProcess)
    {
        // If variable is weight, it can only be located in URAM
        if(m_Variable == eventPropagationProcess->getWeight()) {
            m_LLMCompatible = false;
            m_BRAMCompatible = false;
            m_URAMLLMCompatible = false;

            if(m_UseDRAMForWeights) {
                m_URAMCompatible = false;

                if(!m_DRAMCompatible) {
                    throw std::runtime_error("Event propagation process '" + eventPropagationProcess->getName() 
                                            + "' weight array '" + eventPropagationProcess->getWeight()->getName()
                                            + "' shared with incompatible processes");
                }
            }
            else {
                m_DRAMCompatible = false;

                if(!m_URAMCompatible) {
                    throw std::runtime_error("Event propagation process '" + eventPropagationProcess->getName() 
                                            + "' weight array '" + eventPropagationProcess->getWeight()->getName()
                                            + "' shared with incompatible processes");
                }
            }
        }
        // Otherwise, if variable's target
        else if(m_Variable == eventPropagationProcess->getTarget()) {
            // It can't be located in BRAM or DRAM
            m_BRAMCompatible = false;
            m_DRAMCompatible = false;

            // If it's sparse, it must be located in LLM
            if(eventPropagationProcess->getNumSparseConnectivityBits() > 0) {
                m_URAMCompatible = false;
                m_URAMLLMCompatible = false;

                if(!m_LLMCompatible) {
                    throw std::runtime_error("Event propagation process '" + eventPropagationProcess->getName()
                                            + "' target array '" + eventPropagationProcess->getTarget()->getName()
                                            + "' shared with incompatible processes");
                }
            }
            // Otherwise, if it's delayed, it must be located in URAMLLM
            else if(eventPropagationProcess->getNumDelayBits() > 0) {
                m_URAMCompatible = false;
                m_LLMCompatible = false;

                if(!m_URAMLLMCompatible) {
                    throw std::runtime_error("Event propagation process '" + eventPropagationProcess->getName()
                                            + "' target array '" + eventPropagationProcess->getTarget()->getName()
                                            + "' shared with incompatible processes");
                }
            }
        }
        else {
            assert(false);
        }
    }

    virtual void visit(std::shared_ptr<const RNGInitProcess> rngInitProcess)
    {
        assert (m_Variable == rngInitProcess->getSeed());
        
        // Seeds can only be stored in URAM
        m_LLMCompatible = false;
        m_BRAMCompatible = false;
        m_DRAMCompatible = false;
        m_URAMLLMCompatible = false;

        if(!m_URAMCompatible) {
            throw std::runtime_error("RNG init process '" + rngInitProcess->getName() 
                                     + "' seed array '" + rngInitProcess->getSeed()->getName()
                                     + "' shared with incompatible processes");
        }
    }

    virtual void visit(std::shared_ptr<const BroadcastProcess> broadcastProcess)
    {
        // If variable is source, it can only be located in BRAM
        if(m_Variable == broadcastProcess->getSource()) {
            m_LLMCompatible = false;
            m_URAMCompatible = false;
            m_DRAMCompatible = false;
            m_URAMLLMCompatible = false;

            if(!m_BRAMCompatible) {
                throw std::runtime_error("Broadcast process '" + broadcastProcess->getName() 
                                        + "' source array '" + broadcastProcess->getSource()->getName()
                                        + "' shared with incompatible processes");
            }
        }
        else {
            // Otherwise, if variable's target, it can only located in LLM or URAMLLM
            // **TODO** URAM would also be sensible IN THEORY 
            const auto target = std::get<VariablePtr>(broadcastProcess->getTarget());
            assert(m_Variable == target);
            
            m_BRAMCompatible = false;
            m_URAMCompatible = false;
            m_DRAMCompatible = false;

            if(!m_LLMCompatible && !m_URAMLLMCompatible) {
                throw std::runtime_error("Broadcast process '" + broadcastProcess->getName()
                                        + "' target array '" + target->getName()
                                        + "' shared with incompatible processes");
            }
        }
    }

    virtual void visit(std::shared_ptr<const MemsetProcess> memsetProcess)
    {
        const auto target = std::get<VariablePtr>(memsetProcess->getTarget());
        assert(m_Variable == target);

        // **TODO** memset could handle anything
        m_BRAMCompatible = false;
        m_DRAMCompatible = false;

        if(!m_LLMCompatible && !m_URAMCompatible && !m_URAMLLMCompatible) {
            throw std::runtime_error("Memset process '" + memsetProcess->getName()
                                     + "' target array '" + target->getName()
                                     + "' shared with incompatible processes");
        }
    }


    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const Variable> m_Variable;
    bool m_UseDRAMForWeights;
    bool m_LLMCompatible;
    bool m_URAMCompatible;
    bool m_BRAMCompatible;
    bool m_DRAMCompatible;
    bool m_URAMLLMCompatible;
};

//----------------------------------------------------------------------------
// MaxRowLengthVisitor
//----------------------------------------------------------------------------
class MaxRowLengthVisitor : public ModelComponentVisitor
{
public:
    MaxRowLengthVisitor(const Model &model)
    :   m_MaxRowLength(0)
    {
        // Loop through process groups in model and visit all processes
        for(const auto &g : model.getProcessGroups()) {
            for(const auto &p : g->getProcesses()) {
                p->accept(*this);
            }
        }
    }

    size_t getMaxRowLength() const{ return m_MaxRowLength; }

    
private:
    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const EventPropagationProcess> eventPropagationProcess)
    {
        m_MaxRowLength = std::max(m_MaxRowLength, eventPropagationProcess->getMaxRowLength());
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    size_t m_MaxRowLength;
};

//----------------------------------------------------------------------------
// LUTVisitor
//----------------------------------------------------------------------------
class LUTVisitor : public ModelComponentVisitor
{
public:
    LUTVisitor(const Model &model)
    {
        // Loop through process groups in model and visit all processes
        for(const auto &g : model.getProcessGroups()) {
            for(const auto &p : g->getProcesses()) {
                p->accept(*this);
            }
        }
    }

    const auto &getLUTObjectIDs() const{ return m_LUTObjectIDs; }

private:
    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const NeuronUpdateProcess> neuronUpdateProcess)
    {
        // If exponential function is referenced in tokens
        if(isExpCalled(neuronUpdateProcess->getTokens())) {
            m_LUTObjectIDs.insert(StateObjectID::LUT_EXP);
        }
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_set<StateObjectID> m_LUTObjectIDs;
};

class PerformanceCounterScope
{
public:
    PerformanceCounterScope(std::shared_ptr<const ProcessGroup> processGroup,
                            CodeGenerator &codeGenerator,
                            ScalarRegisterAllocator &scalarRegisterAllocator,
                            const Model &model)
    :   m_ProcessGroup(processGroup), m_CodeGenerator(codeGenerator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator), m_Model(model)
    {
        // If process group has an associated performance counter
        if(m_ProcessGroup->getPerformanceCounter()) {
            // Allocate perf counter registers
            // **TODO** this is rather wasteful of 4 registers! Probably better to store
            auto &c = m_CodeGenerator.get();
            startCyclesReg = m_ScalarRegisterAllocator.get().getRegister("StartCycles X");
            startCyclesHighReg = m_ScalarRegisterAllocator.get().getRegister("StartCyclesH X");
            startInstructsReg = m_ScalarRegisterAllocator.get().getRegister("StartInstructs X");
            startInstructsHighReg = m_ScalarRegisterAllocator.get().getRegister("StartInstructsH X");

            // Read performance counters
            c.csrr(*startCyclesReg, CSR::MCYCLE);
            c.csrr(*startCyclesHighReg, CSR::MCYCLEH);
            c.csrr(*startInstructsReg, CSR::MINSTRET);
            c.csrr(*startInstructsHighReg, CSR::MINSTRETH);
        }
    }

    ~PerformanceCounterScope()
    {
        // If process group has an associated performance counter
        if(m_ProcessGroup->getPerformanceCounter()) {
            auto &c = m_CodeGenerator.get();
            ScalarRegisterAllocator &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
            
            ALLOCATE_SCALAR(SPerfBase);
            ALLOCATE_SCALAR(SCountCycles);
            ALLOCATE_SCALAR(SCountCyclesH);
            ALLOCATE_SCALAR(SCountInstructs);
            ALLOCATE_SCALAR(SCountInstructsH);

            // Read performance counters
            c.csrr(*SCountCycles, CSR::MCYCLE);
            c.csrr(*SCountCyclesH, CSR::MCYCLEH);
            c.csrr(*SCountInstructs, CSR::MINSTRET);
            c.csrr(*SCountInstructsH, CSR::MINSTRETH);
            
            // Subtract previous performance counter values
            AssemblerUtils::generateSubtractUint64(c, scalarRegisterAllocator,
                                                   *SCountCycles, *SCountCyclesH,
                                                   *startCyclesReg, *startCyclesHighReg);
            AssemblerUtils::generateSubtractUint64(c, scalarRegisterAllocator,
                                                   *SCountInstructs, *SCountInstructsH,
                                                   *startInstructsReg, *startInstructsHighReg);
 
            // Get fields associated with this process group
            const auto &stateFields = m_Model.get().getStatefulFields().at(m_ProcessGroup);

            // Load performance counter base address into registers
            c.lw(*SPerfBase, Reg::X0, stateFields.at(m_ProcessGroup->getPerformanceCounter()));
            
            // Load previous performance counter values
            c.lw(*startCyclesReg, *SPerfBase, 0);
            c.lw(*startCyclesHighReg, *SPerfBase, 4);
            c.lw(*startInstructsReg, *SPerfBase, 8);
            c.lw(*startInstructsHighReg, *SPerfBase, 12);

            // Add new performance counter values
            AssemblerUtils::generateAddUint64(c, scalarRegisterAllocator,
                                              *startCyclesReg, *startCyclesHighReg,
                                              *SCountCycles, *SCountCyclesH);
            AssemblerUtils::generateAddUint64(c, scalarRegisterAllocator,
                                              *startInstructsReg, *startInstructsHighReg,
                                              *SCountInstructs, *SCountInstructsH);

            // Store updated values
            c.sw(*startCyclesReg, *SPerfBase, 0);
            c.sw(*startCyclesHighReg, *SPerfBase, 4);
            c.sw(*startInstructsReg, *SPerfBase, 8);
            c.sw(*startInstructsHighReg, *SPerfBase, 12);
        }
    }
private:
    std::shared_ptr<const ProcessGroup> m_ProcessGroup;
    std::reference_wrapper<CodeGenerator> m_CodeGenerator;
    std::reference_wrapper<ScalarRegisterAllocator> m_ScalarRegisterAllocator;
    std::reference_wrapper<const Model> m_Model;

    ScalarRegisterAllocator::RegisterPtr startCyclesReg;
    ScalarRegisterAllocator::RegisterPtr startCyclesHighReg;
    ScalarRegisterAllocator::RegisterPtr startInstructsReg;
    ScalarRegisterAllocator::RegisterPtr startInstructsHighReg;
};
class CodeGeneratorVisitor : public ModelComponentVisitor
{
public:
    CodeGeneratorVisitor(std::shared_ptr<const ProcessGroup> processGroup, 
                         ScalarRegisterAllocator::RegisterPtr timeRegister,
                         std::optional<uint32_t> numTimesteps,
                         CodeGenerator &codeGenerator, 
                         VectorRegisterAllocator &vectorRegisterAllocator, 
                         ScalarRegisterAllocator &scalarRegisterAllocator, 
                         const Model &model, bool useDRAMForWeights, bool keepParamsInRegisters,
                         RoundingMode neuronUpdateRoundingMode)
    :   m_TimeRegister(timeRegister), m_NumTimesteps(numTimesteps), m_CodeGenerator(codeGenerator), m_VectorRegisterAllocator(vectorRegisterAllocator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator), m_Model(model), 
        m_UseDRAMForWeights(useDRAMForWeights), m_KeepParamsInRegisters(keepParamsInRegisters),
        m_NeuronUpdateRoundingMode(neuronUpdateRoundingMode)
    {
        PerformanceCounterScope p(processGroup, codeGenerator, 
                                  scalarRegisterAllocator, model);

        // Visit all the processes
        for(const auto &p : processGroup->getProcesses()) {
            p->accept(*this);
        }

        // Loop through all grouped event propagation processes
        for(const auto &e : m_EventPropagationProcesses) {
            generateEventPropagationProcesses(e.second);
        }
    }

private:
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
        URAMNeuronVar(const std::string &varName, std::shared_ptr<const Variable> var, 
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
        LLMNeuronVar(const std::string &varName, std::shared_ptr<const Variable> var, 
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
        URAMLLMNeuronVar(const std::string &varName, std::shared_ptr<const Variable> var, 
                         CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                         VectorRegisterAllocator &vectorRegisterAllocator, const Model::StateFields &stateFields, 
                         ScalarRegisterAllocator::RegisterPtr timeReg)
        :   m_DelayStride(2 * var->getNumBufferTimesteps()), m_VectorRegisterAllocator(vectorRegisterAllocator)
        {
            // Check number of buffer timesteps is P.O.T.
            if(!isPOT(var->getNumBufferTimesteps())) {
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
        RowGeneratorBase(CodeGenerator &c, std::shared_ptr<const EventPropagationProcess> process,
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
        std::shared_ptr<const EventPropagationProcess> m_Process;
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

            AssemblerUtils::unrollVectorLoopBody(
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
        SparseRowGenerator(CodeGenerator &c, std::shared_ptr<const EventPropagationProcess> process,
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
            
            AssemblerUtils::unrollVectorLoopBody(
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
        DelayedRowGenerator(CodeGenerator &c, std::shared_ptr<const EventPropagationProcess> process,
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
            
            AssemblerUtils::unrollVectorLoopBody(
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

    uint32_t getBackendFieldOffset(StateObjectID id) const
    {
        return std::get<1>(m_Model.get().getBackendFields().at(static_cast<int>(id)));
    }

    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const ProcessGroup>) final
    {
        assert(false);
    }

    virtual void visit(std::shared_ptr<const NeuronUpdateProcess> neuronUpdateProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        // Get fields associated with this process
        const auto &stateFields = m_Model.get().getStatefulFields().at(neuronUpdateProcess);

        // Build literal pool
        std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> literalPool;
        updateLiteralPool(neuronUpdateProcess->getTokens(), m_VectorRegisterAllocator.get(), literalPool);
        
        // Define type for event-emitting function
        const auto emitEventFunctionType = Type::ResolvedType::createFunction(Type::Void, {});

        // Load literals
        for(const auto &l : literalPool) {
            c.vlui(*l.second, (uint16_t)l.first);
        }

         // For now, unrollVectorLoopBody requires SOME buffers
        assert(!neuronUpdateProcess->getVariables().empty());

        std::unordered_map<std::shared_ptr<const Variable>, std::unique_ptr<NeuronVarBase>> varBuffers;
        {
            // If any variables have buffering, calculate stride in bytes
            // **TODO** make set of non-1 bufferings and pre-multiply time by this
            ALLOCATE_SCALAR(SNumVariableBytes);
            if (std::any_of(neuronUpdateProcess->getVariables().cbegin(), neuronUpdateProcess->getVariables().cend(),
                            [](const auto e) { return e.second->getNumBufferTimesteps() != 1; }))
            {
                c.li(*SNumVariableBytes, ceilDivide(neuronUpdateProcess->getNumNeurons(), 32) * 64);
            }

            // Loop through neuron variables
            for(const auto &v : neuronUpdateProcess->getVariables()) {
                // Visit all users of this variable to determine how it should be implemented
                VariableImplementerVisitor visitor(v.second, m_Model.get().getStateProcesses().at(v.second),
                                                   m_UseDRAMForWeights);

                // If variable can be implemented in URAM
                if(visitor.isURAMCompatible()) {
                    varBuffers.emplace(v.second, 
                                       std::make_unique<URAMNeuronVar>(v.first, v.second, c, scalarRegisterAllocator,
                                                                       stateFields, m_NumTimesteps, m_TimeRegister, SNumVariableBytes));
                }
                // Otherwise, if variable is purely implemented in LLM
                else if(visitor.isLLMCompatible()){
                    varBuffers.emplace(v.second, 
                                        std::make_unique<LLMNeuronVar>(v.first, v.second, c, scalarRegisterAllocator,
                                                                       m_VectorRegisterAllocator.get(), stateFields));
                }
                else if(visitor.isURAMLLMCompatible()) {
                    varBuffers.emplace(v.second, 
                                       std::make_unique<URAMLLMNeuronVar>(v.first, v.second, c, scalarRegisterAllocator,
                                                                          m_VectorRegisterAllocator.get(), stateFields, m_TimeRegister));
                }
                // Otherwise, 
                else {
                    assert(false);
                }
            }
        }

        std::unordered_map<std::shared_ptr<const EventContainer>, ScalarRegisterAllocator::RegisterPtr> eventBufferRegisters;
        {
            // If any output events have buffering, calculate stride in bytes
            // **TODO** make set of non-1 bufferings and pre-multiply time by this
            ALLOCATE_SCALAR(SNumEventBytes);
            if(std::any_of(neuronUpdateProcess->getOutputEvents().cbegin(), neuronUpdateProcess->getOutputEvents().cend(),
                           [](const auto e){ return e.second->getNumBufferTimesteps() != 1; }))
            {
                c.li(*SNumEventBytes, ceilDivide(neuronUpdateProcess->getNumNeurons(), 32) * 4);
            }

            // Loop through neuron event outputs
            for(const auto &e : neuronUpdateProcess->getOutputEvents()) {
                // Allocate scalar register to hold address of variable
                const auto reg = m_ScalarRegisterAllocator.get().getRegister((e.first + "Buffer X").c_str());

                // Add register to map
                eventBufferRegisters.try_emplace(e.second, reg);

                // Generate code to load address
                c.lw(*reg, Reg::X0, stateFields.at(e.second));

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
        EnvironmentExternal env(m_CodeGenerator.get());

        // Loop through neuron parameters
        for(const auto &p : neuronUpdateProcess->getParameters()) {
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

        }

        env.add(Type::S8_7, "_zero", literalPool.at(0));

        // Build library with fennrand function and stochastic multiplication
        EnvironmentLibrary::Library functionLibrary;
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
        if(isExpCalled(neuronUpdateProcess->getTokens())) {
            SpecialFunctions::Exp::add(c, scalarRegisterAllocator, m_VectorRegisterAllocator.get(),
                                       env, functionLibrary, getBackendFieldOffset(StateObjectID::LUT_EXP));
        }
        // Insert environment with this library
        EnvironmentLibrary envLibrary(env, functionLibrary);
        
        // **YUCK** find first scalar-addressed variable to use for loop check
        auto firstScalarAddressRegister = std::find_if(varBuffers.cbegin(), varBuffers.cend(),
                                                       [](const auto &v){ return v.second->getLoopCountScalarReg(); });
        assert(firstScalarAddressRegister != varBuffers.cend());

        // Build vectorised neuron loop
        AssemblerUtils::unrollVectorLoopBody(
            envLibrary.getCodeGenerator(), m_ScalarRegisterAllocator.get(),
            neuronUpdateProcess->getNumNeurons(), 4, *firstScalarAddressRegister->second->getLoopCountScalarReg(),
            [this, &envLibrary, &eventBufferRegisters, &literalPool, &neuronUpdateProcess,
             &emitEventFunctionType, &varBuffers]
            (CodeGenerator&, uint32_t r, bool, ScalarRegisterAllocator::RegisterPtr maskReg)
            {
                EnvironmentExternal unrollEnv(envLibrary);

                // Loop through variables
                for(const auto &v : neuronUpdateProcess->getVariables()) {
                    // Allocate vector register
                    const auto reg = m_VectorRegisterAllocator.get().getRegister((v.first + " V").c_str());

                    // Add to environment
                    unrollEnv.add(v.second->getType(), v.first, reg);

                    // Generate load
                    varBuffers.at(v.second)->genLoad(unrollEnv, reg, r);
                }

                // Loop through neuron event outputs
                for(const auto &e : neuronUpdateProcess->getOutputEvents()) {
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
                    TypeChecker::EnvironmentInternal typeCheckEnv(unrollEnv);
                    EnvironmentInternal compilerEnv(unrollEnv);
                    ErrorHandler errorHandler("Neuron update process '" + neuronUpdateProcess->getName() + "'");        
                    compileStatements(neuronUpdateProcess->getTokens(), {}, literalPool, typeCheckEnv, compilerEnv,
                                      errorHandler, nullptr, nullptr, m_NeuronUpdateRoundingMode, 
                                      m_ScalarRegisterAllocator.get(), m_VectorRegisterAllocator.get());
                }

                // Loop through variables
                for(const auto &v : neuronUpdateProcess->getVariables()) {
                    // Get register
                    const auto reg = std::get<VectorRegisterAllocator::RegisterPtr>(unrollEnv.getRegister(v.first));
                    
                    // Generate store
                    varBuffers.at(v.second)->genStore(unrollEnv, reg, r);
                }
            },
            [this, &eventBufferRegisters, &neuronUpdateProcess, &varBuffers]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                // If any variables have vector addresses i.e. are stored in LLM, load number of bytes to unroll
                VectorRegisterAllocator::RegisterPtr numUnrollBytesReg;
                if(std::any_of(varBuffers.cbegin(), varBuffers.cend(),
                               [](const auto &v){ return v.second->needsNumUnrollBytesReg(); }))
                {
                    numUnrollBytesReg = m_VectorRegisterAllocator.get().getRegister("NumUnrollBytes V");
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

    virtual void visit(std::shared_ptr<const RNGInitProcess> rngInitProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        //**TODO** check seed shape

        // Get fields associated with this process
        const auto &stateFields = m_Model.get().getStatefulFields().at(rngInitProcess);

        // Allocate scalar register to hold address of seed buffer
        ALLOCATE_SCALAR(SReg);

        // Generate code to load address of seed
        c.lw(*SReg, Reg::X0, stateFields.at(rngInitProcess->getSeed()));

        // Load seed into RNG registers
        c.vloadr0(*SReg);
        c.vloadr1(*SReg, 64);
    }

    virtual void visit(std::shared_ptr<const BroadcastProcess> broadcastProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

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
        
            if(std::holds_alternative<VariablePtr>(broadcastProcess->getTarget())) {
                auto target = std::get<VariablePtr>(broadcastProcess->getTarget());
                
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
        c.lw(*SDataBuffer, Reg::X0, stateFields.at(broadcastProcess->getSource()));

        // Load size in bytes and add to scalar buffer to get end
        c.li(*SDataBufferEnd, 2 * broadcastProcess->getSource()->getShape().getDims().at(0));
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
    }

    virtual void visit(std::shared_ptr<const MemsetProcess> memsetProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        // Get fields associated with this process
        const auto &stateFields = m_Model.get().getStatefulFields().at(memsetProcess);
        
        // If target is a variable reference
        if(std::holds_alternative<VariablePtr>(memsetProcess->getTarget())) {
            auto target = std::get<VariablePtr>(memsetProcess->getTarget());
            
            // Visit all users of this variable to determine how it has been be implemented
            VariableImplementerVisitor visitor(target, m_Model.get().getStateProcesses().at(target),
                                               m_UseDRAMForWeights);

            // Determine how many vectors we're memsetting
            const size_t numVecsOneTimestep = ceilDivide(target->getShape().getFlattenedSize(), 32);
            const size_t numVecs = numVecsOneTimestep * target->getNumBufferTimesteps();

            // Allocate register for target address
            ALLOCATE_SCALAR(STargetBuffer);
            
            if(visitor.isURAMCompatible()) {
                c.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
                generateURAMMemset(numVecs, STargetBuffer);
            }
            else if(visitor.isLLMCompatible()) {
                c.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
                generateLLMMemset(numVecs, STargetBuffer);
            }
            else if(visitor.isURAMLLMCompatible()) {
                c.lw(*STargetBuffer, Reg::X0, stateFields.at(target));
                generateURAMMemset(numVecsOneTimestep, STargetBuffer);

                c.lw(*STargetBuffer, Reg::X0, stateFields.at(target) + 4);
                generateLLMMemset(numVecs, STargetBuffer);
            }
            /*else if(visitor.isBRAMCompatible()) {
            }*/
            else {
                assert(false);
            }
        }
        // Otherwise
        else {
            // Get state object ID
            const int target = std::get<int>(memsetProcess->getTarget());
            
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
                throw std::runtime_error("Memset process '" + memsetProcess->getName() 
                                         + "' targets incompatible backend state object "
                                        + std::to_string(target));
            }
        }
    }

    virtual void visit(std::shared_ptr<const EventPropagationProcess> eventPropagationProcess)
    {
        // Add event propagation process to vector of processes with same input event container
        m_EventPropagationProcesses[eventPropagationProcess->getInputEvents()].emplace_back(eventPropagationProcess);
    }

    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void generateLLMMemset(size_t numVectors, ScalarRegisterAllocator::RegisterPtr targetReg)
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        ALLOCATE_VECTOR(VValue);
        ALLOCATE_VECTOR(VLLMAddress);
        ALLOCATE_VECTOR(VNumUnrollBytes);
        
        // Load value to memset
        c.vlui(*VValue, 0);

        // Broadcast address
        c.vfill(*VLLMAddress, *targetReg);

        // Generate unrolled loop 
        AssemblerUtils::unrollVectorLoopBody(
            c, scalarRegisterAllocator, numVectors * 32, 4, *targetReg,
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

    void generateURAMMemset(size_t numVectors, ScalarRegisterAllocator::RegisterPtr targetReg)
    {
        // Make some friendlier-named references
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        ALLOCATE_VECTOR(VValue);

        // Load value to memset and calculate unroll bytes
        // **TODO** parameterise
        c.vlui(*VValue, 0);
  
        // Generate unrolled loop 
        AssemblerUtils::unrollVectorLoopBody(
            c, m_ScalarRegisterAllocator.get(), numVectors * 32, 4, *targetReg,
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

    void generateURAMWordLoop(const std::vector<std::unique_ptr<RowGeneratorBase>> &rowGenerators, 
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

                // CurrentEventWord = CurrentEventWord << (NumLZ + 1)*/
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

    void generateEventPropagationProcesses(const std::vector<std::shared_ptr<const EventPropagationProcess>> &processes)
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        // Register allocation
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
        }
        
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_map<std::shared_ptr<const EventContainer>, std::vector<std::shared_ptr<const EventPropagationProcess>>> m_EventPropagationProcesses;
    ScalarRegisterAllocator::RegisterPtr m_TimeRegister;
    std::optional<uint32_t> m_NumTimesteps;
    std::reference_wrapper<CodeGenerator> m_CodeGenerator;
    std::reference_wrapper<VectorRegisterAllocator> m_VectorRegisterAllocator;
    std::reference_wrapper<ScalarRegisterAllocator> m_ScalarRegisterAllocator;
    std::reference_wrapper<const Model> m_Model;
    bool m_UseDRAMForWeights;
    bool m_KeepParamsInRegisters;
    RoundingMode m_NeuronUpdateRoundingMode;
};
}

//------------------------------------------------------------------------
// URAMArrayBase
//------------------------------------------------------------------------
void URAMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy URAM pointer into bytes
    const uint32_t uramPointer = getURAMPointer(); 
    std::memcpy(bytes.data(), &uramPointer, 4);
}

//------------------------------------------------------------------------
// BRAMArrayBase
//------------------------------------------------------------------------
void BRAMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy BRAM pointer into bytes
    const uint32_t bramPointer = getBRAMPointer();
    std::memcpy(bytes.data(), &bramPointer, 4);
}

//------------------------------------------------------------------------
// LLMArrayBase
//------------------------------------------------------------------------
void LLMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy LLM pointer into bytes
    // **NOTE** the code wants lane addresses not array addresses
    // **THINK** would it be better to allocate 2 byte aligned?
    const uint32_t llmPointer = getLLMPointer() / 32;
    std::memcpy(bytes.data(), &llmPointer, 4);
}

//----------------------------------------------------------------------------
// DRAMArrayBase
//----------------------------------------------------------------------------
void DRAMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy DRAM pointer into bytes
    const uint32_t dramPointer = getDRAMPointer();
    std::memcpy(bytes.data(), &dramPointer, 4);
}

//----------------------------------------------------------------------------
// URAMLLMArrayBase
//----------------------------------------------------------------------------
void URAMLLMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 8 bytes of space
    bytes.resize(8);

    // Memcpy URAM pointer into bytes
    const uint32_t uramPointer = getURAMPointer();
    std::memcpy(bytes.data(), &uramPointer, 4);

    // Memcpy LLM pointer into bytes
    // **NOTE** the code wants lane addresses not array addresses
    // **THINK** would it be better to allocate 2 byte aligned?
    const uint32_t llmPointer = getLLMPointer() / 32;
    std::memcpy(bytes.data() + 4, &llmPointer, 4);
}

//----------------------------------------------------------------------------
// BackendFeNN
//------------------------------------------------------------------------
std::vector<uint32_t> BackendFeNN::generateSimulationKernel(const std::vector<std::shared_ptr<const ProcessGroup>> &timestepProcessGroups,
                                                            const std::vector <std::shared_ptr<const ProcessGroup>> &beginProcessGroups,
                                                            const std::vector <std::shared_ptr<const ProcessGroup>> &endProcessGroups,
                                                            uint32_t numTimesteps, const Model &model) const
{
    uint32_t readyFlagPtr = 0;
    return AssemblerUtils::generateStandardKernel(
        shouldGenerateSimulationKernels(), readyFlagPtr,
        [=, &endProcessGroups, &model, &timestepProcessGroups]
        (CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Register allocation
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);

            // Labels
            auto timeLoop = createLabel();
            
            // If performance counters are enabled, disinhibit them
            // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
            if(arePerformanceCountersRequired(timestepProcessGroups) 
               || arePerformanceCountersRequired(endProcessGroups)) 
            {
                c.csrw(CSR::MCOUNTINHIBIT, Reg::X0);
            }

            // Set timestep range and load ready flag pointer
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);
            
            // Visit begin process group
            for (const auto &p : beginProcessGroups) {
                CodeGeneratorVisitor visitor(p, nullptr, std::nullopt, c, vectorRegisterAllocator,
                                             scalarRegisterAllocator, model,
                                             m_UseDRAMForWeights, m_KeepParamsInRegisters,
                                             m_NeuronUpdateRoundingMode);
            }

            // Loop over time
            c.L(timeLoop);
            {
                // Visit timestep process group
                for (const auto &p : timestepProcessGroups) {
                    CodeGeneratorVisitor visitor(p, STime, numTimesteps, c, vectorRegisterAllocator,
                                                 scalarRegisterAllocator, model,
                                                 m_UseDRAMForWeights, m_KeepParamsInRegisters,
                                                 m_NeuronUpdateRoundingMode);
                }
                
                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }

            // Visit end process group
            for (const auto &p : endProcessGroups) {
                CodeGeneratorVisitor visitor(p, nullptr, std::nullopt, c, vectorRegisterAllocator,
                                             scalarRegisterAllocator, model,
                                             m_UseDRAMForWeights, m_KeepParamsInRegisters,
                                             m_NeuronUpdateRoundingMode);
            }
        });
}
//------------------------------------------------------------------------
std::vector<uint32_t> BackendFeNN::generateKernel(const std::vector <std::shared_ptr<const ProcessGroup>> &processGroups,
                                                  const Model &model) const
{
    uint32_t readyFlagPtr = 0;
    return AssemblerUtils::generateStandardKernel(
        shouldGenerateSimulationKernels(), readyFlagPtr,
        [=, &model, &processGroups]
        (CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // If performance counters are enabled, disinhibit them
            // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
            if(arePerformanceCountersRequired(processGroups)) {
                c.csrw(CSR::MCOUNTINHIBIT, Reg::X0);
            }

            // Visit process groups
            for (const auto &p : processGroups) {
                CodeGeneratorVisitor visitor(p, nullptr, std::nullopt, c, vectorRegisterAllocator,
                                             scalarRegisterAllocator, model,
                                             m_UseDRAMForWeights, m_KeepParamsInRegisters,
                                             m_NeuronUpdateRoundingMode);
            }
        });
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const Variable> variable,
                                                    const Model::StateProcesses::mapped_type &processes,
                                                    StateBase *state) const
{
    // Pad last dimension to multiplies of 32
    // **THINK** how much of this belongs in Shape?
    // **TODO** more information is required here to seperate variables with
    // shape (B,) which shouldn't be padded from (N,) variables which should
    auto varDims = variable->getShape().getDims();
    varDims.back() = padSize(varDims.back(), 32);

    // Multiple padded dimensions together
    const size_t countOneTimestep = std::accumulate(varDims.cbegin(), varDims.cend(), 
                                                    1, std::multiplies<size_t>());
    const size_t count = countOneTimestep * variable->getNumBufferTimesteps();

    // Create URAM array if variable can be implemented there
    VariableImplementerVisitor visitor(variable, processes, m_UseDRAMForWeights);
    if(visitor.isURAMCompatible()) {
        LOGI << "Creating variable '" << variable->getName() << "' array in URAM";
        return state->createURAMArray(variable->getType(), count);
    }
    // Otherwise, create LLM array if variable can be implemented there
    else if(visitor.isLLMCompatible()) {
        LOGI << "Creating variable '" << variable->getName() << "' array in LLM";
        return state->createLLMArray(variable->getType(), count);
    }
    else if(visitor.isURAMLLMCompatible()) {
        LOGI << "Creating variable '" << variable->getName() << "' array in URAM and LLM";
        return state->createURAMLLMArray(variable->getType(), countOneTimestep, count);
    }
    // Otherwise, create BRAM array if variable can be implemented there
    else if(visitor.isBRAMCompatible()) {
        LOGI << "Creating variable '" << variable->getName() << "' array in BRAM";
        return state->createBRAMArray(variable->getType(), count);
    }
    // Otherwise, create DRAM array if variable can be implemented there
    else if(visitor.isDRAMCompatible()) {
        LOGI << "Creating variable '" << variable->getName() << "' array in DRAM";
        return state->createDRAMArray(variable->getType(), count);
    }
    else {
        assert(false);
    }
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const EventContainer> eventContainer,
                                                    const Model::StateProcesses::mapped_type&,
                                                    StateBase *state) const
{
    LOGI << "Creating event container '" << eventContainer->getName() << "' array in BRAM";

    // Event containers are always implemented as BRAM bitfields
    const size_t numSpikeWords = ceilDivide(eventContainer->getShape().getFlattenedSize(), 32) * eventContainer->getNumBufferTimesteps();
    return state->createBRAMArray(GeNN::Type::Uint32, numSpikeWords);
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const PerformanceCounter> performanceCounter,
                                                    const Model::StateProcesses::mapped_type&,
                                                    StateBase *state) const
{
    LOGI << "Creating performance counter '" << performanceCounter->getName() << "' array in BRAM";

    // Performance counter contains a 64-bit number for 
    // instructions retired and one for number of cycles 
    return state->createBRAMArray(GeNN::Type::Uint64, 2);
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const State> state, 
                                                    int stateID, StateBase *backendState) const
{
    auto variable = std::static_pointer_cast<const Variable>(state);
    switch(static_cast<StateObjectID>(stateID)) {
    case StateObjectID::ROW_BUFFER_A:
    case StateObjectID::ROW_BUFFER_B:
    {
        LOGI << "Creating row buffer array in URAM";
        return backendState->createURAMArray(variable->getType(), variable->getShape().getFlattenedSize());
    }

    case StateObjectID::LUT_EXP:
        LOGI << "Creating LUT array in LLM";
        return backendState->createLLMArray(variable->getType(), variable->getShape().getFlattenedSize());

    default:
        assert(false);
    }
}
//------------------------------------------------------------------------
std::unordered_map<int, std::shared_ptr<State>> BackendFeNN::getRequiredStateObjects(const Model &model) const
{
    // If we should use DRAM for weights, we require two row buffers
    std::unordered_map<int, std::shared_ptr<State>> stateObjects;
    if(m_UseDRAMForWeights) {
         // Visit model to find largest max row length
        MaxRowLengthVisitor visitor(model);

        // Create DMA buffers
        LOGI << "Creating DMA row buffers with " + std::to_string(visitor.getMaxRowLength()) + " entries";
        const Shape rowBufferShape({visitor.getMaxRowLength()});
        stateObjects.try_emplace(static_cast<int>(StateObjectID::ROW_BUFFER_A), Variable::create(rowBufferShape, Type::Int16, 1, "RowBufferA"));
        stateObjects.try_emplace(static_cast<int>(StateObjectID::ROW_BUFFER_B), Variable::create(rowBufferShape, Type::Int16, 1, "RowBufferB"));
    }

    // Visit model to find what LUT object IDs are required and copy into state IDs
    LUTVisitor visitor(model);
    const Shape lutShape({SpecialFunctions::getLUTCount(), 32});
    std::transform(visitor.getLUTObjectIDs().cbegin(), visitor.getLUTObjectIDs().cend(),
                   std::inserter(stateObjects, stateObjects.end()), 
                   [&lutShape](auto i)
                   { 
                       return std::make_pair(static_cast<int>(i), 
                                             Variable::create(lutShape, Type::Int16, 1, "LUT")); 
                   });

    return stateObjects;
}