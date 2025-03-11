// Standard C++ include
#include <functional>
#include <unordered_map>
#include <tuple>
#include <vector>

// Standard C includes
#include <cstdint>

#include <fast_float/fast_float.h>

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
#include "compiler/parameter.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"

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
            return std::get<1>(env->second);
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
    virtual void define(const Transpiler::Token &name, const GeNN::Type::ResolvedType &type, 
                        Transpiler::ErrorHandlerBase &errorHandler) override
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
    void add(const GeNN::Type::ResolvedType &type, const std::string &name, RegisterPtr reg)
    {
         if(!m_Environment.try_emplace(name, type, reg).second) {
            throw std::runtime_error("Redeclaration of '" + std::string{name} + "'");
        }
    }
  

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::tuple<Transpiler::TypeChecker::EnvironmentBase*, ::EnvironmentBase*, CodeGenerator*> m_Context;
    std::unordered_map<std::string, std::tuple<Type::ResolvedType, RegisterPtr>> m_Environment;
};

void updateLiteralPool(const std::vector<Token> &tokens, const Type::TypeContext &typeContext, 
                       VectorRegisterAllocator &vectorRegisterAllocator, 
                       std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool)
{
    // Loop through tokens
    const auto &scalarType = typeContext.at("scalar");
    for(const auto &t : tokens) {
        // If token is a number
        if(t.type == Token::Type::NUMBER) {
            // Get start and end of lexeme
            const auto lexeme = t.lexeme;
            const char *lexemeBegin = lexeme.c_str();
            const char *lexemeEnd = lexemeBegin + lexeme.size();
            
            // Get it's type (scalar if not specified)
            int64_t integerResult;
            const auto &numericType = t.numberType.value_or(scalarType).getNumeric();
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
                       std::optional<ScalarRegisterAllocator::RegisterPtr> maskRegister, 
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
class ModelVisitor : public ModelComponentVisitor
{
public:
    ModelVisitor(CodeGenerator &codeGenerator, VectorRegisterAllocator &vectorRegisterAllocator, 
                 ScalarRegisterAllocator &scalarRegisterAllocator)
    :   m_CodeGenerator(codeGenerator), m_VectorRegisterAllocator(vectorRegisterAllocator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator)
    {
    
    }
    // ModelComponentVisitor virtuals
    virtual void visit(const SpikeInputProcess &spikeInputProcess) final
    {

    }

    virtual void visit(const NeuronUpdateProcess &neuronUpdateProcess)
    {
        EnvironmentExternal env(m_CodeGenerator.get());

        // Build literal pool
        std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> literalPool;
        updateLiteralPool(neuronUpdateProcess.getTokens(), {}, m_VectorRegisterAllocator.get(), literalPool);
        
        // Allocate registers for Isyn and zero
        env.add(Type::S8_7, "_zero", literalPool.at(0));

        // Allocate registers for neuron variables
        for(const auto &v : neuronUpdateProcess.getVariables()) {
            env.add(v.second->getType(), v.first, m_VectorRegisterAllocator.get().getRegister((v.first + " V").c_str()));
        }

        // Allocate registers for neuron parameters
        for(const auto &p : neuronUpdateProcess.getParameters()) {
            env.add(p.second->getType(), p.first, m_VectorRegisterAllocator.get().getRegister((p.first + " V").c_str()));
        }

        // TODO emit spike functions

        // Resolve types within one scope
        TypeChecker::EnvironmentInternal typeCheckEnv(env);
        EnvironmentInternal compilerEnv(env);
        ErrorHandler errorHandler("Errors");
        
        // Compile tokens
        compileStatements(neuronUpdateProcess.getTokens(), {}, literalPool, typeCheckEnv, compilerEnv,
                          errorHandler, nullptr, std::nullopt, m_ScalarRegisterAllocator.get(),
                          m_VectorRegisterAllocator.get());
    }

    virtual void visit(const EventPropagationProcess &eventPropagationProcess)
    {

    }

private:
    std::reference_wrapper<CodeGenerator> m_CodeGenerator;
    std::reference_wrapper<VectorRegisterAllocator> m_VectorRegisterAllocator;
    std::reference_wrapper<ScalarRegisterAllocator> m_ScalarRegisterAllocator;
};
}

std::vector<uint32_t> generateSimulationKernel(const ProcessGroup *synapseProcessGroup, 
                                               const ProcessGroup *neuronProcessGroup,
                                               uint32_t numTimesteps, bool simulate)
{
    uint32_t readyFlagPtr = 0;
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Register allocation
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);

            // Labels
            Label timeLoop;
         
            // Set timestep range and load ready flag pointer
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);

            // Create code-generation visitor
            ModelVisitor visitor(c, vectorRegisterAllocator, scalarRegisterAllocator);

            // Loop over time
            c.L(timeLoop);
            {
                // Visit synapse process group
                synapseProcessGroup->accept(visitor);

                // Visit neuron process group
                neuronProcessGroup->accept(visitor);

                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }
        });
}