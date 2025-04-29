#include "backend/backend_fenn.h"

// Standard C++ include
#include <functional>
#include <numeric>
#include <tuple>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>

#include <fast_float/fast_float.h>

// GeNN includes
#include "type.h"

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
#include "backend/memory_allocator.h"

using namespace GeNN;
using namespace GeNN::Transpiler;

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
class EnvironmentExternal : public ::EnvironmentBase, public Transpiler::TypeChecker::EnvironmentBase
{
public:
    explicit EnvironmentExternal(EnvironmentExternal &enclosing)
    :   m_Context{&enclosing, &enclosing, nullptr}
    {
    }

    explicit EnvironmentExternal(::EnvironmentBase &enclosing)
    :   m_Context{nullptr, &enclosing, nullptr}
    {
    }

    explicit EnvironmentExternal(CodeGenerator &os)
    :   m_Context{nullptr, nullptr, &os}
    {
    }


    EnvironmentExternal(const EnvironmentExternal&) = delete;

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual void define(const std::string&, RegisterPtr) override
    {
        throw std::runtime_error("Cannot declare variable in external environment");
    }

    virtual RegisterPtr getRegister(const std::string &name) final
    {
        // If name isn't found in environment
        auto env = m_Environment.find(name);
        if (env == m_Environment.end()) {
            // If context includes a pretty-printing environment, get name from it
            if(std::get<1>(m_Context)) {
                return std::get<1>(m_Context)->getRegister(name);
            }
            // Otherwise, give error
            else {
                throw std::runtime_error("Identifier '" + name + "' undefined"); 
            }
        }
        // Otherwise, get name from payload
        else {
            return std::get<RegisterPtr>(std::get<1>(env->second));
        }
    }

    virtual FunctionGenerator getFunctionGenerator(const std::string &name) final
    {
        // If name isn't found in environment
        auto env = m_Environment.find(name);
        if (env == m_Environment.end()) {
            // If context includes a pretty-printing environment, get name from it
            if(std::get<1>(m_Context)) {
                return std::get<1>(m_Context)->getFunctionGenerator(name);
            }
            // Otherwise, give error
            else {
                throw std::runtime_error("Identifier '" + name + "' undefined"); 
            }
        }
        // Otherwise, get name from payload
        else {
            return std::get<FunctionGenerator>(std::get<1>(env->second));
        }
    }


    //! Get stream to write code within this environment to
    virtual CodeGenerator &getCodeGenerator() final
    {
        // If context includes a code stream
        if(std::get<2>(m_Context)) {
            return *std::get<2>(m_Context);
        }
        // Otherwise
        else {
            // Assert that there is a pretty printing environment
            assert(std::get<1>(m_Context));

            // Return its stream
            return std::get<1>(m_Context)->getCodeGenerator();
        }
    }
   
    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual void define(const Transpiler::Token&, const GeNN::Type::ResolvedType&, 
                        Transpiler::ErrorHandlerBase&) override
    {
        throw std::runtime_error("Cannot declare variable in external environment");
    }

    virtual std::vector<Type::ResolvedType> getTypes(const Transpiler::Token &name, Transpiler::ErrorHandlerBase &errorHandler) final
    {
        // If name isn't found in environment
        auto env = m_Environment.find(name.lexeme);
        if (env == m_Environment.end()) {
            // If context includes a type-checking environment, get type from it
            if(std::get<0>(m_Context)) {
                return std::get<0>(m_Context)->getTypes(name, errorHandler); 
            }
            // Otherwise, give error
            else {
                errorHandler.error(name, "Undefined identifier");
                throw TypeChecker::TypeCheckError();
            }
        }
        // Otherwise, return type of variables
        else {
            return {std::get<0>(env->second)};
        }
    }

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Map a type (for type-checking) and a value (for pretty-printing) to an identifier
    void add(const GeNN::Type::ResolvedType &type, const std::string &name, std::variant<RegisterPtr, FunctionGenerator> value)
    {
         if(!m_Environment.try_emplace(name, type, value).second) {
            throw std::runtime_error("Redeclaration of '" + std::string{name} + "'");
        }
    }
  

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::tuple<Transpiler::TypeChecker::EnvironmentBase*, ::EnvironmentBase*, CodeGenerator*> m_Context;
    std::unordered_map<std::string, std::tuple<Type::ResolvedType, std::variant<RegisterPtr, FunctionGenerator>>> m_Environment;
};

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
}

void compileStatements(const std::vector<Token> &tokens, const Type::TypeContext &typeContext,
                       const std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool,
                       TypeChecker::EnvironmentInternal &typeCheckEnv, EnvironmentInternal &compilerEnv,
                       ErrorHandler &errorHandler, Transpiler::TypeChecker::StatementHandler forEachSynapseTypeCheckHandler,
                       ScalarRegisterAllocator::RegisterPtr maskRegister, 
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
            literalPool, maskRegister, scalarRegisterAllocator, vectorRegisterAllocator);
}

class CodeGeneratorVisitor : public ModelComponentVisitor
{
public:
    CodeGeneratorVisitor(std::shared_ptr<const ProcessGroup> processGroup, 
                         ScalarRegisterAllocator::RegisterPtr timeRegister,
                         CodeGenerator &codeGenerator, 
                         VectorRegisterAllocator &vectorRegisterAllocator, 
                         ScalarRegisterAllocator &scalarRegisterAllocator, 
                         const Model &model)
    :   m_TimeRegister(timeRegister), m_CodeGenerator(codeGenerator), m_VectorRegisterAllocator(vectorRegisterAllocator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator), m_Model(model)
    {
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
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const ProcessGroup>) final
    {
        assert(false);
    }

    virtual void visit(std::shared_ptr<const NeuronUpdateProcess> neuronUpdateProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        // Get fields associated with this process
        const auto &stateFields = m_Model.get().getProcessFields().at(neuronUpdateProcess);

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

        std::unordered_map<std::shared_ptr<const Variable>, ScalarRegisterAllocator::RegisterPtr> varBufferRegisters;
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
                // **TODO** check data structure to determine how this variable is implemented
                // i.e. in lane-local or vector memory and create appropriate address register

                // Allocate scalar register to hold address of variable
                const auto reg = m_ScalarRegisterAllocator.get().getRegister((v.first + "Buffer X").c_str());

                // Add register to map
                varBufferRegisters.try_emplace(v.second, reg);

                // Generate code to load address
                c.lw(*reg, Reg::X0, stateFields.at(v.second));

                // If there are multiple timesteps, multiply timestep by stride and add to register
                // **TODO** modulus number of buffer timesteps/whatever else for delays
                if (v.second->getNumBufferTimesteps() != 1) {
                    ALLOCATE_SCALAR(STmp);

                    c.mul(*STmp, *m_TimeRegister, *SNumVariableBytes);
                    c.add(*reg, *reg, *STmp);
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
                // **TODO** modulus number of buffer timesteps/whatever else for delays
                if (e.second->getNumBufferTimesteps() != 1) {
                    ALLOCATE_SCALAR(STmp);
                    
                    c.mul(*STmp, *m_TimeRegister, *SNumEventBytes);
                    c.add(*reg, *reg, *STmp);
                }
            }
        }
        // Create code generation environment
        EnvironmentExternal env(m_CodeGenerator.get());

        // Loop through neuron parameters
        for(const auto &p : neuronUpdateProcess->getParameters()) {
            // Allocate vector register for parameter
            const auto reg = m_VectorRegisterAllocator.get().getRegister((p.first + " V").c_str());

            // Add to environment
            env.add(p.second->getType(), p.first, reg);

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

            // Generate code to load parameter
            c.vlui(*reg, (uint16_t)integerResult);
        }

        env.add(Type::S8_7, "_zero", literalPool.at(0));
        env.add(Type::ResolvedType::createFunction(Type::S0_15, {}), "fennrand",
                [](auto &env, auto &vectorRegisterAllocator, auto&, auto, const auto&)
                {
                    auto result = vectorRegisterAllocator.getRegister("fennrand = V");
                    env.getCodeGenerator().vrng(*result);
                    return std::make_pair(result, true);
                });

        // Build vectorised neuron loop
        AssemblerUtils::unrollVectorLoopBody(
            env.getCodeGenerator(), m_ScalarRegisterAllocator.get(), 
            neuronUpdateProcess->getNumNeurons(), 4, *varBufferRegisters.begin()->second,
            [this, &env, &eventBufferRegisters, &literalPool, &neuronUpdateProcess, 
             &emitEventFunctionType, &varBufferRegisters]
            (CodeGenerator&, uint32_t r, bool, ScalarRegisterAllocator::RegisterPtr maskReg)
            {
                EnvironmentExternal unrollEnv(env);

                // Loop through variables
                for(const auto &v : neuronUpdateProcess->getVariables()) {
                    // Allocate vector register
                    const auto reg = m_VectorRegisterAllocator.get().getRegister((v.first + " V").c_str());

                    // Add to environment
                    unrollEnv.add(v.second->getType(), v.first, reg);
                        
                    // Load vector from buffer
                    // **TODO** based on data structure, determine if this variable should be loaded from vector or lane local memory
                    unrollEnv.getCodeGenerator().vloadv(*reg, *varBufferRegisters.at(v.second), 64 * r);
                }

                // Loop through neuron event outputs
                for(const auto &e : neuronUpdateProcess->getOutputEvents()) {
                    // Add function to environment to store current mask (inherently which neurons are spiking) to scalar memory
                    unrollEnv.add(emitEventFunctionType, e.first, 
                                  [e, r, &eventBufferRegisters](auto &env, auto&, auto&, auto maskReg, const auto&)
                                  {
                                      // Store buffer
                                      env.getCodeGenerator().sw(*maskReg, *eventBufferRegisters.at(e.second), 4 * r);
                                      return std::make_pair(RegisterPtr{}, false);
                                  });
                }

                // Compile tokens
                // **TODO** pass mask register in here
                {
                    TypeChecker::EnvironmentInternal typeCheckEnv(unrollEnv);
                    EnvironmentInternal compilerEnv(unrollEnv);
                    ErrorHandler errorHandler("Neuron update process '" + neuronUpdateProcess->getName() + "'");        
                    compileStatements(neuronUpdateProcess->getTokens(), {}, literalPool, typeCheckEnv, compilerEnv,
                                      errorHandler, nullptr, nullptr, m_ScalarRegisterAllocator.get(),
                                      m_VectorRegisterAllocator.get());
                }

                // Loop through variables
                for(const auto &v : neuronUpdateProcess->getVariables()) {
                    // Get register
                    const auto reg = std::get<VectorRegisterAllocator::RegisterPtr>(unrollEnv.getRegister(v.first));
                    
                    // Store updated vector back to buffer
                    // **TODO** based on data structure, determine if this variable should be stored to vector or lane local memory
                    unrollEnv.getCodeGenerator().vstore(*reg, *varBufferRegisters.at(v.second), 64 * r);
                }
            },
            [this, &eventBufferRegisters, &neuronUpdateProcess, &varBufferRegisters]
            (CodeGenerator &c, uint32_t numUnrolls)
            {
                // Loop through variables and increment buffers
                // **TODO** based on data structure, determine stride
                for(const auto &v : neuronUpdateProcess->getVariables()) {        
                    const auto bufferReg = varBufferRegisters.at(v.second);
                    c.addi(*bufferReg, *bufferReg, 64 * numUnrolls);
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
        const auto &stateFields = m_Model.get().getProcessFields().at(rngInitProcess);

        // Allocate scalar register to hold address of seed buffer
        ALLOCATE_SCALAR(SReg);

        // Generate code to load address of seed
        c.lw(*SReg, Reg::X0, stateFields.at(rngInitProcess->getSeed()));

        // Load seed into RNG registers
        c.vloadr0(*SReg);
        c.vloadr1(*SReg, 64);
    }

    virtual void visit(std::shared_ptr<const CopyProcess> copyProcess)
    {
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        // **TODO** URAM->BRAM copy assumed

        // Get fields associated with this process
        const auto &stateFields = m_Model.get().getProcessFields().at(copyProcess);

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
        // Make some friendlier-named references
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
        auto &vectorRegisterAllocator = m_VectorRegisterAllocator.get();
        auto &c = m_CodeGenerator.get();

        // Register allocation
        ALLOCATE_SCALAR(SEventBufferEnd);
        ALLOCATE_SCALAR(SWordNStart);
        ALLOCATE_SCALAR(SConst1);
        ALLOCATE_SCALAR(SEventWord);
        ALLOCATE_SCALAR(SEventBuffer);

        // Labels
        Label wordLoop;
        Label bitLoopStart;
        Label bitLoopBody;
        Label bitLoopEnd;
        Label zeroSpikeWord;
        Label wordEnd;

        // Generate code to load address of input event buffer
        const auto &processFields = m_Model.get().getProcessFields();
        c.lw(*SEventBuffer, Reg::X0, processFields.at(processes.front()).at(processes.front()->getInputEvents()));

        {
            // Load immediate with number of words required to represent one timestep of input spikes
            ALLOCATE_SCALAR(STmp);
            c.li(*STmp, ceilDivide(processes.front()->getNumSourceNeurons(), 32) * 4);

            // If there are multiple timesteps, multiply timestep by stride and add to event start pointer
            // **TODO** modulus number of buffer timesteps/whatever else for delays
            if (processes.front()->getInputEvents()->getNumBufferTimesteps() != 1) {
                ALLOCATE_SCALAR(STmp2);
                c.mul(*STmp2, *m_TimeRegister, *STmp);
                c.add(*SEventBuffer, *SEventBuffer, *STmp2);
            }

            // Get address of end of input event buffer        
            c.add(*SEventBufferEnd, *STmp, *SEventBuffer);
        }

        // Loop through postsynaptic targets
        std::vector<std::pair<ScalarRegisterAllocator::RegisterPtr, ScalarRegisterAllocator::RegisterPtr>> sTargetStrideBufferRegs;
        for(const auto &p : processes) {
            // Allocate scalar registers
            auto bufferStartReg = scalarRegisterAllocator.getRegister("STargetBuffer = X");
            auto strideReg = scalarRegisterAllocator.getRegister("SStride = X");

            // Load addresses of targets
            c.lw(*bufferStartReg, Reg::X0, processFields.at(p).at(p->getTarget()));

            // Calculate stride and load as immediate
            c.li(*strideReg, ceilDivide(p->getNumTargetNeurons(), 32) * 64);

            // Add scalar registers to vector
            sTargetStrideBufferRegs.emplace_back(bufferStartReg, strideReg);
        }
    
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
            c.lw(*SEventWord, *SEventBuffer);
            c.addi(*SEventBuffer, *SEventBuffer, 4);

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
                for(size_t i = 0; i < processes.size(); i++) {
                    const auto p = processes[i];
                    const auto &stateFields = processFields.at(p);

                    ScalarRegisterAllocator::RegisterPtr targetReg;
                    ScalarRegisterAllocator::RegisterPtr strideReg;
                    std::tie(targetReg, strideReg) = sTargetStrideBufferRegs[i];

                    // Reset target pointer
                    c.lw(*targetReg, Reg::X0, stateFields.at(p->getTarget()));

                    // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
                    // **TODO** multiply
                    ALLOCATE_SCALAR(SWeightBuffer);
                    c.lw(*SWeightBuffer, Reg::X0, stateFields.at(p->getWeight()));
                    {
                        ALLOCATE_SCALAR(STemp);
                        c.mul(*STemp, *SN, *strideReg);
                        c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
                    }

                    // Load weight and Isyn
                    //if(t.debug) {
                    //    c.ebreak();
                    //}

                    ALLOCATE_VECTOR(VWeight);
                    ALLOCATE_VECTOR(VTarget1);
                    ALLOCATE_VECTOR(VTarget2);
                    ALLOCATE_VECTOR(VTargetNew);

                    // Preload first ISyn to avoid stall
                    c.vloadv(*VTarget1, *targetReg, 0);

                    AssemblerUtils::unrollVectorLoopBody(
                        c, scalarRegisterAllocator, p->getNumTargetNeurons(), 4, *targetReg,
                        [&targetReg, SWeightBuffer, VWeight, VTarget1, VTarget2, VTargetNew]
                        (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr maskReg)
                        {
                            // Load vector of weights
                            c.vloadv(*VWeight, *SWeightBuffer, r * 64);

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
                        [&targetReg, SWeightBuffer]
                        (CodeGenerator &c, uint32_t numUnrolls)
                        {
                            // Increment pointers 
                            c.addi(*targetReg, *targetReg, 64 * numUnrolls);
                            c.addi(*SWeightBuffer, *SWeightBuffer, 64 * numUnrolls);
                        });
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
            c.bne(*SEventBuffer, *SEventBufferEnd, wordLoop);

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

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_map<std::shared_ptr<const EventContainer>, std::vector<std::shared_ptr<const EventPropagationProcess>>> m_EventPropagationProcesses;
    ScalarRegisterAllocator::RegisterPtr m_TimeRegister;
    std::reference_wrapper<CodeGenerator> m_CodeGenerator;
    std::reference_wrapper<VectorRegisterAllocator> m_VectorRegisterAllocator;
    std::reference_wrapper<ScalarRegisterAllocator> m_ScalarRegisterAllocator;
    std::reference_wrapper<const Model> m_Model;
};

//----------------------------------------------------------------------------
// VariableImplementerVisitor
//----------------------------------------------------------------------------
class VariableImplementerVisitor : public ModelComponentVisitor
{
public:
    VariableImplementerVisitor(std::shared_ptr<const Variable> variable, const Model::StateProcesses::mapped_type &processes)
    :   m_Variable(variable), m_LLMCompatible(true), m_URAMCompatible(true), m_BRAMCompatible(true)
    {
        // Visit all processes
        for(auto p : processes) {
            p->accept(*this);
        }
    }

    bool isLLMCompatible() const{ return m_LLMCompatible; }
    bool isURAMCompatible() const{ return m_URAMCompatible; }
    bool isBRAMCompatible() const{ return m_BRAMCompatible; }

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

            if(!m_URAMCompatible) {
                throw std::runtime_error("Event propagation process '" + eventPropagationProcess->getName() 
                                        + "' weight array '" + eventPropagationProcess->getWeight()->getName()
                                        + "' shared with incompatible processes");
            }
        }
        // Otherwise, if variable's target, it can either be located in URAM or LLM
        // **TODO** if sparse/delayed only in LLM
        else if(m_Variable == eventPropagationProcess->getTarget()) {
            m_BRAMCompatible = false;

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

        if(!m_URAMCompatible) {
            throw std::runtime_error("RNG init process '" + rngInitProcess->getName() 
                                     + "' seed array '" + rngInitProcess->getSeed()->getName()
                                     + "' shared with incompatible processes");
        }
    }

    virtual void visit(std::shared_ptr<const CopyProcess> copyProcess)
    {
        // If variable is source, it can only be located in URAM
        if(m_Variable == copyProcess->getSource()) {
            m_LLMCompatible = false;
            m_BRAMCompatible = false;

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


    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const Variable> m_Variable;
    bool m_LLMCompatible;
    bool m_URAMCompatible;
    bool m_BRAMCompatible;
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

//----------------------------------------------------------------------------
// BackendFeNN
//------------------------------------------------------------------------
std::vector<uint32_t> BackendFeNN::generateSimulationKernel(const std::vector<std::shared_ptr<const ProcessGroup>> &timestepProcessGroups,
                                                            const std::vector <std::shared_ptr<const ProcessGroup>> &endProcessGroups,
                                                            uint32_t numTimesteps, const Model &model) const
{
    uint32_t readyFlagPtr = 0;
    return AssemblerUtils::generateStandardKernel(
        shouldGenerateSimulationKernels(), readyFlagPtr,
        [=, &model]
        (CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Register allocation
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);

            // Labels
            Label timeLoop;
         
            // Set timestep range and load ready flag pointer
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);

            // Loop over time
            c.L(timeLoop);
            {
                // Visit timestep process group
                for (const auto &p : timestepProcessGroups) {
                    CodeGeneratorVisitor visitor(p, STime, c, vectorRegisterAllocator,
                                                 scalarRegisterAllocator, model);
                }
                
                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }

            // Visit end process group
            for (const auto &p : endProcessGroups) {
                CodeGeneratorVisitor visitor(p, nullptr, c, vectorRegisterAllocator,
                                             scalarRegisterAllocator, model);
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
        [=, &model]
        (CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Visit process groups
            for (const auto &p : processGroups) {
                CodeGeneratorVisitor visitor(p, nullptr, c, vectorRegisterAllocator,
                                             scalarRegisterAllocator, model);
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
    VariableImplementerVisitor visitor(variable, processes);
    if(visitor.isURAMCompatible()) {
        return createURAMArray(variable->getType(), count, state);
    }
    // Otherwise, create BRAM array if variable can be implemented there
    else if(visitor.isBRAMCompatible()) {
        return createBRAMArray(variable->getType(), count, state);
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
    // Event containers are always implemented as BRAM bitfields
    const size_t numSpikeWords = ceilDivide(eventContainer->getShape().getFlattenedSize(), 32) * eventContainer->getNumBufferTimesteps();
    return createBRAMArray(GeNN::Type::Uint32, numSpikeWords, state);
}