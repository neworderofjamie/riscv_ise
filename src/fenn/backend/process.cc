#include "fenn/backend/process.h"

// Standard C++ includes
#include <algorithm>
#include <numeric>

// Compiler frontend includes
#include "compiler_frontend/type.h"
#include "compiler_frontend/error_handler.h"
#include "compiler_frontend/parser.h"
#include "compiler_frontend/type_checker.h"

// Common include
#include "common/utils.h"

// Frontend includes
#include "frontend/model.h"
#include "frontend/shape.h"

// FeNN common includes
#include "fenn/common/isa.h"

// FeNN Assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/assembler_utils.h"
#include "fenn/assembler/register_allocator.h"

// FeNN backend includes
#include "fenn/backend/environment.h"
#include "fenn/backend/events.h"
#include "fenn/backend/fields.h"
#include "fenn/backend/model.h"
#include "fenn/backend/runtime.h"
#include "fenn/backend/special_function.h"
#include "fenn/backend/variable.h"

using namespace FeNN;
using namespace FeNN::Backend;
using namespace CompilerFrontend;

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
using ScalarConstant = std::variant<Assembler::ScalarRegisterPtr, int, std::monostate>;

int64_t getVectorLiteralValue(const Frontend::Literals::value_type &literal)
{
    // Get literal value for this process
    const auto &numericType = std::get<0>(literal).getNumeric();

    // Convert to integer
    int64_t integerResult;
    if(numericType.isIntegral) {
        integerResult = std::get<1>(literal).cast<int64_t>();
    }
    // Otherwise, if it is fixed point
    else if(numericType.fixedPoint) {
        integerResult = static_cast<int64_t>(
            std::round(std::get<1>(literal).cast<double>() * (1u << numericType.fixedPoint.value())));
    }
    else {
        throw std::runtime_error("FeNN does not support floating point types");
    }

    // Check integer value can fit within 16-bit signed type
    if(integerResult < std::numeric_limits<int16_t>::min() 
        || integerResult > std::numeric_limits<int16_t>::max())
    {
        throw std::runtime_error("Literal out of range for type '" + std::get<0>(literal).getName() + "'");
    }

    return integerResult;
}

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
                                   env.getCodeGenerator().vmul_rs(shift, *result, *std::get<Assembler::VectorRegisterPtr>(args[0]),
                                                                  *std::get<Assembler::VectorRegisterPtr>(args[1]));
                                   return std::make_pair(result, true);
                               }));
            library.emplace(
                "mul_rs",
                std::make_pair(Type::ResolvedType::createFunction(resultTypeSat, {aTypeSat, bTypeSat}),
                               [shift](auto &env, auto &vectorRegisterAllocator, auto &, auto, const auto &args)
                               {
                                   auto result = vectorRegisterAllocator.getRegister();
                                   env.getCodeGenerator().vmul_rs(shift, *result, *std::get<Assembler::VectorRegisterPtr>(args[0]),
                                                                  *std::get<Assembler::VectorRegisterPtr>(args[1]));
                                   return std::make_pair(result, true);
                               }));
        }
    }
}

bool isExpCalled(const std::vector<Token> &tokens)
{
    // Loop through tokens
    for(auto t = tokens.cbegin(); t != tokens.cend(); t++) {
        // If token is an identifier with correct name
        if(t->type == Token::Type::IDENTIFIER && (t->lexeme == "exp" || t->lexeme.rfind("exp_") == 0)) {
            // If token isn't last in sequence and it's followed by a left bracket
            const auto tNext = std::next(t);
            if(tNext != tokens.cend() && tNext->type == Token::Type::LEFT_PAREN) {
                return true;
            }
        }
    }

    return false;
}

void compileStatements(const std::vector<Token> &tokens, TypeChecker::EnvironmentInternal &typeCheckEnv, 
                       Compiler::EnvironmentInternal &compilerEnv, ErrorHandler &errorHandler, 
                       TypeChecker::StatementHandler forEachSynapseTypeCheckHandler,
                       Assembler::ScalarRegisterPtr maskRegister, Compiler::RoundingMode roundingMode,
                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, Assembler::VectorRegisterAllocator &vectorRegisterAllocator)
{


    // Parse tokens as block item list (function body)
    auto updateStatements = Parser::parseBlockItemList(tokens, errorHandler);
    if(errorHandler.hasError()) {
        throw std::runtime_error("Parse error " + errorHandler.getContext());
    }

    // Resolve types
    auto resolvedTypes = TypeChecker::typeCheck(updateStatements, typeCheckEnv, errorHandler, 
                                                forEachSynapseTypeCheckHandler);
    if(errorHandler.hasError()) {
        throw std::runtime_error("Type check error " + errorHandler.getContext());
    }

    // Compile
    compile(updateStatements, compilerEnv, resolvedTypes, errorHandler, maskRegister, 
            roundingMode, scalarRegisterAllocator, vectorRegisterAllocator);
    if(errorHandler.hasError()) {
        throw std::runtime_error("Compiler error " + errorHandler.getContext());
    }
}

void unrollVectorLoopBody(Assembler::CodeGenerator &c, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                          ScalarConstant numElements, uint32_t maxUnroll, bool noTail, bool noUnroll,
                          std::function<void(Assembler::CodeGenerator&, uint32_t, Assembler::ScalarRegisterPtr)> genBodyFn, 
                          std::function<void(Assembler::CodeGenerator&, uint32_t)> genTailFn)
{
    std::visit(
        ::Common::Utils::Overload{
            // Compile-time literal
            [&c, &genBodyFn, &genTailFn, &scalarRegisterAllocator, maxUnroll]
            (int numElements)
            {
                Assembler::Utils::unrollVectorLoopBody(c, scalarRegisterAllocator, numElements, maxUnroll,
                                                       genBodyFn, genTailFn);
            },
            [&c, &genBodyFn, &genTailFn, &scalarRegisterAllocator, maxUnroll, noTail, noUnroll]
            (Assembler::ScalarRegisterPtr numElements)
            {
                Assembler::Utils::unrollVectorLoopBody(c, scalarRegisterAllocator,
                                                       *numElements, maxUnroll, noTail, noUnroll,
                                                       genBodyFn, genTailFn);
            },
            [](std::monostate)
            {
                throw std::runtime_error("Empty loop count");
            }},
        numElements);
}

template<typename P>
bool isHeterogeneous(const Frontend::MergedProcess &mergedProcess, size_t numDevices,
                     MergedFields::GetFieldConstantFunc<P> getFieldValueFn)
{
    // Get field value for archetype on first device
    const auto archetypeValue = getFieldValueFn(0, mergedProcess.getArchetype<P>());

    // Loop through each device and each process and determine if any values differ
    bool heterogeneous = false;
    for(size_t d = 0; d < numDevices; d++) {
        mergedProcess.forEachMerged<P>(
            [&archetypeValue, &getFieldValueFn, &heterogeneous, d]
            (auto p)
            {
                if(getFieldValueFn(d, p) != archetypeValue) {
                    heterogeneous = true;
                }
            });
    }

    return heterogeneous;
}

template<typename P, typename F>
bool allOf(const Frontend::MergedProcess &mergedProcess, size_t numDevices,
           MergedFields::GetFieldConstantFunc<P> getFieldValueFn,
           F unaryPredicateFn)
{
    // Loop through devices and processes
    // **YUCK** nov
    bool value = true;
    for (size_t d = 0; d < numDevices; d++) {
        mergedProcess.forEachMerged<P>(
            [&getFieldValueFn, &unaryPredicateFn, &value, d]
            (auto p)
            {
                // If predicate is false then return false
                if(!unaryPredicateFn(getFieldValueFn(d, p))) {
                    value = false;
                }
            });
    }

    return value;
    
}

template<typename P>
Assembler::VectorRegisterPtr addVectorConstant(const Frontend::MergedProcess &mergedProcess, MergedFields &mergedFields,
                                               size_t numDevices, Assembler::ScalarRegisterPtr fieldBaseReg,
                                               Assembler::CodeGenerator &processCodeGenerator, Assembler::CodeGenerator &sharedCodeGenerator,
                                               Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
                                               std::vector<Compiler::RegisterPtr> &sharedRegisters, MergedFields::GetFieldConstantFunc<P> getFieldValueFn)
{
    // If value is heterogeneous, add field
    if(isHeterogeneous(mergedProcess, numDevices, getFieldValueFn)) {
        // Add field
        const uint32_t fieldOffset = mergedFields.addField<P>(getFieldValueFn);

        // Allocate register
        ALLOCATE_SCALAR(SReg);
        ALLOCATE_VECTOR(VReg);

        // Load value into register and fill vector register
        processCodeGenerator.lw(*SReg, *fieldBaseReg, fieldOffset);
        processCodeGenerator.vfill(*VReg, *SReg);
        return VReg;
    }
    // Otherwise
    else {
        // Convert homogeneous value to int
        const int value = std::visit([](auto v){ return static_cast<int>(v); },
                                     getFieldValueFn(0, mergedProcess.getArchetype<P>()));

        // If the value fits within the max literal bits, add scalar literal
        if(!FeNN::Common::inSBit(value, 16)) {
            throw std::runtime_error("Vector literal out of range");
        }

        // Allocate register
        ALLOCATE_VECTOR(VReg);

        // Load shared immediate
        sharedCodeGenerator.vlui(*VReg, value);

        // Add register to vector of shared registers
        sharedRegisters.push_back(VReg);

        return VReg;
    }
}

template<typename P>
ScalarConstant addScalarValue(int maxBits, const Frontend::MergedProcess &mergedProcess, size_t numDevices,
                              MergedFields &mergedFields, Assembler::ScalarRegisterPtr fieldBaseReg,
                              Assembler::CodeGenerator &processCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                              MergedFields::GetFieldConstantFunc<P> getFieldValueFn)
{
    // Convert homogeneous value to int
    const int archetypeValue = std::visit([](auto v){ return static_cast<int>(v); },
                                          getFieldValueFn(0, mergedProcess.getArchetype<P>()));

    // If value is heterogeneous
    if(isHeterogeneous(mergedProcess, numDevices, getFieldValueFn)) {
        // Add field
        const uint32_t fieldOffset = mergedFields.addField<P>(getFieldValueFn);

        // Allocate register
        ALLOCATE_SCALAR(SReg);

        // Load value into register
        processCodeGenerator.lw(*SReg, *fieldBaseReg, fieldOffset);

        return SReg;
    }
    // Otherwise, if it fits in max bits, return value
    else if (FeNN::Common::inSBit(archetypeValue, maxBits)) {
        return archetypeValue;
    }
    // Otherwise
    else {
        // Allocate register
        ALLOCATE_SCALAR(SReg);

        // Load immediate
        processCodeGenerator.li(*SReg, archetypeValue);

        // Return
        return SReg;
    }
}

template<typename P>
ScalarConstant addScalarValue(int maxBits, const Frontend::MergedProcess &mergedProcess, size_t numDevices,
                              MergedFields &mergedFields, Assembler::ScalarRegisterPtr fieldBaseReg,
                              Assembler::CodeGenerator &processCodeGenerator, Assembler::CodeGenerator &sharedCodeGenerator,
                              Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, std::vector<Compiler::RegisterPtr> &sharedRegisters, 
                              MergedFields::GetFieldConstantFunc<P> getFieldValueFn)
{
    // If value is heterogeneous
    if(isHeterogeneous(mergedProcess, numDevices, getFieldValueFn)) {
        // Add field
        const uint32_t fieldOffset = mergedFields.addField<P>(getFieldValueFn);

        // Allocate register
        ALLOCATE_SCALAR(SReg);
    
        // Load value into register
        processCodeGenerator.lw(*SReg, *fieldBaseReg, fieldOffset);

        return SReg;
    }
    // Otherwise
    else {
        // Convert homogeneous value to int
        const int value = std::visit([](auto v){ return static_cast<int>(v); },
                                     getFieldValueFn(0, mergedProcess.getArchetype<P>()));

        // If the value fits within the max literal bits, add scalar literal
        if(FeNN::Common::inSBit(value, maxBits)) {
            return value;
        }
        // Otherwise
        else {
            // Allocate register
            ALLOCATE_SCALAR(SReg);

            // Load shared immediate
            sharedCodeGenerator.li(*SReg, value);

            // Add register to vector of shared registers
            sharedRegisters.push_back(SReg);

            return SReg;
        }
    }
}
}
//----------------------------------------------------------------------------
// FeNN::Backend::TimeDrivenProcessImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
void TimeDrivenProcessImplementation::generateCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                                   const KernelImplementation &kernel, MergedFields &mergedFields, 
                                                   Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg, 
                                                   Assembler::ScalarRegisterPtr groupIndReg, std::optional<uint32_t> numTimesteps, 
                                                   uint32_t &fieldBase, Assembler::CodeGenerator &c, 
                                                   Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                                   Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Allocate base register
    ALLOCATE_SCALAR(SFieldBase);
    ALLOCATE_SCALAR(SFieldBaseEnd);

    // Check presynaptic or group index registers are not set at this point
    assert(!preIndReg);
    assert(!groupIndReg);

    // Generate archetype code and populate merged fields
    Assembler::CodeGenerator archetypeCodeGenerator;
    const auto sharedRegisters = generateArchetypeCode(mergedProcess, runtime, kernel, mergedFields,
                                                       SFieldBase, timeReg, numTimesteps, archetypeCodeGenerator,
                                                       c, scalarRegisterAllocator, vectorRegisterAllocator);

    // Load fieldBase
    c.li(*SFieldBase, fieldBase);

    // Calculate fieldEnd and load if required
    fieldBase += (mergedProcess.getMerged().size() * mergedFields.getSize());
    if(mergedProcess.getMerged().size() > 1) {
        c.li(*SFieldBaseEnd, fieldBase);
    }

    // Generate loop over merged groups
    auto groupLoop = c.L();
    {
        // Insert generated code to simulate archetype
        c += archetypeCodeGenerator;

        // If a loop is required
        if(mergedProcess.getMerged().size() > 1) {
            // Advance to next group's fields
            c.addi(*SFieldBase, *SFieldBase, mergedFields.getSize());

            // Keep looping
            c.bne(*SFieldBase, *SFieldBaseEnd, groupLoop);
        }
    }
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventDrivenProcessImplementation
//----------------------------------------------------------------------------
void EventDrivenProcessImplementation::generateCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                                    const KernelImplementation &kernel, MergedFields &mergedFields, 
                                                    Assembler::ScalarRegisterPtr timeReg, Assembler::ScalarRegisterPtr preIndReg, 
                                                    Assembler::ScalarRegisterPtr groupIndReg, std::optional<uint32_t> numTimesteps, 
                                                    uint32_t &fieldBase, Assembler::CodeGenerator &c, 
                                                    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                                    Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Allocate base register
    ALLOCATE_SCALAR(SFieldBase);

    // Generate archetype code and populate merged fields
    Assembler::CodeGenerator archetypeCodeGenerator;
    generateArchetypeCode(mergedProcess, runtime, kernel, mergedFields, 
                          SFieldBase, timeReg,  preIndReg, numTimesteps, archetypeCodeGenerator,
                          scalarRegisterAllocator, vectorRegisterAllocator);

    // If there is only one merged group so fieldbase is used  
    // directly or fieldBase can't fit in an immediate, load it
    if(mergedProcess.getMerged().size() == 1 || !Common::inSBit(fieldBase, 12)) {
        c.li(*SFieldBase, fieldBase);
    }

    // If there is more than one merged group, calculate offset
    if(mergedProcess.getMerged().size() > 1) {
        ALLOCATE_SCALAR(STmp);
        c.li(*STmp, mergedFields.getSize());
        c.mul(*STmp, *STmp, *groupIndReg);

        // Either add fieldbase as an immediate or register
        if(Common::inSBit(fieldBase, 12)) {
            c.addi(*SFieldBase, *STmp, fieldBase);
        }
        else {
            c.add(*SFieldBase, *SFieldBase, *STmp);
        }
    }

    // Insert generated code to simulate archetype
    c += archetypeCodeGenerator;

    // Update fieldbase for next merged group
    fieldBase += (mergedProcess.getMerged().size() * mergedFields.getSize());
}

//----------------------------------------------------------------------------
// FeNN::Backend::NeuronUpdateProcess
//----------------------------------------------------------------------------
void NeuronUpdateProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const
{
    // Superclass
    Frontend::NeuronUpdateProcess::updateMergeHash(hash, model);

    // Loop through neuron variables and include hash of their memory spaces
    for(const auto &v : getVariables()) {
        ::Common::Utils::updateHash(
            static_cast<const Model&>(model).getStateMemSpace(v.second.getUnderlying(), 
                                                              true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
    }
}
//----------------------------------------------------------------------------
void NeuronUpdateProcess::updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                                   MemSpace &compatibleMemSpaces) const
{
    // Search variables for state
    const auto var = std::find_if(getVariables().cbegin(), getVariables().cend(),
                                  [&state](const auto &v){ return v.second.getUnderlying() == state; });
    if (var != getVariables().cend()) {
        compatibleMemSpaces &= (MemSpace::URAM | MemSpace::LLM);
    }
    // Otherwise
    else {
        // If state is an output event
        const auto outEvent = std::find_if(getOutputEventSinks().cbegin(), getOutputEventSinks().cend(),
                                           [&state](const auto &o){ return o.second.getUnderlying() == state; });
        assert (outEvent != getOutputEventSinks().cend());
    }
}
//----------------------------------------------------------------------------
/*void NeuronUpdateProcess::generateMergedPreambleCode(const Frontend::MergedProcess &mergedProcess,
                                                     const Runtime &runtime, EnvironmentExternal &environment, 
                                                     Assembler::CodeGenerator &c,
                                                     Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                     Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    const auto &archetypeLiterals = mergedProcess.getArchetype<NeuronUpdateProcess>()->getLiterals();

    // Loop through each merged process
    const size_t numLiterals = archetypeLiterals.size();

    // Start with each literal mapped to itself
    std::vector<size_t> literalMapping(numLiterals);
    std::iota(literalMapping.begin(), literalMapping.end(), 0);

    {
        // Create N*N binary matrix to mark literals whose value 
        // is the same another across all merged processes
        // **THINK** this could be stored in a more efficient row-major
        // triangular format but a) the indexing is a nuisance and
        // b) while number of merged could be large, number of literals 
        // is going to be limited (especially as we currently load them all into registers!)
        std::vector<bool> literalSelfSimilarity(numLiterals * numLiterals, true);

        // Loop through merged processed
        mergedProcess.forEachMerged<NeuronUpdateProcess>(
            [&literalSelfSimilarity, numLiterals](const auto &np)
            {
                // Update upper-triangular portion of matrix (excluding diagonal) with comparison
                assert(np->getLiterals().size() == numLiterals);
                for (size_t i = 0; i < numLiterals; i++) {
                    for (size_t j = (i + 1); j < numLiterals; j++) {
                        literalSelfSimilarity[(i * numLiterals) + j] &= (np->getLiterals()[i] == np->getLiterals()[j]);
                    }
                }
            });

        // Loop through upper-triangular portion of matrix (excluding diagonal)
        for (size_t i = 0; i < numLiterals; i++) {
            for (size_t j = (i + 1); j < numLiterals; j++) {
                // If this literal always has the same value as another, add to mapping
                if (literalSelfSimilarity[(i * numLiterals) + j]) {
                    literalMapping[i] = j;
                }
            }
        }
    }

    // Loop through literals shared across merged processes, check FeNN-compliance and load
    std::unordered_map<int16_t, Assembler::VectorRegisterPtr> literalPool;
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
}*/
//----------------------------------------------------------------------------
std::vector<Compiler::RegisterPtr> NeuronUpdateProcess::generateArchetypeCode(
        const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, const KernelImplementation &kernel, 
        MergedFields &mergedFields, Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg,
        std::optional<uint32_t> numTimesteps, Assembler::CodeGenerator &processCodeGenerator, 
        Assembler::CodeGenerator &sharedCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
        Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Define type for event-emitting function
    const auto emitEventFunctionType = Type::ResolvedType::createFunction(Type::Void, {});

    // Define lambda function to get number of neurons
    // **NOTE** this shouldn't include time dimension
    auto getNumNeurons =
        [&runtime](size_t d, auto p)
        {
            // Pick state object from process
            const auto state = (p->getVariables().empty()
                                ? std::static_pointer_cast<const Frontend::State>(p->getOutputEventSinks().begin()->second.getUnderlying())
                                : std::static_pointer_cast<const Frontend::State>(p->getVariables().begin()->second.getUnderlying()));

            // Get shape of array storing this state on device
            const auto shape = std::get<0>(runtime.getDeviceArrayShapeStrides(state, d));

            // Multiply together last dimensions of shape (determined based on sliced shape)
            return static_cast<uint32_t>(std::accumulate(shape.rbegin(), shape.rbegin() + p->getShape().size(), 
                                                         1, std::multiplies<size_t>()));
        };

    // Add number of neurons
    std::vector<Compiler::RegisterPtr> sharedRegisters;
    const auto numNeurons = addScalarValue<NeuronUpdateProcess>(
        31, mergedProcess, runtime.getNumDevices(), mergedFields, fieldBaseReg, 
        processCodeGenerator, sharedCodeGenerator, scalarRegisterAllocator, sharedRegisters,
        getNumNeurons);

    // No need for a tail if all neuron counts are multiples of 32
    const bool numNeuronsNoTail = allOf<NeuronUpdateProcess>(
        mergedProcess, runtime.getNumDevices(), getNumNeurons,
        [](const MergedFields::FieldValue &num)
        {
            return ((std::get<uint32_t>(num) % 32) == 0);
        });
    
    // No need for unrolling if all neuron counts are 
    // less than the size of a single unrolled iteration
    // **THINK** this could also trigger a reduction in maxUnroll
    const bool numNeuronsNoUnroll = allOf<NeuronUpdateProcess>(
        mergedProcess, runtime.getNumDevices(), getNumNeurons,
        [this](const MergedFields::FieldValue &num)
        {
            return (std::get<uint32_t>(num) < (getMaxUnroll() * 32));
        });

	const auto *model = runtime.getModel<Model>();
    std::unordered_map<std::shared_ptr<const Frontend::Variable>, std::vector<Compiler::RegisterPtr>> varState;
    {
        // If any variables have time dimension
        Assembler::ScalarRegisterPtr numVariableBytes;
        if (std::any_of(getVariables().cbegin(), getVariables().cend(),
                        [](const auto &e) { return e.second.hasTime(); }))
        {
            // Allocate register
            numVariableBytes = scalarRegisterAllocator.getRegister("SNumVariableBytes");

            // Calculate number of variable bytes from number of neurons
            // **NOTE** this could be slightly optimised by calculating in shared code generator if numNeurons is common 
            std::visit(
                ::Common::Utils::Overload{
                [&numVariableBytes, &processCodeGenerator](Assembler::ScalarRegisterPtr reg)
                {
                    //  ((numerator + 31) / 32) * 64;
                    processCodeGenerator.addi(*numVariableBytes, *reg, 31);
                    processCodeGenerator.srli(*numVariableBytes, *numVariableBytes, 5);
                    processCodeGenerator.slli(*numVariableBytes, *numVariableBytes, 6);
                },
                [&numVariableBytes, &processCodeGenerator](int val)
                {
                    processCodeGenerator.li(*numVariableBytes, ::Common::Utils::padSize(val, 32) * 2);
                },
                [](auto)
                {
                    assert(false);
                }},
                numNeurons);
        }


        // Loop through neuron variables
        for(const auto &v : getVariables()) {
            // Add field for variable to environment
            const auto &varName = v.first;
            const uint32_t varFieldOffset = mergedFields.addField<NeuronUpdateProcess>(
                [varName](const Frontend::DeviceBase &d, auto p)
                { 
                    return d.getArray(p->getVariables().at(varName).getUnderlying()); 
                });

            auto fennVar = std::dynamic_pointer_cast<const Variable>(v.second.getUnderlying());
            if (!fennVar) {
                throw std::runtime_error("FeNN backend used with incompatible variable");
            }

            // Generate preamble and add state to map
            varState.try_emplace(v.second.getUnderlying(), 
                                 fennVar->genPreamble(processCodeGenerator, scalarRegisterAllocator,
                                                      vectorRegisterAllocator, varFieldOffset, 
                                                      numTimesteps, fieldBaseReg, timeReg, numVariableBytes, 
                                                      v.second.hasTime(), *model));
        }
    }

    std::unordered_map<std::shared_ptr<const Frontend::EventSink>, 
                       std::vector<Assembler::ScalarRegisterPtr>> eventSinkState;
    {
        // If any output events have buffering, calculate stride in bytes
        // **TODO** incorrect - this is only for event sinks involvoing recording bits
        Assembler::ScalarRegisterPtr numEventBytes;
        if(std::any_of(getOutputEventSinks().cbegin(), getOutputEventSinks().cend(),
                       [](const auto e){ return e.second.hasTime(); }))
        {
            // Allocate register
            numEventBytes = scalarRegisterAllocator.getRegister("SNumEventBytes");

            // Calculate number of event bytes from number of neurons
            // **NOTE** result goes into MUL so has to be in a register
            // **NOTE** this could be slightly optimised by calculating in shared code generator if numNeurons is common 
            std::visit(
                ::Common::Utils::Overload{
                [&numEventBytes, &processCodeGenerator](Assembler::ScalarRegisterPtr reg)
                {
                    //  ((numerator + 31) / 32) * 4;
                    processCodeGenerator.addi(*numEventBytes, *reg, 31);
                    processCodeGenerator.srli(*numEventBytes, *numEventBytes, 5);
                    processCodeGenerator.slli(*numEventBytes, *numEventBytes, 2);
                }, 
                [&numEventBytes, &processCodeGenerator](int val)
                {
                    processCodeGenerator.li(*numEventBytes, ::Common::Utils::ceilDivide(val, 32) * 4);
                },
                [](auto)
                {
                    assert(false);
                }},
                numNeurons);
        }

        // Loop through event sinks attached to neuron population
        for(const auto &e : getOutputEventSinks()) {
            auto fennEventSink = std::dynamic_pointer_cast<const EventSinkImplementation>(e.second.getUnderlying());
            if (!fennEventSink) {
                throw std::runtime_error("FeNN backend used with incompatible event sink");
            }

            // Generate preamble and add state to map
            eventSinkState.try_emplace(
                e.second.getUnderlying(), 
                fennEventSink->genPreamble(
                    *model, kernel, processCodeGenerator, scalarRegisterAllocator, 
                    e.first, numTimesteps, e.second.hasTime(), runtime.getNumDevices(),
                    timeReg, numEventBytes,
                    [&fieldBaseReg, &mergedFields, &mergedProcess, &runtime, &scalarRegisterAllocator]
                    (Assembler::CodeGenerator &c, auto func)
                    {
                        return std::get<Assembler::ScalarRegisterPtr>(
                            addScalarValue<NeuronUpdateProcess>(0, mergedProcess, runtime.getNumDevices(), mergedFields,
                                                                fieldBaseReg, c, scalarRegisterAllocator, func));
                    },
                    [&e, &fieldBaseReg, &mergedFields, &scalarRegisterAllocator](Assembler::CodeGenerator &c)
                    {
                        // Add field
                        const auto &outputEventName = e.first;
                        const uint32_t fieldOffset = mergedFields.addField<NeuronUpdateProcess>(
                            [outputEventName](const Frontend::DeviceBase &d, auto p)
                            { 
                                return d.getArray(p->getOutputEventSinks().at(outputEventName).getUnderlying()); 
                            });

                        // Allocate scalar register to hold address of variable
                        const auto reg = scalarRegisterAllocator.getRegister((outputEventName + "Buffer X").c_str());

                        // Generate code to load address
                        c.lw(*reg, *fieldBaseReg, fieldOffset);

                        return reg;
                    }));
        }
    }
    // Create code generation environment
    EnvironmentExternal env(processCodeGenerator);

    // **YUCK** add vector register containing zero
    {
        // Allocate register
        ALLOCATE_VECTOR(VZero);

        // Load zero into register in shared code
        sharedCodeGenerator.vlui(*VZero, 0);

        // Add register to vector of shared registers
        sharedRegisters.push_back(VZero);

        // Add to environment
        env.add(Type::S8_7, "_zero", VZero);
    }
    
    // **TODO** 
    assert(runtime.shouldKeepParamsInRegisters());

    {
        // Count literals
        const size_t numLiterals = getLiterals().size();

        // Create N*N binary matrix to mark literals whose value 
        // is the same another across all merged processes
        // **THINK** this could be stored in a more efficient row-major
        // triangular format but a) the indexing is a nuisance and
        // b) while number of merged could be large, number of literals 
        // is going to be limited (especially as we currently load them all into registers!)
        /*std::vector<bool> literalSelfSimilarity(numLiterals * numLiterals, true);

        // Loop through merged processes
        // **NOTE** literals don't change across devices
        // **TODO** comparisons should be made based on FeNN-processed values
        mergedProcess.forEachMerged<NeuronUpdateProcess>(
            [&literalSelfSimilarity, numLiterals](const auto &np)
            {
                // Update upper-triangular portion of matrix (excluding diagonal) with comparison
                assert(np->getLiterals().size() == numLiterals);
                for (size_t i = 0; i < numLiterals; i++) {
                    for (size_t j = (i + 1); j < numLiterals; j++) {
                        literalSelfSimilarity[(i * numLiterals) + j] &= (np->getLiterals()[i] == np->getLiterals()[j]);
                    }
                }
            });

        // Start with each literal mapped to itself
        std::vector<std::vector<size_t>> literalMapping(numLiterals);

        // Loop through upper-triangular portion of matrix (excluding diagonal)
        for (size_t i = 0; i < numLiterals; i++) {
            for (size_t j = (i + 1); j < numLiterals; j++) {
                // If this literal always has the same value as another, add to mapping
                if (literalSelfSimilarity[(i * numLiterals) + j]) {
                    literalMapping[i] = j;
                }
            }
        }*/
        
        
        // Loop through literals
        // **TODO** used mapped representation
        for(size_t i = 0; i < numLiterals; i++) {
            // Load vector register with this literal
            auto literalReg = addVectorConstant<NeuronUpdateProcess>(
                mergedProcess, mergedFields, runtime.getNumDevices(), fieldBaseReg, processCodeGenerator,
                sharedCodeGenerator, scalarRegisterAllocator, vectorRegisterAllocator, sharedRegisters,
                [i](size_t, auto p)
                {
                    return static_cast<int32_t>(getVectorLiteralValue(p->getLiterals().at(i)));
                });
            
            // Add to environment
            auto &archLiteral = getLiterals().at(i);
            env.add(std::get<0>(archLiteral), "_literal_" + std::to_string(std::get<2>(archLiteral)), 
                    literalReg);
        }
    }

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
    /*(isExpCalled(getTokens())) {
        SpecialFunctions::Exp::add(c, scalarRegisterAllocator, vectorRegisterAllocator.get(),
                                    env, functionLibrary, getBackendFieldOffset(StateObjectID::LUT_EXP));
    }*/
    // Insert environment with this library
    EnvironmentLibrary envLibrary(env, functionLibrary);

    // Build vectorised neuron loop
    // **TODO** check all merged processes don't have multiple of 32 neurons, less than 4 * 32 neurons, multiple of 4 * 32 neurons
    unrollVectorLoopBody(
        envLibrary.getCodeGenerator(), scalarRegisterAllocator,
        numNeurons, getMaxUnroll(), numNeuronsNoTail, numNeuronsNoUnroll,
        [this, &envLibrary, &eventSinkState, &emitEventFunctionType, &mergedProcess, 
         &model, &runtime, &scalarRegisterAllocator, &varState, &vectorRegisterAllocator]
        (auto&, uint32_t r, auto maskReg)
        {
            EnvironmentExternal unrollEnv(envLibrary);

            // Loop through variables
            for(const auto &v : getVariables()) {
                // Allocate vector register
                const auto reg = vectorRegisterAllocator.getRegister((v.first + " V").c_str());

                // Add to environment
                unrollEnv.add(v.second.getUnderlying()->getType(), v.first, reg);

                // Generate load
                std::dynamic_pointer_cast<const Variable>(v.second.getUnderlying())->genLoad(
                    unrollEnv, reg, r, varState.at(v.second.getUnderlying()), *model);
            }

            // Loop through neuron event outputsC
            for(const auto &e : getOutputEventSinks()) {
                // Add function to environment to store current mask (inherently which neurons are spiking) to scalar memory
                const auto &state = eventSinkState.at(e.second.getUnderlying());
                unrollEnv.add(emitEventFunctionType, e.first, 
                              [e, maskReg, r, &state]
                              (auto &env, auto&, auto &scalarRegisterAllocator, auto spikeMaskReg, const auto&)
                              {
                                  auto feNNEventSink = std::dynamic_pointer_cast<const EventSinkImplementation>(e.second.getUnderlying());

                                  // If there's a mask, AND with spike and emit
                                  if (maskReg) {
                                      ALLOCATE_SCALAR(STmp);
                                      env.getCodeGenerator().and_(*STmp, *spikeMaskReg, *maskReg);
                                      feNNEventSink->genEmit(env, scalarRegisterAllocator, STmp, r, state);
                                  }
                                  // Otherwise, just emit spike mask register
                                  else {
                                      feNNEventSink->genEmit(env, scalarRegisterAllocator, spikeMaskReg, r, state);
                                  }
                                  return std::make_pair(Compiler::RegisterPtr{}, false);
                              });
            }

            // **HACK** if you're unlucky, there can be a RAW hazard between last load and first instruction so nop
            unrollEnv.getCodeGenerator().nop();

            // Compile tokens
            // **NOTE** we don't pass mask register through here - aside from blocking 
            // spike generation, there's no need to mask EVERY assignement etc
            {
                TypeChecker::EnvironmentInternal typeCheckEnv(unrollEnv);
                Compiler::EnvironmentInternal compilerEnv(unrollEnv);
                ErrorHandler errorHandler("Neuron update merged process " + std::to_string(mergedProcess.getIndex()));
                compileStatements(getTokens(), typeCheckEnv, compilerEnv, errorHandler, 
                                  nullptr, nullptr, runtime.getNeuronRoundingMode(),
                                  scalarRegisterAllocator, vectorRegisterAllocator);
            }

            // Loop through variables
            for(const auto &v : getVariables()) {
                // Get register
                const auto reg = unrollEnv.getVectorRegister(v.first);
                    
                // Generate store
                std::dynamic_pointer_cast<const Variable>(v.second.getUnderlying())->genStore(
                    unrollEnv, reg, r, varState.at(v.second.getUnderlying()), *model);
            }
        },
        [this, &eventSinkState, &model, &varState, &vectorRegisterAllocator]
        (auto &c, uint32_t numUnrolls)
        {
            // If any variables have vector addresses i.e. are stored in LLM, load number of bytes to unroll
            Assembler::VectorRegisterPtr numUnrollBytesReg;
            if(std::any_of(getVariables().cbegin(), getVariables().cend(),
                            [&model](const auto &v)
                            { 
                                return std::dynamic_pointer_cast<const Variable>(v.second.getUnderlying())->needsNumUnrollBytesReg(*model); 
                            }))
            {
                numUnrollBytesReg = vectorRegisterAllocator.getRegister("NumUnrollBytes V");
                c.vlui(*numUnrollBytesReg, numUnrolls * 2);
            }
            
            // Loop through variables and increment buffers
            for(const auto &v : getVariables()) {
                std::dynamic_pointer_cast<const Variable>(v.second.getUnderlying())->genIncrement(
                    c, numUnrolls, numUnrollBytesReg, varState.at(v.second.getUnderlying()), *model);
            }

            // Loop through output events and increment buffers
            for(const auto &e : getOutputEventSinks()) {
                auto feNNEventSink = std::dynamic_pointer_cast<const EventSinkImplementation>(e.second.getUnderlying());
                feNNEventSink->genIncrement(c, numUnrolls, eventSinkState.at(e.second.getUnderlying()));
            }
        });

    return sharedRegisters;
}

//----------------------------------------------------------------------------
// FeNN::Backend::DenseEventPropagationProcess
//----------------------------------------------------------------------------
DenseEventPropagationProcess::DenseEventPropagationProcess(Private, Frontend::Sliced<Frontend::EventSource> inputEventSource, 
                                                           Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                                           const std::string &name)
:   EventPropagationProcess(Private(), inputEventSource, target, name), m_Weight(weight)
{
    if(m_Weight == nullptr) {
        throw std::runtime_error("Dense event propagation process requires weight variable");
    }

    if (getWeight()->getShape().size() != 2) {
        throw std::runtime_error("Dense event propagation process requires weight variable with a 2D shape");
    }

    if (getInputEventSource().getShape().size() != 1) {
        throw std::runtime_error("Dense event propagation process requires source events with a 1D shape");
    }  

    if (getTarget().getShape().size() != 1) {
        throw std::runtime_error("Event propagation process requires target variable with a 1D shape");
    } 

    // Check weight shape matches input event shape
    if(getWeight()->getShape()[0] != getInputEventSource().getShape()[0]) {
        throw std::runtime_error("Weight with shape: " + Frontend::Shape::toString(getWeight()->getShape())
                                 + " is not compatible with event source with shape: " 
                                 + Frontend::Shape::toString(getInputEventSource().getShape()));
    }

    // Check weight shape matches target shape
    if(getWeight()->getShape()[1] != getTarget().getShape()[0]) {
        throw std::runtime_error("Weight with shape: " + Frontend::Shape::toString(getWeight()->getShape())
                                 + " is not compatible with target variable with shape: " 
                                 + Frontend::Shape::toString(getTarget().getShape()));
    }

    // Check weight and target have same types
    if(getWeight()->getType() != getTarget().getUnderlying()->getType()) {
        throw std::runtime_error("Weight with type: " + getWeight()->getType().getName() 
                                 + " is not compatible with target variable with type: " 
                                 + getTarget().getUnderlying()->getType().getName());
    }
}
//------------------------------------------------------------------------
void DenseEventPropagationProcess::updateMaxDMABufferSize(size_t &size) const
{
    size = std::max(size, getWeight()->getShape()[1]);
}
//------------------------------------------------------------------------
void DenseEventPropagationProcess::generateArchetypeCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                                         const KernelImplementation&, MergedFields &mergedFields, 
                                                         Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr, 
                                                         Assembler::ScalarRegisterPtr preIndReg, std::optional<uint32_t>, 
                                                         Assembler::CodeGenerator &processCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                         Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Make some friendlier-named references
    auto &c = processCodeGenerator;

    // Add fields for weight and target
    const uint32_t weightFieldOffset = mergedFields.addField<DenseEventPropagationProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getWeight()); 
        });
    
    const uint32_t targetFieldOffset = mergedFields.addField<DenseEventPropagationProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getTarget().getUnderlying()); 
        });

    // Define lambda function to get presynaptic stride of weight
    auto getStride =
        [&runtime](size_t d, auto p)
        {
            return static_cast<uint32_t>(std::get<1>(runtime.getDeviceArrayShapeStrides(p->getWeight(), d))[0]);
        };
    
    // Get stride
    // **NOTE** this is going into a multiply so always needs to be in a register
    // **OPTIMISE** immediate loop count saves much more than a single li!
    const auto strideReg = std::get<Assembler::ScalarRegisterPtr>(
        addScalarValue<DenseEventPropagationProcess>(
            0, mergedProcess, runtime.getNumDevices(), mergedFields, fieldBaseReg, 
            processCodeGenerator, scalarRegisterAllocator, getStride));

    // No need for unrolling if all strides are 
    // less than the size of a single unrolled iteration
    // **THINK** this could also trigger a reduction in maxUnroll
    const bool strideNoUnroll = allOf<DenseEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            return (std::get<uint32_t>(num) < (getMaxUnroll() * 64));
        });

    // No need to unroll pairs if all strides have 
    // less than 2 vector remaining after unrolling
    const bool strideNoPairs = allOf<DenseEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            // Calculate how much remains after unrolled iterations
            const uint32_t unrollRemainder = (std::get<uint32_t>(num) % (getMaxUnroll() * 64));
            return (unrollRemainder < 128);
        });

    // No need to add final iteration if all strides
    // are a multiple of two after unrolling
    const bool strideNoFinal = allOf<DenseEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            const uint32_t unrollRemainder = (std::get<uint32_t>(num) % (getMaxUnroll() * 64));
            return (unrollRemainder % 128) == 0;
        });

    // Load target register from state fields
    ALLOCATE_SCALAR(STargetBuf);
    c.lw(*STargetBuf, *fieldBaseReg, targetFieldOffset);

    // Calculate row start address
    ALLOCATE_SCALAR(SWeightBuffer);
    c.lw(*SWeightBuffer, *fieldBaseReg, weightFieldOffset);
    {
        ALLOCATE_SCALAR(STemp);
        c.mul(*STemp, *preIndReg, *strideReg);
        c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
    }

    // **TODO** this should be fine, just need to index target correctly
    assert(!getTarget().hasTime());

    ALLOCATE_VECTOR(VWeight);
    ALLOCATE_VECTOR(VTarget1);
    ALLOCATE_VECTOR(VTarget2);
    ALLOCATE_VECTOR(VTargetNew);
    
    // Preload first ISyn to avoid stall
    c.vloadv(*VTarget1, *STargetBuf, 0);

    // Unroll loop over row
    Assembler::Utils::unrollOddEvenLoopBody(
        c, scalarRegisterAllocator,
        *strideReg, getMaxUnroll(), 64,
        strideNoUnroll, strideNoPairs, strideNoFinal,
        [this, SWeightBuffer, STargetBuf, VWeight, VTarget1, VTarget2, VTargetNew]
        (Assembler::CodeGenerator &c, uint32_t r, bool even)
        {
            // Load vector of weights
            c.vloadv(*VWeight, *SWeightBuffer, r * 64);

            // Load NEXT vector of target to avoid stall
            // **YUCK** in last iteration, while this may not be accessed, it may be out of bounds                  
            c.vloadv(even ? *VTarget2 : *VTarget1, *STargetBuf, (r + 1) * 64);

            // Add weights to ISyn
            auto VTarget = even ? VTarget1 : VTarget2;
            c.vadd_s(*VTarget, *VTarget, *VWeight);
            
            // Write back target
            c.vstore(*VTarget, *STargetBuf, r * 64);
        },
        [this, SWeightBuffer, STargetBuf](Assembler::CodeGenerator &c, uint32_t numUnrolls)
        {
            // Increment pointers 
            c.addi(*STargetBuf, *STargetBuf, 64 * numUnrolls);
            c.addi(*SWeightBuffer, *SWeightBuffer, 64 * numUnrolls);
        });
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const Frontend::State>> DenseEventPropagationProcess::getAllState() const
{
    return {getInputEventSource().getUnderlying(), getWeight(), getTarget().getUnderlying()};
}
//----------------------------------------------------------------------------
void DenseEventPropagationProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const
{
    using namespace ::Common::Utils;
    UPDATE_HASH_CLASS_NAME(DenseEventPropagationProcess);

    // **NOTE** we do NOT call the superclass here because we want to set our 
    // own name and do not want to include input event source in hash as 
    // event sources are handled seperately in FeNN backend

    // Targets
    // **NOTE** generated code doesn't depend on underlying target type so do not include in hash
    getTarget().updateMergeHash(hash, false);

    // Include hash of target memory space
    updateHash(static_cast<const Model&>(model).getStateMemSpace(getTarget().getUnderlying(), 
                                                                 true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
    
    // **NOTE** we don't need to hash the weights as the generated code doesn't depend on their type

    // Include hash of weight memory space
    updateHash(static_cast<const Model&>(model).getStateMemSpace(getTarget().getUnderlying(), 
                                                                 true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
}
//----------------------------------------------------------------------------
void DenseEventPropagationProcess::updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                                   uint32_t &compatibleSplitDimensions,
                                                                   uint32_t &compatibleIndexDimensions) const 
{
    // If variable is weight, it can only be split in 2nd (postsynaptic) axis
    // and it can only be indexed along 1st (presynaptic) axis
    if(state == getWeight()) {
        compatibleSplitDimensions &= (1 << 1);
        compatibleIndexDimensions &= (1 << 0);
    }
    // Otherwise, superclass
    else {
        Frontend::EventPropagationProcess::updateCompatibleSplitDimensions(state, compatibleSplitDimensions, 
                                                                           compatibleIndexDimensions);
    }
}
//----------------------------------------------------------------------------
void DenseEventPropagationProcess::updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                                            MemSpace &compatibleMemSpaces) const
{
    // If variable is weight, it can  be located in URAM or DRAM
    if(state == getWeight()) {
        compatibleMemSpaces &= (MemSpace::DRAM | MemSpace::URAM);
    }
    // Otherwise, if variable's target, it can be in URAM or LLM 
    else if(state == getTarget().getUnderlying()) {
        compatibleMemSpaces &= (MemSpace::LLM | MemSpace::URAM);
    }
    else {
        assert(state == getInputEventSource().getUnderlying());
    }
}

//----------------------------------------------------------------------------
// FeNN::Backend::SparseEventPropagationProcess
//----------------------------------------------------------------------------
SparseEventPropagationProcess::SparseEventPropagationProcess(Private, Frontend::Sliced<Frontend::EventSource> inputEventSource, 
                                                             Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                                             size_t numSparseConnectivityBits, const std::string &name)
:   EventPropagationProcess(Private(), inputEventSource, target, name), m_Weight(weight), m_NumSparseConnectivityBits(numSparseConnectivityBits)
{
    if(m_Weight == nullptr) {
        throw std::runtime_error("Sparse event propagation process requires weight variable");
    }

    if (getWeight()->getShape().size() != 2) {
        throw std::runtime_error("Sparse event propagation process requires weight variable with a 2D shape");
    }

    if (getInputEventSource().getShape().size() != 1) {
        throw std::runtime_error("Sparse event propagation process requires source events with a 1D shape");
    }  

    if (getTarget().getShape().size() != 1) {
        throw std::runtime_error("Sparse propagation process requires target variable with a 1D shape");
    } 

    // Check weight shape matches input event shape
    if(getWeight()->getShape()[0] != getInputEventSource().getShape()[0]) {
        throw std::runtime_error("Weight with shape: " + Frontend::Shape::toString(getWeight()->getShape()) 
                                 + " is not compatible with event source with shape: " 
                                 + Frontend::Shape::toString(getInputEventSource().getShape()));
    }

    // Check weight shape is less than or equal to target shape
    if(getWeight()->getShape()[1] > getTarget().getShape()[0]) {
        throw std::runtime_error("Weight with shape: " + Frontend::Shape::toString(getWeight()->getShape()) 
                                 + " is not compatible with target variable with shape: " 
                                 + Frontend::Shape::toString(getTarget().getShape()));
    }

    // Check weight and target have same types
    if(getWeight()->getType() != getTarget().getUnderlying()->getType()) {
        throw std::runtime_error("Weight with type: " + getWeight()->getType().getName() 
                                 + " is not compatible with target variable with type: " 
                                 + getTarget().getUnderlying()->getType().getName());
    }
}
//------------------------------------------------------------------------
void SparseEventPropagationProcess::updateMaxDMABufferSize(size_t &size) const
{
    size = std::max(size, getWeight()->getShape()[1]);
}
//------------------------------------------------------------------------
void SparseEventPropagationProcess::generateArchetypeCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                                          const KernelImplementation&, MergedFields &mergedFields, 
                                                          Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr, 
                                                          Assembler::ScalarRegisterPtr preIndReg, std::optional<uint32_t>, 
                                                          Assembler::CodeGenerator &processCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                          Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Make some friendlier-named references
    auto &c = processCodeGenerator;

    // Add fields for weight and target
    const uint32_t weightFieldOffset = mergedFields.addField<SparseEventPropagationProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getWeight()); 
        });

    const uint32_t targetFieldOffset = mergedFields.addField<SparseEventPropagationProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getTarget().getUnderlying()); 
        });

    // Define lambda function to get presynaptic stride of weight
    auto getStride =
        [&runtime](size_t d, auto p)
        {
            return static_cast<uint32_t>(std::get<1>(runtime.getDeviceArrayShapeStrides(p->getWeight(), d))[0]);
        };

    // Get stride
    // **NOTE** this is going into a multiply so always needs to be in a register
    // **OPTIMISE** immediate loop count saves much more than a single li!
    const auto strideReg = std::get<Assembler::ScalarRegisterPtr>(
        addScalarValue<SparseEventPropagationProcess>(
            0, mergedProcess, runtime.getNumDevices(), mergedFields, fieldBaseReg, 
            processCodeGenerator, scalarRegisterAllocator, getStride));

    // No need for unrolling if all strides are 
    // less than the size of a single unrolled iteration
    // **THINK** this could also trigger a reduction in maxUnroll
    const bool strideNoUnroll = allOf<SparseEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            return (std::get<uint32_t>(num) < (getMaxUnroll() * 64));
        });

    // No need to unroll pairs if all strides have 
    // less than 2 vector remaining after unrolling
    const bool strideNoPairs = allOf<SparseEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            // Calculate how much remains after unrolled iterations
            const uint32_t unrollRemainder = (std::get<uint32_t>(num) % (getMaxUnroll() * 64));
            return (unrollRemainder < 128);
        });

    // No need to add final iteration if all strides
    // are a multiple of two after unrolling
    const bool strideNoFinal = allOf<SparseEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            const uint32_t unrollRemainder = (std::get<uint32_t>(num) % (getMaxUnroll() * 64));
            return (unrollRemainder % 128) == 0;
        });

    // Load target register from state fields
    ALLOCATE_SCALAR(STargetBuf);
    c.lw(*STargetBuf, *fieldBaseReg, targetFieldOffset);

    // Calculate row start address
    ALLOCATE_SCALAR(SWeightBuffer);
    c.lw(*SWeightBuffer, *fieldBaseReg, weightFieldOffset);
    {
        ALLOCATE_SCALAR(STemp);
        c.mul(*STemp, *preIndReg, *strideReg);
        c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
    }

    // **TODO** this should be fine, just need to index target correctly
    assert(!getTarget().hasTime());

    // Loop over postsynaptic neurons
    ALLOCATE_VECTOR(VAccum);
    ALLOCATE_VECTOR(VWeightInd1);
    ALLOCATE_VECTOR(VWeightInd2);
    ALLOCATE_VECTOR(VPostAddr);
    ALLOCATE_VECTOR(VWeight);

    // Preload first weights and indices to avoid stall
    c.vloadv(*VWeightInd1, *SWeightBuffer, 0);

    Assembler::Utils::unrollOddEvenLoopBody(
        c, scalarRegisterAllocator,
        *strideReg, getMaxUnroll(), 64,
        strideNoUnroll, strideNoPairs, strideNoFinal,
        [this, SWeightBuffer, STargetBuf, VAccum, VPostAddr, VWeight, VWeightInd1, VWeightInd2]
        (Assembler::CodeGenerator &c, uint32_t r, bool even)
        {
            // Load NEXT vector of weights and indices
            c.vloadv(even ? *VWeightInd2 : *VWeightInd1, *SWeightBuffer, (r + 1) * 64);

            // Extract postsynaptic index and add base address
            auto VWeightInd = even ? VWeightInd1 : VWeightInd2;
            c.vandadd(getNumSparseConnectivityBits(), *VPostAddr, *VWeightInd,
                      *STargetBuf);

            // Load accumulator
            c.vloadl(*VAccum, *VPostAddr);

            // Extract weight
            c.vsrai(getNumSparseConnectivityBits(), *VWeight, *VWeightInd);

            // Add weights to accumulator loaded in previous iteration
            c.vadd_s(*VAccum, *VAccum, *VWeight);

            // Write back accumulator
            c.vstorel(*VAccum, *VPostAddr);
        },
        [this, SWeightBuffer]
        (Assembler::CodeGenerator &c, uint32_t numUnrolls)
        {
            // Increment pointers 
            c.addi(*SWeightBuffer, *SWeightBuffer, 64 * numUnrolls);
        });

}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const Frontend::State>> SparseEventPropagationProcess::getAllState() const
{
    return {getInputEventSource().getUnderlying(), getWeight(), getTarget().getUnderlying()};
}
//----------------------------------------------------------------------------
void SparseEventPropagationProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const
{
    using namespace ::Common::Utils;
    UPDATE_HASH_CLASS_NAME(SparseEventPropagationProcess);

    // **NOTE** we do NOT call the superclass here because we want to set our 
    // own name and do not want to include input event source in hash as 
    // event sources are handled seperately in FeNN backend

    // Include number of sparse connectivity bits in hash
    updateHash(getNumSparseConnectivityBits(), hash);

    // Targets
    // **NOTE** generated code doesn't depend on underlying target type so do not include in hash
    getTarget().updateMergeHash(hash, false);

    // Include hash of target memory space
    updateHash(static_cast<const Model&>(model).getStateMemSpace(getTarget().getUnderlying(), 
                                                                 true/*getRuntime().shouldUseDRAMForWeights()*/), hash);

    // **NOTE** we don't need to hash the weights as the generated code doesn't depend on their type

    // Include hash of weight memory space
    updateHash(static_cast<const Model&>(model).getStateMemSpace(getTarget().getUnderlying(), 
                                                                 true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
}
//----------------------------------------------------------------------------
void SparseEventPropagationProcess::updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                                   uint32_t &compatibleSplitDimensions,
                                                                   uint32_t &compatibleIndexDimensions) const 
{
    // If variable is weight, it can only be split in 2nd (postsynaptic) dimension
    // and it can only be indexed along 1st (presynaptic) axis
    if(state == getWeight()) {
        compatibleSplitDimensions &= (1 << 1);
        compatibleIndexDimensions &= (1 << 0);
    }
    // Otherwise, superclass
    else {
        Frontend::EventPropagationProcess::updateCompatibleSplitDimensions(state, compatibleSplitDimensions, 
                                                                           compatibleIndexDimensions);
    }
}
//----------------------------------------------------------------------------
void SparseEventPropagationProcess::updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                                             MemSpace &compatibleMemSpaces) const
{
    // If variable is weight, it can  be located in URAM or DRAM
    if(state == getWeight()) {
        compatibleMemSpaces &= (MemSpace::DRAM | MemSpace::URAM);
    }
    // Otherwise, if variable's target, it can only be in LLM 
    else if(state == getTarget().getUnderlying()) {
        compatibleMemSpaces &= MemSpace::LLM;
    }
    else {
        assert(state == getInputEventSource().getUnderlying());
    }
}


//----------------------------------------------------------------------------
// FeNN::Backend::DelayEventPropagationProcess
//----------------------------------------------------------------------------
DelayEventPropagationProcess::DelayEventPropagationProcess(Private, Frontend::Sliced<Frontend::EventSource> inputEventSource, 
                                                           Frontend::VariablePtr weight, Frontend::Sliced<Frontend::Variable> target, 
                                                           size_t numDelayBits, const std::string &name)
:   EventPropagationProcess(Private(), inputEventSource, target, name), m_Weight(weight), m_NumDelayBits(numDelayBits)
{
    if(m_Weight == nullptr) {
        throw std::runtime_error("Delayed event propagation process requires weight variable");
    }

    if (getWeight()->getShape().size() != 2) {
        throw std::runtime_error("Delayed event propagation process requires weight variable with a 2D shape");
    }

    if (getInputEventSource().getShape().size() != 1) {
        throw std::runtime_error("Delayed event propagation process requires source events with a 1D shape");
    }  

    if (getTarget().hasTimeSlice()) {
        throw std::runtime_error("Delayed propagation process requires target whose time dimension hasn't been sliced away");
    }
    
    if (getTarget().getShape().size() != 2) {
        throw std::runtime_error("Delayed propagation process requires target variable with a 2D shape");
    } 

    if(getTarget().getNumTimesteps() != (1 << (getNumDelayBits()- 1))) {
        throw std::runtime_error("Time dimension of delayed propagation process target buffer "
                                 " does not match specified number of delay bits");
    }

    // Check weight shape matches input event shape
    if(getWeight()->getShape()[0] != getInputEventSource().getShape()[0]) {
        throw std::runtime_error("Weight with shape: " + Frontend::Shape::toString(getWeight()->getShape()) 
                                 + " is not compatible with event source with shape: " 
                                 + Frontend::Shape::toString(getInputEventSource().getShape()));
    }

    // Check weight shape is less than or equal to target 
    if(getWeight()->getShape()[1] != getTarget().getShape()[1]) {
        throw std::runtime_error("Weight with shape: " + Frontend::Shape::toString(getWeight()->getShape()) 
                                 + " is not compatible with target variable with shape: " 
                                 + Frontend::Shape::toString(getTarget().getShape()));
    }

    // Check weight and target have same types
    if(getWeight()->getType() != getTarget().getUnderlying()->getType()) {
        throw std::runtime_error("Weight with type: " + getWeight()->getType().getName() 
                                 + " is not compatible with target variable with type: " 
                                 + getTarget().getUnderlying()->getType().getName());
    }
}
//------------------------------------------------------------------------
void DelayEventPropagationProcess::updateMaxDMABufferSize(size_t &size) const
{
    size = std::max(size, getWeight()->getShape()[1]);
}
//------------------------------------------------------------------------
void DelayEventPropagationProcess::generateArchetypeCode(const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, 
                                                         const KernelImplementation&, MergedFields &mergedFields, 
                                                         Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr timeReg, 
                                                         Assembler::ScalarRegisterPtr preIndReg, std::optional<uint32_t>, 
                                                         Assembler::CodeGenerator &processCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                                         Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Make some friendlier-named references
    auto &c = processCodeGenerator;

    // Add fields for weight and target
    const uint32_t weightFieldOffset = mergedFields.addField<DelayEventPropagationProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getWeight()); 
        });

    const uint32_t targetFieldOffset = mergedFields.addField<DelayEventPropagationProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getTarget().getUnderlying()); 
        });

    // Define lambda function to get presynaptic stride of weight
    auto getStride =
        [&runtime](size_t d, auto p)
        {
            return static_cast<uint32_t>(std::get<1>(runtime.getDeviceArrayShapeStrides(p->getWeight(), d))[0]);
        };

    // Get stride
    // **NOTE** this is going into a multiply so always needs to be in a register
    // **OPTIMISE** immediate loop count saves much more than a single li!
    const auto strideReg = std::get<Assembler::ScalarRegisterPtr>(
        addScalarValue<DelayEventPropagationProcess>(
            0, mergedProcess, runtime.getNumDevices(), mergedFields, fieldBaseReg, 
            processCodeGenerator, scalarRegisterAllocator, getStride));

    // No need for unrolling if all strides are 
    // less than the size of a single unrolled iteration
    // **THINK** this could also trigger a reduction in maxUnroll
    const bool strideNoUnroll = allOf<DelayEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            return (std::get<uint32_t>(num) < (getMaxUnroll() * 64));
        });

    // No need to unroll pairs if all strides have 
    // less than 2 vector remaining after unrolling
    const bool strideNoPairs = allOf<DelayEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            // Calculate how much remains after unrolled iterations
            const uint32_t unrollRemainder = (std::get<uint32_t>(num) % (getMaxUnroll() * 64));
            return (unrollRemainder < 128);
        });

    // No need to add final iteration if all strides
    // are a multiple of two after unrolling
    const bool strideNoFinal = allOf<DelayEventPropagationProcess>(
        mergedProcess, runtime.getNumDevices(), getStride,
        [this](const MergedFields::FieldValue &num)
        {
            const uint32_t unrollRemainder = (std::get<uint32_t>(num) % (getMaxUnroll() * 64));
            return (unrollRemainder % 128) == 0;
        });

    // Load target register from state fields
    ALLOCATE_SCALAR(STargetBuf);
    c.lw(*STargetBuf, *fieldBaseReg, targetFieldOffset);

    // Calculate row start address
    ALLOCATE_SCALAR(SWeightBuffer);
    c.lw(*SWeightBuffer, *fieldBaseReg, weightFieldOffset);
    {
        ALLOCATE_SCALAR(STemp);
        c.mul(*STemp, *preIndReg, *strideReg);
        c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
    }

    // Loop over postsynaptic neurons
    ALLOCATE_SCALAR(STargetReg);
    ALLOCATE_VECTOR(VAccum);
    ALLOCATE_VECTOR(VWeightInd1);
    ALLOCATE_VECTOR(VWeightInd2);
    ALLOCATE_VECTOR(VPostAddr);
    ALLOCATE_VECTOR(VWeight);
    ALLOCATE_VECTOR(VTime);

    // **OPTIMISE** this could be done in the runtime 
    // at the  top of the event source process group
    c.vfill(*VTime, *timeReg);
    c.vslli(1, *VTime, *VTime);

    // Preload first weights and indices to avoid stall
    c.vloadv(*VWeightInd1, *SWeightBuffer, 0);

    // Stride of each neuron's delay buffer in LLM
    const uint32_t delayStride = 2 * getTarget().getNumTimesteps();

    Assembler::Utils::unrollOddEvenLoopBody(
        c, scalarRegisterAllocator,
        *strideReg, getMaxUnroll(), 64,
        strideNoUnroll, strideNoPairs, strideNoFinal,
        [this, delayStride, SWeightBuffer, STargetBuf, VAccum, VPostAddr, VTime, VWeight, VWeightInd1, VWeightInd2]
        (Assembler::CodeGenerator &c, uint32_t r, bool even)
        {
            // Load NEXT vector of weights and indices
            c.vloadv(even ? *VWeightInd2 : *VWeightInd1, *SWeightBuffer, (r + 1) * 64);

            // Add time to delay
            auto VWeightInd = even ? VWeightInd1 : VWeightInd2;
            c.vadd(*VPostAddr, *VWeightInd, *VTime);

            // Extract delay and add base address
            c.vandadd(getNumDelayBits(), *VPostAddr, *VPostAddr, *STargetBuf);

            // Load accumulator
            c.vloadl(*VAccum, *VPostAddr, delayStride * r);

            // Extract weight
            c.vsrai(getNumDelayBits(), *VWeight, *VWeightInd);

            // Add weights to accumulator loaded in previous iteration
            c.vadd_s(*VAccum, *VAccum, *VWeight);

            // Write back accumulator
            c.vstorel(*VAccum, *VPostAddr, delayStride * r);
        },
        [this, delayStride, STargetReg, SWeightBuffer, &vectorRegisterAllocator]
        (Assembler::CodeGenerator &c, uint32_t numUnrolls)
        {
            // Increment pointers 
            c.addi(*SWeightBuffer, *SWeightBuffer, 64 * numUnrolls);
            c.addi(*STargetReg, *STargetReg, numUnrolls * delayStride);
        });
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const Frontend::State>> DelayEventPropagationProcess::getAllState() const
{
    return {getInputEventSource().getUnderlying(), getWeight(), getTarget().getUnderlying()};
}
//----------------------------------------------------------------------------
void DelayEventPropagationProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const
{
    using namespace ::Common::Utils;
    UPDATE_HASH_CLASS_NAME(DelayEventPropagationProcess);

    // **NOTE** we do NOT call the superclass here because we want to set our 
    // own name and do not want to include input event source in hash as 
    // event sources are handled seperately in FeNN backend

    // Include number of delay bits in hash
    updateHash(getNumDelayBits(), hash);

    // Targets
    // **NOTE** generated code doesn't depend on underlying target type so do not include in hash
    getTarget().updateMergeHash(hash, false);

    // **YUCK** include number of timesteps in hash
    updateHash(getTarget().getNumTimesteps(), hash);

    // Include hash of target memory space
    updateHash(static_cast<const Model&>(model).getStateMemSpace(getTarget().getUnderlying(), 
                                                                 true/*getRuntime().shouldUseDRAMForWeights()*/), hash);

    // **NOTE** we don't need to hash the weights as the generated code doesn't depend on their type

    // Include hash of weight memory space
    updateHash(static_cast<const Model&>(model).getStateMemSpace(getTarget().getUnderlying(), 
                                                                 true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
}
//----------------------------------------------------------------------------
void DelayEventPropagationProcess::updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                                   uint32_t &compatibleSplitDimensions,
                                                                   uint32_t &compatibleIndexDimensions) const 
{
    // If variable is weight, it can only be split in 2nd (postsynaptic) dimension
    // and it can only be indexed along 1st (presynaptic) axis
    if(state == getWeight()) {
        compatibleSplitDimensions &= (1 << 1);
        compatibleIndexDimensions &= (1 << 0);
    }
    // Otherwise, superclass
    else {
        Frontend::EventPropagationProcess::updateCompatibleSplitDimensions(state, compatibleSplitDimensions,
                                                                           compatibleIndexDimensions);
    }
}
//----------------------------------------------------------------------------
void DelayEventPropagationProcess::updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                                            MemSpace &compatibleMemSpaces) const
{
    // If variable is weight, it can  be located in URAM or DRAM
    if(state == getWeight()) {
        compatibleMemSpaces &= (MemSpace::DRAM | MemSpace::URAM);
    }
    // Otherwise, if variable's target, it can only be in LLM 
    else if(state == getTarget().getUnderlying()) {
        compatibleMemSpaces &= MemSpace::LLM;
    }
    else {
        assert(state == getInputEventSource().getUnderlying());
    }
}
//----------------------------------------------------------------------------
// FeNN::Backend::EventPropagationProcess
//----------------------------------------------------------------------------
/*

void generateDRAMWordLoop(const std::vector<std::unique_ptr<RowGeneratorBase>> &rowGenerators, 
                          ScalarRegisterPtr eventBufferReg, 
                          ScalarRegisterPtr eventBufferEndReg)
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
RNGInitProcess::RNGInitProcess(Private, Frontend::VariablePtr seed, const std::string &name)
:   Frontend::RNGInitProcess(Private(), seed, name)
{
    if(getSeed()->getShape().size() != 2) {
        throw std::runtime_error("RNG init process requires two dimensional seed");
    }

    if(getSeed()->getType().getSize() != 2) {
        throw std::runtime_error("On FeNN, RNG init process seed values must be 16-bit");
    }

    if(getSeed()->getShape().back() != 64) {
        throw std::runtime_error("On FeNN, each RNG init process requires 64 seed values for each device");
    }
}
//----------------------------------------------------------------------------
void RNGInitProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const
{
    // Superclass
    Frontend::RNGInitProcess::updateMergeHash(hash, model);

    // Include hash of seed memory space
    ::Common::Utils::updateHash(
        static_cast<const Model&>(model).getStateMemSpace(getSeed(), 
                                                          true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
}
//----------------------------------------------------------------------------
void RNGInitProcess::updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                              MemSpace &compatibleMemSpaces) const
{
    assert(state == getSeed());

    // Seeds can only be stored in URAM
    compatibleMemSpaces &= MemSpace::URAM;
}
//----------------------------------------------------------------------------
std::vector<Compiler::RegisterPtr> RNGInitProcess::generateArchetypeCode(
        const Frontend::MergedProcess&, const Runtime&, const KernelImplementation&, MergedFields &mergedFields,
        Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr,
        std::optional<uint32_t>, Assembler::CodeGenerator &processCodeGenerator, 
        Assembler::CodeGenerator &, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
        Assembler::VectorRegisterAllocator&) const 
{
    // Add fields
    const uint32_t seedFieldOffset = mergedFields.addField<RNGInitProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getSeed()); 
        });
    
    // Allocate scalar register to hold address of seed buffer
    ALLOCATE_SCALAR(SReg);

    // Generate code to load address of seed
    auto &c = processCodeGenerator;
    c.lw(*SReg, *fieldBaseReg, seedFieldOffset);
    
    // Load seed into RNG registers
    c.vloadr0(*SReg);
    c.vloadr1(*SReg, 64);

    return {};
}

//----------------------------------------------------------------------------
// FeNN::Backend::MemsetProcess
//----------------------------------------------------------------------------
void MemsetProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const
{
    // Superclass
    Frontend::MemsetProcess::updateMergeHash(hash, model);

    // Include hash of target memory space
    ::Common::Utils::updateHash(
        static_cast<const Model&>(model).getStateMemSpace(getTarget().getUnderlying(), 
                                                          true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
}
//----------------------------------------------------------------------------
void MemsetProcess::updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                             MemSpace &compatibleMemSpaces) const
{
    assert(state == getTarget().getUnderlying());

    // **TODO** memset could handle anything
    compatibleMemSpaces &= (MemSpace::LLM | MemSpace::URAM);
}
//----------------------------------------------------------------------------
std::vector<Compiler::RegisterPtr> MemsetProcess::generateArchetypeCode(
    const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, const KernelImplementation&, 
    MergedFields &mergedFields, Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr,
    std::optional<uint32_t>, Assembler::CodeGenerator &processCodeGenerator, 
    Assembler::CodeGenerator &sharedCodeGenerator, Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
    Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
    
{
    // Add process fields
    const uint32_t targetFieldOffset = mergedFields.addField<MemsetProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getTarget().getUnderlying()); 
        });
   
                                
    // Register allocation
    ALLOCATE_SCALAR(STargetBuffer);
    
    // Figure out best way to represent number of 
    std::vector<Compiler::RegisterPtr> sharedRegisters;
    auto stride = addScalarValue<MemsetProcess>(
        12, mergedProcess, runtime.getNumDevices(), mergedFields, fieldBaseReg,
        processCodeGenerator, sharedCodeGenerator, scalarRegisterAllocator, sharedRegisters,
        [&runtime](size_t d, auto p)
        { 
            // Get stride of target
            const auto stride = std::get<1>(runtime.getDeviceArrayShapeStrides(p->getTarget().getUnderlying(), d));

            // Return stride of axes at top of slice
            return static_cast<uint32_t>(stride.at(stride.size() - p->getTarget().getShape().size()));
        });

    
    auto &c = processCodeGenerator;
    c.lw(*STargetBuffer, *fieldBaseReg, targetFieldOffset);

    // **TODO** add stride * current time
    assert(!getTarget().hasTime());

    switch(runtime.getModel<Model>()->getStateMemSpace(getTarget().getUnderlying(), 
                                                       runtime.shouldUseDRAMForWeights()))
    {
    case MemSpace::URAM:
    {
        generateURAMMemset(c, scalarRegisterAllocator, vectorRegisterAllocator,
                           STargetBuffer, stride);
        break;
    }
    case MemSpace::LLM: 
    {
        generateLLMMemset(c, scalarRegisterAllocator, vectorRegisterAllocator,
                          STargetBuffer, stride);
        break;
    }
    default:
        throw std::runtime_error("Memspace process incompatible with target memory space");
    }

    return sharedRegisters;
}
//----------------------------------------------------------------------------
void MemsetProcess::generateLLMMemset(Assembler::CodeGenerator &c,
                                      Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                      Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                      Assembler::ScalarRegisterPtr targetReg,
                                      ScalarConstant stride) const
{
    ALLOCATE_VECTOR(VValue);
    ALLOCATE_VECTOR(VLLMAddress);
    ALLOCATE_VECTOR(VNumUnrollBytes);

    // Load value to memset
    c.vlui(*VValue, 0);

    // Broadcast address
    c.vfill(*VLLMAddress, *targetReg);

    // Generate unrolled loop 
    // **TODO** figure out unrolledness
    unrollVectorLoopBody(
        c, scalarRegisterAllocator, stride, getMaxUnroll(), true, false,
        [VLLMAddress, VValue]
        (auto &c, uint32_t r, auto)
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
                                       Assembler::ScalarRegisterPtr targetReg,
                                       ScalarConstant stride) const
{
    ALLOCATE_VECTOR(VValue);

    // Load value to memset and calculate unroll bytes
    // **TODO** parameterise
    c.vlui(*VValue, 0);

    // Generate unrolled loop 
    // **TODO** figure out unrolled
    unrollVectorLoopBody(
        c, scalarRegisterAllocator, stride, getMaxUnroll(), true, false,
        [targetReg, VValue]
        (auto &c, uint32_t r, auto)
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
BroadcastProcess::BroadcastProcess(Private, Frontend::VariablePtr source, Frontend::VariablePtr target, const std::string &name)
:   Frontend::Process(name), m_Source(source), m_Target(target)
{
    if(m_Source == nullptr) {
        throw std::runtime_error("Broadcast process requires source");
    }

    if(m_Source->getShape().size() != 1) {
        throw std::runtime_error("Multi-dimensional sources aren't currently "
                                 "supported by broadcast processes");
    }

    // If target is a variable
    if(m_Target == nullptr) {
        throw std::runtime_error("Broadcast process requires target");
    }

    if(m_Target->getShape().size() != 2) {
        throw std::runtime_error("Broadcast process currently required 2 dimensional target");
    }

    if(m_Target->getShape()[0] != m_Source->getShape()[0]) {
        throw std::runtime_error("Broadcast process requires first dimension of source and target to match");
    }

    if(m_Target->getShape()[1] != 32) {
        throw std::runtime_error("Broadcast process can currnetly only broadcast over 32 elements");
    }

    if (m_Source->getType() != m_Target->getType()) {
        throw std::runtime_error("Broadcast process requires source and target with same shape");
    }
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const Frontend::State>> BroadcastProcess::getAllState() const
{
    return {getSource(), getTarget()};
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const Frontend::EventSource>> BroadcastProcess::getAllEventSources() const
{
    return {};
}
//----------------------------------------------------------------------------
std::vector<Frontend::Sliced<Frontend::EventSink>> BroadcastProcess::getAllEventSinks() const
{
    return {};
}
//----------------------------------------------------------------------------
void BroadcastProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Frontend::Model &model) const
{
    UPDATE_HASH_CLASS_NAME(BroadcastProcess);

    getSource()->updateMergeHash(hash);
    getTarget()->updateMergeHash(hash);

    // Include hash of source and target memory spaces
    ::Common::Utils::updateHash(
        static_cast<const Model&>(model).getStateMemSpace(getSource(), 
                                                          true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
    ::Common::Utils::updateHash(
        static_cast<const Model&>(model).getStateMemSpace(getTarget(), 
                                                          true/*getRuntime().shouldUseDRAMForWeights()*/), hash);
}
//----------------------------------------------------------------------------
void BroadcastProcess::updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                       uint32_t &compatibleSplitDimensions,
                                                       uint32_t &compatibleIndexDimensions) const
{
    assert(state == getTarget() || state == getSource());
    
    // Broadcast process is used for populating LUTs - can't be split
    compatibleSplitDimensions = 0;

    // Nothing needs to be padded to support indexing
    compatibleIndexDimensions = 0;
}
//----------------------------------------------------------------------------
void BroadcastProcess::updateCompatibleMemSpace(std::shared_ptr<const Frontend::State> state, 
                                                MemSpace &compatibleMemSpaces) const
{
    // If variable is source, it can only be located in BRAM
    if(state == getSource()) {
        compatibleMemSpaces &= MemSpace::BRAM;
    }
    else {
        // Otherwise, if variable's target, it can only located in LLM or URAMLLM
        assert(state == getTarget());
        compatibleMemSpaces &= MemSpace::LLM;
    }
}
//----------------------------------------------------------------------------
std::vector<Compiler::RegisterPtr> BroadcastProcess::generateArchetypeCode(
    const Frontend::MergedProcess &mergedProcess, const Runtime &runtime, const KernelImplementation&, 
    MergedFields &mergedFields, Assembler::ScalarRegisterPtr fieldBaseReg, Assembler::ScalarRegisterPtr,
    std::optional<uint32_t>, Assembler::CodeGenerator &processCodeGenerator, Assembler::CodeGenerator &sharedCodeGenerator,
    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, Assembler::VectorRegisterAllocator &vectorRegisterAllocator) const
{
    // Add process fields
    const uint32_t sourceFieldOffset = mergedFields.addField<BroadcastProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getSource()); 
        });
    const uint32_t targetFieldOffset = mergedFields.addField<BroadcastProcess>(
        [](const Frontend::DeviceBase &d, auto p)
        { 
            return d.getArray(p->getTarget()); 
        });
                                
    // Register allocation
    ALLOCATE_SCALAR(SDataBuffer);
    ALLOCATE_VECTOR(VAddress);
    ALLOCATE_VECTOR(VTwo);


    // Load constant shared across entire group
    std::vector<Compiler::RegisterPtr> sharedRegisters;
    sharedCodeGenerator.vlui(*VTwo, 2);
    sharedRegisters.push_back(VTwo);
    
    // Figure out best way to represent number of bytes
    // **NOTE** we don't want stride here as 
    // **TODO** support more advanced broadcasting
    auto numBytes = addScalarValue<BroadcastProcess>(
        12, mergedProcess, runtime.getNumDevices(), mergedFields, fieldBaseReg,
        processCodeGenerator, sharedCodeGenerator, scalarRegisterAllocator, sharedRegisters,
        [&runtime](size_t, auto p)
        { 
            assert(p->getSource()->getShape().size() == 1);
            return static_cast<uint32_t>(p->getSource()->getShape()[0] * 2); 
        });


    auto &c = processCodeGenerator;
    
    {
        ALLOCATE_SCALAR(SLLMAddress);
    
        // Load target address into scalar register
        c.lw(*SLLMAddress, *fieldBaseReg, targetFieldOffset);
        
        // Fill vector register with LLM address
        c.vfill(*VAddress, *SLLMAddress);
    }
    
    // Load target address into scalar register
    c.lw(*SDataBuffer, *fieldBaseReg, sourceFieldOffset);

    // If number of bytes is a literal
    Assembler::ScalarRegisterPtr SDataBufferEnd;
    if(std::holds_alternative<int>(numBytes)) {
        SDataBufferEnd = scalarRegisterAllocator.getRegister("SDataBufferEnd");
        c.addi(*SDataBufferEnd, *SDataBufferEnd, std::get<int>(numBytes));
    }
    else {
        SDataBufferEnd = std::get<Assembler::ScalarRegisterPtr>(numBytes);
        c.add(*SDataBufferEnd, *SDataBufferEnd, *SDataBuffer);
    }
    
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
        c.vadd(*VAddress, *VAddress, *VTwo);

        // Loop
        c.bne(*SDataBuffer, *SDataBufferEnd, halfWordLoop);
    }

    return sharedRegisters;
}
}
