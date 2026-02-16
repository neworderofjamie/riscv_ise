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

// Model includes
#include "model/event_container.h"
#include "model/process.h"
#include "model/process_group.h"
#include "model/variable.h"

// Compiler includes
#include "fenn/compiler/compiler.h"

// Backend includes
#include "fenn/backend/environment.h"
#include "fenn/backend/memory_allocator.h"
#include "fenn/backend/special_function.h"

using namespace FeNN;
using namespace Model;
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

void addStochMulFunctions(Backend::EnvironmentLibrary::Library &library) 
{
    using namespace Assembler;

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

void updateLiteralPool(const std::vector<Token> &tokens, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
                       std::unordered_map<int16_t, Assembler::VectorRegisterAllocator::RegisterPtr> &literalPool)
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
                       const std::unordered_map<int16_t, Assembler::VectorRegisterAllocator::RegisterPtr> &literalPool,
                       TypeChecker::EnvironmentInternal &typeCheckEnv, Compiler::EnvironmentInternal &compilerEnv,
                       ErrorHandler &errorHandler, Transpiler::TypeChecker::StatementHandler forEachSynapseTypeCheckHandler,
                       Assembler::ScalarRegisterAllocator::RegisterPtr maskRegister, Compiler::RoundingMode roundingMode,
                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, Assembler::VectorRegisterAllocator &vectorRegisterAllocator)
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
// LUTVisitor
//----------------------------------------------------------------------------
/*class LUTVisitor : public ModelComponentVisitor
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
};*/

class PerformanceCounterScope
{
public:
    PerformanceCounterScope(std::shared_ptr<const ::Model::ProcessGroup> processGroup,
                            Assembler::CodeGenerator &codeGenerator,
                            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                            const ::Model::Model &model)
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
        using namespace FeNN::Common;
        using namespace Assembler;

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
            Assembler::Utils::generateSubtractUint64(c, scalarRegisterAllocator,
                                                    *SCountCycles, *SCountCyclesH,
                                                    *startCyclesReg, *startCyclesHighReg);
            Assembler::Utils::generateSubtractUint64(c, scalarRegisterAllocator,
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
            Assembler::Utils::generateAddUint64(c, scalarRegisterAllocator,
                                                *startCyclesReg, *startCyclesHighReg,
                                                *SCountCycles, *SCountCyclesH);
            Assembler::Utils::generateAddUint64(c, scalarRegisterAllocator,
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
    std::shared_ptr<const ::Model::ProcessGroup> m_ProcessGroup;
    std::reference_wrapper<Assembler::CodeGenerator> m_CodeGenerator;
    std::reference_wrapper<Assembler::ScalarRegisterAllocator> m_ScalarRegisterAllocator;
    std::reference_wrapper<const ::Model::Model> m_Model;

    Assembler::ScalarRegisterAllocator::RegisterPtr startCyclesReg;
    Assembler::ScalarRegisterAllocator::RegisterPtr startCyclesHighReg;
    Assembler::ScalarRegisterAllocator::RegisterPtr startInstructsReg;
    Assembler::ScalarRegisterAllocator::RegisterPtr startInstructsHighReg;
};
/*class CodeGeneratorVisitor : public ModelComponentVisitor
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
        
        
    }

    virtual void visit(std::shared_ptr<const RNGInitProcess> rngInitProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        
    }

    virtual void visit(std::shared_ptr<const BroadcastProcess> broadcastProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        
    }

    virtual void visit(std::shared_ptr<const MemsetProcess> memsetProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        
    }

    virtual void visit(std::shared_ptr<const EventPropagationProcess> eventPropagationProcess)
    {
        // Add event propagation process to vector of processes with same input event container
        m_EventPropagationProcesses[eventPropagationProcess->getInputEvents()].emplace_back(eventPropagationProcess);
    }

    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    
    

    void generateEventPropagationProcesses(const std::vector<std::shared_ptr<const EventPropagationProcess>> &processes)
    {
        
        
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
};*/
}

//------------------------------------------------------------------------
// URAMArrayBase
//------------------------------------------------------------------------
namespace FeNN::Backend
{
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
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const Variable> variable,
                                                    const ::Model::Model::StateProcesses::mapped_type &processes,
                                                    StateBase *state) const
{
    // Pad last dimension to multiplies of 32
    // **THINK** how much of this belongs in Shape?
    // **TODO** more information is required here to seperate variables with
    // shape (B,) which shouldn't be padded from (N,) variables which should
    auto varDims = variable->getShape().getDims();
    varDims.back() = ::Common::Utils::padSize(varDims.back(), 32);

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
                                                    const ::Model::Model::StateProcesses::mapped_type&,
                                                    StateBase *state) const
{
    LOGI << "Creating event container '" << eventContainer->getName() << "' array in BRAM";

    // Event containers are always implemented as BRAM bitfields
    const size_t numSpikeWords = ::Common::Utils::ceilDivide(eventContainer->getShape().getFlattenedSize(), 32) * eventContainer->getNumBufferTimesteps();
    return state->createBRAMArray(GeNN::Type::Uint32, numSpikeWords);
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const PerformanceCounter> performanceCounter,
                                                    const ::Model::Model::StateProcesses::mapped_type&,
                                                    StateBase *state) const
{
    LOGI << "Creating performance counter '" << performanceCounter->getName() << "' array in BRAM";

    // Performance counter contains a 64-bit number for 
    // instructions retired and one for number of cycles 
    return state->createBRAMArray(GeNN::Type::Uint64, 2);
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const ::Model::State> state, 
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
std::unordered_map<int, std::shared_ptr<::Model::State>> BackendFeNN::getRequiredStateObjects(const ::Model::Model &model) const
{
    // If we should use DRAM for weights, we require two row buffers
    std::unordered_map<int, std::shared_ptr<::Model::State>> stateObjects;
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
}