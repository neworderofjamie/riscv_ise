#include "backend/backend_fenn.h"

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
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// Compiler includes
#include "compiler/compiler.h"
#include "compiler/event_container.h"
#include "compiler/parameter.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"

// Backend includes
#include "backend/environment.h"
#include "backend/memory_allocator.h"
#include "backend/special_function.h"

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

bool isIdentifierCalled(const std::string &identifierName, const std::vector<Transpiler::Token> &tokens)
{
    // Loop through tokens
    for(auto t = tokens.cbegin(); t != tokens.cend(); t++) {
        // If token is an identifier with correct name
        if(t->type == Transpiler::Token::Type::IDENTIFIER && t->lexeme == identifierName) {
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
        m_URAMCompatible(true), m_BRAMCompatible(true), m_DRAMCompatible(true)
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

private:
    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const NeuronUpdateProcess> neuronUpdateProcess)
    {
        const auto var = std::find_if(neuronUpdateProcess->getVariables().cbegin(), neuronUpdateProcess->getVariables().cend(),
                                      [this](const auto &v){ return v.second == m_Variable; });
        assert(var != neuronUpdateProcess->getVariables().cend());
     
        // Neuron variables can be located in URAM or LLM
        m_BRAMCompatible = false;
        m_DRAMCompatible = false;

        if(!m_URAMCompatible && !m_LLMCompatible) {
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

            // If it's sparse or delayed, it also can't be located in URAM
            if(eventPropagationProcess->getNumSparseConnectivityBits() > 0
               || eventPropagationProcess->getNumDelayBits() > 0) 
            {
                m_URAMCompatible = false;
            }
            
            if(!m_URAMCompatible && !m_LLMCompatible) {
                throw std::runtime_error("Event propagation process '" + eventPropagationProcess->getName()
                                        + "' target array '" + eventPropagationProcess->getTarget()->getName()
                                        + "' shared with incompatible processes");
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

            if(!m_BRAMCompatible) {
                throw std::runtime_error("Broadcast process '" + broadcastProcess->getName() 
                                        + "' source array '" + broadcastProcess->getSource()->getName()
                                        + "' shared with incompatible processes");
            }
        }
        else {
            // Otherwise, if variable's target, it can only located in LLM
            // **TODO** URAM would also be sensible IN THEORY 
            const auto target = std::get<VariablePtr>(broadcastProcess->getTarget());
            assert(m_Variable == target);
            
            m_BRAMCompatible = false;
            m_URAMCompatible = false;
            m_DRAMCompatible = false;

            if(!m_LLMCompatible) {
                throw std::runtime_error("Broadcast process '" + broadcastProcess->getName()
                                        + "' target array '" + target->getName()
                                        + "' shared with incompatible processes");
            }
        }
    }

    virtual void visit(std::shared_ptr<const CopyProcess> copyProcess)
    {
        // If variable is source, it can only be located in URAM
        if(m_Variable == copyProcess->getSource()) {
            m_LLMCompatible = false;
            m_BRAMCompatible = false;
            m_DRAMCompatible = false;

            if(!m_URAMCompatible) {
                throw std::runtime_error("Copy process '" + copyProcess->getName() 
                                        + "' source array '" + copyProcess->getSource()->getName()
                                        + "' shared with incompatible processes");
            }
        }
        // Otherwise, if variable's target, it can either be located in BRAM
        else if(m_Variable == copyProcess->getTarget()) {
            m_LLMCompatible = false;
            m_URAMCompatible = false;
            m_DRAMCompatible = false;

            if(!m_BRAMCompatible) {
                throw std::runtime_error("Copy process '" + copyProcess->getName()
                                        + "' target array '" + copyProcess->getTarget()->getName()
                                        + "' shared with incompatible processes");
            }
        }
        else {
            assert(false);
        }
    }

    virtual void visit(std::shared_ptr<const MemsetProcess> memsetProcess)
    {
        const auto target = std::get<VariablePtr>(memsetProcess->getTarget());
        assert(m_Variable == target);

        // **TODO** memset could handle anything
        m_BRAMCompatible = false;
        m_DRAMCompatible = false;

        if(!m_LLMCompatible && !m_URAMCompatible) {
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
        if(isIdentifierCalled("exp", neuronUpdateProcess->getTokens())) {
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
    // Using directives
    //------------------------------------------------------------------------
    using EventPropagationProcessRegisters = std::vector<std::tuple<ScalarRegisterAllocator::RegisterPtr, 
                                                                    VectorRegisterAllocator::RegisterPtr, 
                                                                    VectorRegisterAllocator::RegisterPtr>>;

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

        std::unordered_map<std::shared_ptr<const Variable>, std::vector<RegisterPtr>> varBufferRegisters;
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
                    // Allocate scalar register to hold address of variable
                    const auto regRead = m_ScalarRegisterAllocator.get().getRegister((v.first + "Buffer X").c_str());

                    // Add register to map
                    varBufferRegisters[v.second].emplace_back(regRead);

                    // Generate code to load address
                    c.lw(*regRead, Reg::X0, stateFields.at(v.second));

                    // **TODO** currently this just handles providing entire simulation kernel worth of variable data or
                    // recording variables for entire simulation - extend to support axonal delays and ring-buffer recording
                    const size_t numBufferTimesteps = v.second->getNumBufferTimesteps();
                    if (numBufferTimesteps != 1) {
                        // Check there is a buffer entry for each timestep with one extra
                        // **NOTE** variables get read from timestep and written to timestep + 1 hence extra buf
                        if(numBufferTimesteps < (m_NumTimesteps.value() + 1)) {
                            throw std::runtime_error("Variables need to be buffered for " + std::to_string(m_NumTimesteps.value() + 1u) + " timesteps");
                        }

                        ALLOCATE_SCALAR(STmp);
                        c.mul(*STmp, *m_TimeRegister, *SNumVariableBytes);
                        c.add(*regRead, *regRead, *STmp);

                        // Allocate additional register for writing variable
                        // **TODO** should be lazy
                        const auto regWrite = m_ScalarRegisterAllocator.get().getRegister((v.first + "BufferWrite X").c_str());
                        c.add(*regWrite, *regRead, *SNumVariableBytes);

                        // Add additional register
                        varBufferRegisters[v.second].emplace_back(regWrite);
                    }
                }
                // Otherwise, if it can be implemented in LLM
                else if(visitor.isLLMCompatible()){
                    // Allocate vector register to hold address of variable
                    const auto reg = m_VectorRegisterAllocator.get().getRegister((v.first + "Buffer V").c_str());

                    // Add register to map
                    varBufferRegisters[v.second].emplace_back(reg);

                    // Generate code to load address
                    ALLOCATE_SCALAR(STmp);
                    c.lw(*STmp, Reg::X0, stateFields.at(v.second));
                    c.vfill(*reg, *STmp);

                    assert(v.second->getNumBufferTimesteps() == 1);
                }
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
        
        // If exp is called
        if(isIdentifierCalled("exp", neuronUpdateProcess->getTokens())) {
            // Get field containing LUT for exponential function
            const auto &lutField = m_Model.get().getBackendFields().at(static_cast<int>(StateObjectID::LUT_EXP));

            // Add special function to environment
            SpecialFunctions::Exp::add(c, scalarRegisterAllocator, m_VectorRegisterAllocator.get(),
                                       env, functionLibrary, std::get<1>(lutField));
        }
        // Insert environment with this library
        EnvironmentLibrary envLibrary(env, functionLibrary);
        
        // **YUCK** find first scalar-addressed variable to use for loop check
        auto firstScalarAddressRegister = std::find_if(varBufferRegisters.cbegin(), varBufferRegisters.cend(),
                                                       [](const auto &v){ return std::holds_alternative<ScalarRegisterAllocator::RegisterPtr>(v.second.front()); });
        assert(firstScalarAddressRegister != varBufferRegisters.cend());

        // Build vectorised neuron loop
        AssemblerUtils::unrollVectorLoopBody(
            envLibrary.getCodeGenerator(), m_ScalarRegisterAllocator.get(),
            neuronUpdateProcess->getNumNeurons(), 4, *std::get<ScalarRegisterAllocator::RegisterPtr>(firstScalarAddressRegister->second.front()),
            [this, &envLibrary, &eventBufferRegisters, &literalPool, &neuronUpdateProcess,
             &emitEventFunctionType, &varBufferRegisters]
            (CodeGenerator&, uint32_t r, bool, ScalarRegisterAllocator::RegisterPtr maskReg)
            {
                EnvironmentExternal unrollEnv(envLibrary);

                // Loop through variables
                for(const auto &v : neuronUpdateProcess->getVariables()) {
                    // Allocate vector register
                    const auto reg = m_VectorRegisterAllocator.get().getRegister((v.first + " V").c_str());

                    // Add to environment
                    unrollEnv.add(v.second->getType(), v.first, reg);

                    // Load vector from correct memory space depending on type of address register
                    std::visit(
                        Utils::Overload{
                            [&unrollEnv, reg, r](ScalarRegisterAllocator::RegisterPtr bufferReg)
                            {
                                unrollEnv.getCodeGenerator().vloadv(*reg, *bufferReg, 64 * r);            
                            },
                            [&unrollEnv, reg, r](VectorRegisterAllocator::RegisterPtr bufferReg)
                            {
                                unrollEnv.getCodeGenerator().vloadl(*reg, *bufferReg, 2 * r);    
                            }},
                        varBufferRegisters.at(v.second).front());
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
                    
                    // Store updated vector back to buffer
                    std::visit(
                        Utils::Overload{
                            [&unrollEnv, reg, r](ScalarRegisterAllocator::RegisterPtr bufferReg)
                            {
                                unrollEnv.getCodeGenerator().vstore(*reg, *bufferReg, 64 * r);          
                            },
                            [&unrollEnv, reg, r](VectorRegisterAllocator::RegisterPtr bufferReg)
                            {
                                unrollEnv.getCodeGenerator().vstorel(*reg, *bufferReg, 2 * r);    
                            }},
                        varBufferRegisters.at(v.second).back());
                }
            },
            [this, &eventBufferRegisters, &neuronUpdateProcess, &varBufferRegisters]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                // If any variables have vector addresses i.e. are stored in LLM, load number of bytes to unroll
                VectorRegisterAllocator::RegisterPtr numUnrollBytesReg;
                if(std::any_of(varBufferRegisters.cbegin(), varBufferRegisters.cend(),
                               [](const auto &v){ return std::holds_alternative<VectorRegisterAllocator::RegisterPtr>(v.second.front()); }))
                {
                    numUnrollBytesReg = m_VectorRegisterAllocator.get().getRegister("NumUnrollBytes V");
                    c.vlui(*numUnrollBytesReg, numUnrolls * 2);
                }
            
                // Loop through variables and increment all buffers
                // **TODO** based on data structure, determine stride
                for(const auto &v : neuronUpdateProcess->getVariables()) {
                    for(const auto &r : varBufferRegisters.at(v.second)) {
                        std::visit(
                            Utils::Overload{
                                [&c, numUnrolls](ScalarRegisterAllocator::RegisterPtr bufferReg)
                                {
                                    c.addi(*bufferReg, *bufferReg, 64 * numUnrolls);
                                },
                                [&c, numUnrollBytesReg](VectorRegisterAllocator::RegisterPtr bufferReg)
                                {
                                    c.vadd(*bufferReg, *bufferReg, *numUnrollBytesReg);
                                }},
                            r);
                    }
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
        Label halfWordLoop;

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

    virtual void visit(std::shared_ptr<const CopyProcess> copyProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        // **TODO** URAM->BRAM copy assumed

        // Get fields associated with this process
        const auto &stateFields = m_Model.get().getStatefulFields().at(copyProcess);

        // Register allocation
        ALLOCATE_SCALAR(SDataBuffer);
        ALLOCATE_SCALAR(SVectorBuffer)
        ALLOCATE_SCALAR(SVectorBufferEnd);

        // Labels
        Label vectorLoop;

        // Load source and target start address
        c.lw(*SVectorBuffer, Reg::X0, stateFields.at(copyProcess->getSource()));
        c.lw(*SDataBuffer, Reg::X0, stateFields.at(copyProcess->getTarget()));

        {
            // Calculate vector-padded size in bytes and add to get end address
            ALLOCATE_SCALAR(STemp);
            c.li(*STemp, 2 * padSize(copyProcess->getSource()->getShape().getFlattenedSize(), 32));
            c.add(*SVectorBufferEnd, *SVectorBuffer, *STemp);

            // If target has buffering, multiply timestep by stride and add to target buffer pointer
            if (copyProcess->getTarget()->getNumBufferTimesteps() != 1) {
                ALLOCATE_SCALAR(STmp2);
                c.mul(*STmp2, *m_TimeRegister, *STemp);
                c.add(*SDataBuffer, *SDataBuffer, *STmp2);
            }
        }
        
        // Loop over vectors
        c.L(vectorLoop);
        {
            // Register allocation
            ALLOCATE_VECTOR(VData);
            ALLOCATE_SCALAR(SVal);

            // Load vector
            c.vloadv(*VData, *SVectorBuffer, 0);

            // **STALL**
            c.nop();
            
            // Unroll lane loop
            for(int l = 0; l < 32; l++) {
                // Extract lane into scalar registers
                c.vextract(*SVal, *VData, l);

                // Store halfword
                c.sh(*SVal, *SDataBuffer, l * 2);
            }

            // SVectorBuffer += 64
            c.addi(*SVectorBuffer, *SVectorBuffer, 64);

            // SDataBuffer += 64
            c.addi(*SDataBuffer, *SDataBuffer, 64);
        
            // If SVectorBuffer != SVectorBufferEnd, goto vector loop
            c.bne(*SVectorBuffer, *SVectorBufferEnd, vectorLoop);
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

            // Load target address
            ALLOCATE_SCALAR(STargetBuffer);
            c.lw(*STargetBuffer, Reg::X0, stateFields.at(target));

            if(visitor.isURAMCompatible()) {
                generateURAMMemset(target->getShape(), STargetBuffer);
            }
            else if(visitor.isLLMCompatible()) {
                generateLLMMemset(target->getShape(), STargetBuffer);
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
                 generateLLMMemset(targetVar->getShape(), STargetBuffer);
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
    void generateLLMMemset(const Shape &shape, ScalarRegisterAllocator::RegisterPtr targetReg)
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        ALLOCATE_VECTOR(VValue);
        ALLOCATE_VECTOR(VLLMAddress);
        ALLOCATE_VECTOR(VNumUnrollBytes);
        
        // Determine how many vectors we're memsetting
        const size_t numVectors = ceilDivide(shape.getFlattenedSize(), 32);

        // Load value to memset and calculate unroll bytes
        // **TODO** parameterise
        c.vlui(*VValue, 0);
        c.vlui(*VNumUnrollBytes, 2 * std::min(numVectors, size_t{4}));

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
            [targetReg, VLLMAddress, VNumUnrollBytes]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                c.vadd(*VLLMAddress, *VLLMAddress, *VNumUnrollBytes);
                c.addi(*targetReg, *targetReg, 64 * numUnrolls);
            });
    }

    void generateURAMMemset(const Shape &shape, ScalarRegisterAllocator::RegisterPtr targetReg)
    {
        // Make some friendlier-named references
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        ALLOCATE_VECTOR(VValue);
        
        // Determine how many vectors we're memsetting
        const size_t numVectors = ceilDivide(shape.getFlattenedSize(), 32);

        // Load value to memset and calculate unroll bytes
        // **TODO** parameterise
        c.vlui(*VValue, 0);
  
        // Generate unrolled loop 
        AssemblerUtils::unrollVectorLoopBody(
            c, m_ScalarRegisterAllocator.get(), numVectors * 32, 4, *targetReg,
            [targetReg, VValue]
            (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr)
            {
                c.vstore(*VValue, *targetReg, r * 2);                  
            },
            [targetReg]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                c.addi(*targetReg, *targetReg, 64 * numUnrolls);
            });
    }

    void generateDenseRow(std::shared_ptr<const EventPropagationProcess> process,
                          ScalarRegisterAllocator::RegisterPtr targetReg,
                          ScalarRegisterAllocator::RegisterPtr weightBufferReg)
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        ALLOCATE_VECTOR(VWeight);
        ALLOCATE_VECTOR(VTarget1);
        ALLOCATE_VECTOR(VTarget2);
        ALLOCATE_VECTOR(VTargetNew);

        // Preload first ISyn to avoid stall
        c.vloadv(*VTarget1, *targetReg, 0);

        AssemblerUtils::unrollVectorLoopBody(
            c, scalarRegisterAllocator, process->getNumTargetNeurons(), 4, *targetReg,
            [&targetReg, weightBufferReg, VWeight, VTarget1, VTarget2, VTargetNew]
            (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr maskReg)
            {
                // Load vector of weights
                c.vloadv(*VWeight, *weightBufferReg, r * 64);

                // Load NEXT vector of target to avoid stall
                // **YUCK** in last iteration, while this may not be accessed, it may be out of bounds                  
                c.vloadv(even ? *VTarget2 : *VTarget1, *targetReg, (r + 1) * 64);
            
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
                c.vstore(*VTarget, *targetReg, r * 64);
            },
            [&targetReg, weightBufferReg]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                // Increment pointers 
                c.addi(*targetReg, *targetReg, 64 * numUnrolls);
                c.addi(*weightBufferReg, *weightBufferReg, 64 * numUnrolls);
            });
    }

    void generateSparseDelayRow(std::shared_ptr<const EventPropagationProcess> process,
                                VectorRegisterAllocator::RegisterPtr targetDelayMaskReg, 
                                VectorRegisterAllocator::RegisterPtr targetAddrReg,
                                VectorRegisterAllocator::RegisterPtr vectorTimeReg,
                                ScalarRegisterAllocator::RegisterPtr weightBufferReg)
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        // Loop over postsynaptic neurons
        ALLOCATE_VECTOR(VAccum)
        ALLOCATE_VECTOR(VWeightInd1);
        ALLOCATE_VECTOR(VWeightInd2);
        ALLOCATE_VECTOR(VPostAddr);
        ALLOCATE_VECTOR(VWeight);

        // Preload first weights and indices to avoid stall
        c.vloadv(*VWeightInd1, *weightBufferReg, 0);
        
        AssemblerUtils::unrollVectorLoopBody(
            c, scalarRegisterAllocator, process->getMaxRowLength(), 4, *weightBufferReg,
            [process, targetDelayMaskReg, targetAddrReg, vectorTimeReg, weightBufferReg, 
             VAccum, VPostAddr, VWeight, VWeightInd1, VWeightInd2]
            (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr)
            {
                // Load NEXT vector of weights and indices
                c.vloadv(even ? *VWeightInd2 : *VWeightInd1, *weightBufferReg, (r + 1) * 64);

                // Extract postsynaptic index/delay
                auto VWeightInd = even ? VWeightInd1 : VWeightInd2;
                c.vand(*VPostAddr, *VWeightInd, *targetDelayMaskReg);
                
                // If process has delays
                if(process->getNumDelayBits() > 0) {
                    // Add time to delay
                    c.vadd(*VPostAddr, *VPostAddr, *vectorTimeReg);

                    // Take modulo max delay
                    c.vand(*VPostAddr, *VPostAddr, *targetDelayMaskReg);
                }

                // Add base address
                c.vadd(*VPostAddr, *VPostAddr, *targetAddrReg); 
                
                // Load accumulator
                c.vloadl(*VAccum, *VPostAddr);
                
                // Extract weight
                const size_t numBits = ((process->getNumSparseConnectivityBits() > 0) 
                                        ? process->getNumSparseConnectivityBits()
                                        : process->getNumDelayBits());
                c.vsrai(numBits, *VWeight, *VWeightInd);

                // Add weights to accumulator loaded in previous iteration
                c.vadd_s(*VAccum, *VAccum, *VWeight);

                // Write back accumulator
                c.vstorel(*VAccum, *VPostAddr);
            },
            [weightBufferReg]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                // Increment pointers 
                c.addi(*weightBufferReg, *weightBufferReg, 64 * numUnrolls);
            });
    }

    void generateURAMWordLoop(const std::vector<std::shared_ptr<const EventPropagationProcess>> &processes,
                              const EventPropagationProcessRegisters &processRegs, ScalarRegisterAllocator::RegisterPtr eventBufferReg, 
                              ScalarRegisterAllocator::RegisterPtr eventBufferEndReg, VectorRegisterAllocator::RegisterPtr vectorTimeReg)
    {
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        ALLOCATE_SCALAR(SWordNStart);
        ALLOCATE_SCALAR(SConst1);
        ALLOCATE_SCALAR(SEventWord);

        // Labels
        Label wordLoop;
        Label bitLoopStart;
        Label bitLoopBody;
        Label bitLoopEnd;
        Label zeroSpikeWord;
        Label wordEnd;

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

                // Loop through postsynaptic targets
                const auto &processFields = m_Model.get().getStatefulFields();
                for(size_t i = 0; i < processes.size(); i++) {
                    const auto p = processes[i];
                    const auto &stateFields = processFields.at(p);

                    // Get previously allocated registers
                    auto [strideReg, sparseMaskReg, targetAddrReg] = processRegs[i];

                    // If process is dense, allocate target register and load
                    // **YUCK** it would be nice to put this at the start of generateDenseRow
                    // but first instruction requires targetReg so this would stall
                    ScalarRegisterAllocator::RegisterPtr targetReg;
                    const bool sparseOrDelay = (p->getNumSparseConnectivityBits() > 0 
                                                || p->getNumDelayBits() > 0);
                    if(!sparseOrDelay) {
                        targetReg = scalarRegisterAllocator.getRegister("STargetBuffer = X");
                        c.lw(*targetReg, Reg::X0, stateFields.at(p->getTarget()));
                    }

                    // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
                    ALLOCATE_SCALAR(SWeightBuffer);
                    c.lw(*SWeightBuffer, Reg::X0, stateFields.at(p->getWeight()));
                    {
                        ALLOCATE_SCALAR(STemp);
                        c.mul(*STemp, *SN, *strideReg);
                        c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
                    }

                    if(sparseOrDelay) {
                        generateSparseDelayRow(p, sparseMaskReg, targetAddrReg, 
                                               vectorTimeReg, SWeightBuffer);
                    }
                    else {
                        generateDenseRow(p, targetReg, SWeightBuffer);
                    }
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

    /*void generateDRAMWordLoop(const std::vector<std::shared_ptr<const EventPropagationProcess>> &processes,
                              const EventPropagationProcessRegisters &processRegs, ScalarRegisterAllocator::RegisterPtr eventBufferReg, 
                              ScalarRegisterAllocator::RegisterPtr eventBufferEndReg, VectorRegisterAllocator::RegisterPtr vectorTimeReg)
    {
        // Make some friendlier-named references
        const auto &processFields = m_Model.get().getProcessFields();
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        ALLOCATE_SCALAR(SWordNStart);
        ALLOCATE_SCALAR(SConst1);
        ALLOCATE_SCALAR(SEventWord);
        ALLOCATE_SCALAR(SRowBufferA);
        ALLOCATE_SCALAR(SRowBufferB);

        // Labels
        Label wordLoop;
        Label bitLoopStart;
        Label bitLoopBody;
        Label bitLoopEnd;
        Label prefetchBody;
        Label zeroSpikeWord;
        Label wordEnd;

        // Load row buffer pointers
        c.li(*SRowBufferA, rowBufferAPtr);
        c.li(*SRowBufferB, rowBufferBPtr);
    
        // Load some useful constants
        c.li(*SConst1, 1);

        // SWordNStart = 31
        c.li(*SWordNStart, 31);
        
        // Outer word loop
        c.L(wordLoop);
        {
            // Register allocation
            ALLOCATE_SCALAR(SN);
            ALLOCATE_SCALAR(SNumLZ);

            {
                ALLOCATE_SCALAR(SEventWordTemp);

                // SEventWordTemp = *eventBufferReg++
                c.lw(*SEventWordTemp, *eventBufferReg);
                c.addi(*eventBufferReg, *eventBufferReg, 4);

                // If SSpikeWord == 0, goto bitloop end
                c.beq(*SEventWordTemp, Reg::X0, bitLoopEnd);

                // SN = SWordNStart
                c.mv(*SN, *SWordNStart);

                // CNumLZ = clz(SSpikeWord);
                c.clz(*SNumLZ, *SEventWordTemp);

                // If SSpikeWord == 1  i.e. CNumLZ == 31, goto prefetch body, using zero as new spike word
                c.li(*SEventWord, 0);
                c.beq(*SEventWordTemp, *SConst1, prefetchBody);
        
                {
                    // STmp = CNumLZ + 1
                    ALLOCATE_SCALAR(STmp);
                    c.addi(*STmp, *SNumLZ, 1);

                    // SSpikeWord <<= CNumLZPlusOne
                    c.sll(*SEventWord, *SEventWordTemp, *STmp);
                }

            }
            // SN -= SNumLZ
            c.L(prefetchBody);
            c.sub(*SN, *SN, *SNumLZ);

            // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
            // **TODO** use process[0]
            {
                ALLOCATE_SCALAR(STemp);
                c.mul(*STemp, *SN, *SStride);
                c.add(*STemp, *weightBaseAddress, *STemp);

                // Start DMA write into RowBufferA
                AssemblerUtils::generateDMAStartWrite(c, *SRowBufferA, *STemp, *SStride);
            }

            // SN --
            c.addi(*SN, *SN, -1);
       
            // If we've now got no spikes to process skip to tail
            c.beq(*SEventWord, Reg::X0, wordEnd);

            // Inner bit loop
            c.L(bitLoopStart);
            {
                // CNumLZ = clz(SEventWord);
                c.clz(*SNumLZ, *SEventWord);

                // If SEventWord == 1  i.e. CNumLZ == 31, goto zeroSpikeWord
                c.beq(*SEventWord, *SConst1, zeroSpikeWord);
            
                {
                    // STmp = CNumLZ + 1
                    ALLOCATE_SCALAR(STmp);
                    c.addi(*STmp, *SNumLZ, 1);

                    // SEventWord <<= CNumLZPlusOne
                    c.sll(*SEventWord, *SEventWord, *STmp);
                }

                // SN -= SNumLZ
                c.L(bitLoopBody);
                c.sub(*SN, *SN, *SNumLZ);

                // Loop through postsynaptic targets
                for(size_t i = 0; i < processes.size(); i++) {
                    const auto p = processes[i];
                    const auto &stateFields = processFields.at(p);

                    // Get previously allocated registers
                    auto [strideReg, sparseMaskReg, targetAddrReg] = processRegs[i];

                    // If process is dense, allocate target register and load
                    // **YUCK** it would be nice to put this at the start of generateDenseRow
                    // but first instruction requires targetReg so this would stall
                    ScalarRegisterAllocator::RegisterPtr targetReg;
                    const bool sparseOrDelay = (p->getNumSparseConnectivityBits() > 0 
                                                || p->getNumDelayBits() > 0);
                    if(!sparseOrDelay) {
                        targetReg = scalarRegisterAllocator.getRegister("STargetBuffer = X");
                        c.lw(*targetReg, Reg::X0, stateFields.at(p->getTarget()));
                    }

                    // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
                    {
                        ALLOCATE_SCALAR(STemp);

                        c.mul(*STemp, *SN, *strideReg);
                        c.add(*STemp, *weightBaseAddress, *STemp);

                        // Wait for previous DMA write to completet
                        AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);

                        // Start DMA write into RowBufferB
                        AssemblerUtils::generateDMAStartWrite(c, *SRowBufferB, *STemp, *strideReg);
                    }

                    if(sparseOrDelay) {
                        generateSparseDelayRow(p, sparseMaskReg, targetAddrReg, 
                                               vectorTimeReg, SRowBufferA);
                    }
                    else {
                        generateDenseRow(p, targetReg, SRowBufferA);
                    }

                    // Swap buffers
                    {
                        ALLOCATE_SCALAR(STmp);
                        c.mv(*STmp, *SRowBufferA);
                        c.mv(*SRowBufferA, *SRowBufferB);
                        c.mv(*SRowBufferB, *STmp);
                    }
                }
            
                 // SN --
                c.addi(*SN, *SN, -1);
            
                // If SSpikeWord != 0, goto bitLoopStart
                c.bne(*SEventWord, Reg::X0, bitLoopStart);

                // Goto wordEnd
                //c.j_(wordEnd);
                c.beq(Reg::X0, Reg::X0, wordEnd);
            }

            // Zero spike word
            {
                c.L(zeroSpikeWord);
                c.li(*SEventWord, 0);
                //c.j_(bitLoopBody);
                c.beq(Reg::X0, Reg::X0, bitLoopBody);
            }

            c.L(wordEnd);

            // Wait for final DMA write to completet
            AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);

            // Process final row
            {
                const auto p = processes.back();
                const auto &stateFields = processFields.at(p);

                // Get previously allocated registers
                auto [strideReg, sparseMaskReg, targetAddrReg] = processRegs[i];

                // If process is dense, allocate target register and load
                // **YUCK** it would be nice to put this at the start of generateDenseRow
                // but first instruction requires targetReg so this would stall
                ScalarRegisterAllocator::RegisterPtr targetReg;
                const bool sparseOrDelay = (p->getNumSparseConnectivityBits() > 0 
                                            || p->getNumDelayBits() > 0);
                if(!sparseOrDelay) {
                    targetReg = scalarRegisterAllocator.getRegister("STargetBuffer = X");
                    c.lw(*targetReg, Reg::X0, stateFields.at(p->getTarget()));
                }

                if(sparseOrDelay) {
                    generateSparseDelayRow(p, sparseMaskReg, targetAddrReg, 
                                            vectorTimeReg, SRowBufferA);
                }
                else {
                    generateDenseRow(p, targetReg, SRowBufferA);
                }
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
    }*/

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
                    throw std::runtime_error("Event containers need to have a buffer");
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
        }

        // Loop through postsynaptic targets
        EventPropagationProcessRegisters sProcessRegs;
        for(const auto &p : processes) {
            // Allocate register for stride, calculate and load as immediate
            auto strideReg = scalarRegisterAllocator.getRegister("SStride = X");
            c.li(*strideReg, ceilDivide(p->getMaxRowLength(), 32) * 64);

            // If process has sparse connectivity or delays
            VectorRegisterAllocator::RegisterPtr targetDelayMaskReg;
            VectorRegisterAllocator::RegisterPtr targetAddrReg;
            if(p->getNumSparseConnectivityBits() > 0 || p->getNumDelayBits() > 0) {
                // Allocate register for target/delay mask and calculate
                targetDelayMaskReg = vectorRegisterAllocator.getRegister("VTargetDelayMask V");
                const size_t numBits = ((p->getNumSparseConnectivityBits() > 0) 
                                        ? p->getNumSparseConnectivityBits()
                                        : p->getNumDelayBits());
                c.vlui(*targetDelayMaskReg, (1 << numBits) - 1);

                // Allocate register for target address
                // Generate code to load address
                ALLOCATE_SCALAR(STmp);
                targetAddrReg = vectorRegisterAllocator.getRegister("VTargetAddr V");
                c.lw(*STmp, Reg::X0, processFields.at(p).at(p->getTarget()));
                c.vfill(*targetAddrReg, *STmp);
            }

            // Add register pointers to vector
            sProcessRegs.emplace_back(strideReg, targetDelayMaskReg, targetAddrReg);
        }
    
        // Generate correct loop depending on whether weights are in DRAM or URAM
        if(m_UseDRAMForWeights) {
            assert(false);
            //generateDRAMWordLoop(processes, sProcessRegs, SEventBuffer, SEventBufferEnd, vectorTimeReg);
        }
        else {
            generateURAMWordLoop(processes, sProcessRegs, SEventBuffer, SEventBufferEnd, vectorTimeReg);
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

//--------------------------------------------------------------------------
// ArrayBase
//--------------------------------------------------------------------------
void ArrayBase::memsetHostPointer(int value)
{
    std::memset(m_HostPointer, value, getSizeBytes());
}

//------------------------------------------------------------------------
// URAMArrayBase
//------------------------------------------------------------------------
void URAMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy URAM pointer into bytes
    const uint32_t uramPointer = m_URAMPointer.value(); 
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
    const uint32_t bramPointer = m_BRAMPointer.value();
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
    const uint32_t llmPointer = m_LLMPointer.value() / 32;
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
    const uint32_t dramPointer = m_DRAMPointer.value(); 
    std::memcpy(bytes.data(), &dramPointer, 4);
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
            Label timeLoop;
            
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
    const size_t count = std::accumulate(varDims.cbegin(), varDims.cend(), 
                                         1, std::multiplies<size_t>()) * variable->getNumBufferTimesteps();

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