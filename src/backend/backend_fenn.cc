#include "backend/backend_fenn.h"

// Standard C++ include
#include <functional>
#include <unordered_map>
#include <tuple>
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
    CodeGeneratorVisitor(std::shared_ptr<const ProcessGroup> processGroup, CodeGenerator &codeGenerator, 
                         VectorRegisterAllocator &vectorRegisterAllocator, 
                         ScalarRegisterAllocator &scalarRegisterAllocator, 
                         const Model &model)
    :   m_CodeGenerator(codeGenerator), m_VectorRegisterAllocator(vectorRegisterAllocator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator), m_Model(model)
    {
        // Visit all the processes
        for(const auto p : processGroup->getProcesses()) {
            p->accept(*this);
        }

        // Loop through all grouped event propagation processes
        for(const auto e : m_EventPropagationProcesses) {
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

        // Loop through neuron variables
        std::unordered_map<std::shared_ptr<const Variable>, ScalarRegisterAllocator::RegisterPtr> varBufferRegisters;
        for(const auto v : neuronUpdateProcess->getVariables()) {
            // **TODO** check data structure to determine how this variable is implemented
            // i.e. in lane-local or vector memory and create appropriate address register

            // Allocate scalar register to hold address of variable
            const auto reg = m_ScalarRegisterAllocator.get().getRegister((v.first + "Buffer X").c_str());

            // Add register to map
            varBufferRegisters.try_emplace(v.second, reg);

            // Generate code to load address
            c.lw(*reg, Reg::X0, stateFields.at(v.second));
        }

        // Loop through neuron event outputs
        std::unordered_map<std::shared_ptr<const EventContainer>, ScalarRegisterAllocator::RegisterPtr> eventBufferRegisters;
        for(const auto e : neuronUpdateProcess->getOutputEvents()) {
            // Allocate scalar register to hold address of variable
            const auto reg = m_ScalarRegisterAllocator.get().getRegister((e.first + "Buffer X").c_str());

            // Add register to map
            eventBufferRegisters.try_emplace(e.second, reg);

            // Generate code to load address
            c.lw(*reg, Reg::X0, stateFields.at(e.second));
        }
        
        // Create code generation environment
        EnvironmentExternal env(m_CodeGenerator.get());

        // Loop through neuron parameters
        for(const auto p : neuronUpdateProcess->getParameters()) {
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

            // Check integer is in range and insert in pool
            assert(integerResult >= std::numeric_limits<int16_t>::min());
            assert(integerResult <= std::numeric_limits<int16_t>::max());

            // Generate code to load parameter
            c.vlui(*reg, (uint16_t)integerResult);
        }

        env.add(Type::S8_7, "_zero", literalPool.at(0));

        // Build vectorised neuron loop
        AssemblerUtils::unrollVectorLoopBody(
            env.getCodeGenerator(), m_ScalarRegisterAllocator.get(), 
            neuronUpdateProcess->getNumNeurons(), 4, *varBufferRegisters.begin()->second,
            [this, &env, &eventBufferRegisters, &literalPool, &neuronUpdateProcess, 
             &emitEventFunctionType, &varBufferRegisters]
            (CodeGenerator &c, uint32_t r, bool, ScalarRegisterAllocator::RegisterPtr maskReg)
            {
                EnvironmentExternal unrollEnv(env);

                // Loop through variables
                for(const auto v : neuronUpdateProcess->getVariables()) {
                    // Allocate vector register
                    const auto reg = m_VectorRegisterAllocator.get().getRegister((v.first + " V").c_str());

                    // Add to environment
                    unrollEnv.add(v.second->getType(), v.first, reg);
                        
                    // Load vector from buffer
                    // **TODO** based on data structure, determine if this variable should be loaded from vector or lane local memory
                    unrollEnv.getCodeGenerator().vloadv(*reg, *varBufferRegisters.at(v.second), 64 * r);
                }

                // Loop through neuron event outputs
                for(const auto e : neuronUpdateProcess->getOutputEvents()) {
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
                {
                    TypeChecker::EnvironmentInternal typeCheckEnv(unrollEnv);
                    EnvironmentInternal compilerEnv(unrollEnv);
                    ErrorHandler errorHandler("Neuron update process '" + neuronUpdateProcess->getName() + "'");        
                    compileStatements(neuronUpdateProcess->getTokens(), {}, literalPool, typeCheckEnv, compilerEnv,
                                      errorHandler, nullptr, nullptr, m_ScalarRegisterAllocator.get(),
                                      m_VectorRegisterAllocator.get());
                }

                // Loop through variables
                for(const auto v : neuronUpdateProcess->getVariables()) {
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
                for(const auto v : neuronUpdateProcess->getVariables()) {        
                    const auto bufferReg = varBufferRegisters.at(v.second);
                    c.addi(*bufferReg, *bufferReg, 64 * numUnrolls);
                }

                // Loop through output events and increment buffers
                for(const auto e : neuronUpdateProcess->getOutputEvents()) {
                    const auto bufferReg = eventBufferRegisters.at(e.second);
                    c.addi(*bufferReg, *bufferReg, 4 * numUnrolls);
                 }
            });
        
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

        // If literal is provided for start of presynapric spike buffer, allocate register and load immediate into it
        /*ScalarRegisterAllocator::RegisterPtr SEventBuffer;
        if(std::holds_alternative<uint32_t>(preSpikePtr)) {
            SEventBuffer = scalarRegisterAllocator.getRegister("SEventBuffer = X");
            c.li(*SEventBuffer, std::get<uint32_t>(preSpikePtr));
        }
        // Otherwise, use pointer register directly
        else {
            SEventBuffer = std::get<ScalarRegisterAllocator::RegisterPtr>(preSpikePtr);
        }*/
        // Generate code to load address of input event buffer
        const auto &processFields = m_Model.get().getProcessFields();
        c.lw(*SEventBuffer, Reg::X0, processFields.at(processes.front()).at(processes.front()->getInputEvents()));
        
        // Get address of end of input event buffer
        c.li(*SEventBufferEnd, (ceilDivide(processes.front()->getNumSourceNeurons(), 32) * 4));
        c.add(*SEventBufferEnd, *SEventBufferEnd, *SEventBuffer);
    

        // Loop through postsynaptic targets
        std::vector<std::pair<ScalarRegisterAllocator::RegisterPtr, ScalarRegisterAllocator::RegisterPtr>> sTargetStrideBufferRegs;
        for(const auto p : processes) {
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
    std::reference_wrapper<CodeGenerator> m_CodeGenerator;
    std::reference_wrapper<VectorRegisterAllocator> m_VectorRegisterAllocator;
    std::reference_wrapper<ScalarRegisterAllocator> m_ScalarRegisterAllocator;
    std::reference_wrapper<const Model> m_Model;
};
}

//----------------------------------------------------------------------------
// BackendFeNN
//------------------------------------------------------------------------
std::vector<uint32_t> BackendFeNN::generateSimulationKernel(std::shared_ptr<const ProcessGroup> synapseProcessGroup, 
                                                            std::shared_ptr<const ProcessGroup> neuronProcessGroup,
                                                            uint32_t numTimesteps, bool simulate,
                                                            const Model &model) const
{
    uint32_t readyFlagPtr = 0;
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
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
                // Visit synapse process group
                CodeGeneratorVisitor synapseVisitor(synapseProcessGroup, c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                    model);

                // Visit neuron process group
                CodeGeneratorVisitor neuronVisitor(neuronProcessGroup, c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                   model);

                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }
        });
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const Variable> variable,
                                                    StateBase *state) const
{
    // **TODO** should also take vector of processes that access variables.
    // **TODO** if target of sparse or delayed event propoagation process, implement in L.L.M.
    return createURAMArray(variable->getType(), variable->getShape().getFlattenedSize(), state);
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNN::createArray(std::shared_ptr<const EventContainer> eventContainer,
                                                    StateBase *state) const
{
    // Event containers are always implemented as BRAM bitfields
    const size_t numSpikeWords = ceilDivide(eventContainer->getShape().getFlattenedSize(), 32);
    return createBRAMArray(GeNN::Type::Uint32, numSpikeWords, state);
}