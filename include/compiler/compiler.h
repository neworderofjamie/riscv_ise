#pragma once

// Standard C++ includes
#include <optional>
#include <unordered_map>
#include <variant>

// GeNN includes
#include "type.h"

// Transpiler includes
#include "transpiler/statement.h"
#include "transpiler/typeChecker.h"

// Assembler includes
#include "assembler/register_allocator.h"

// Forward declarations
class CodeGenerator;

//----------------------------------------------------------------------------
// GeNN::CodeGenerator::FeNN::Compiler
//----------------------------------------------------------------------------
using RegisterPtr = std::variant<ScalarRegisterAllocator::RegisterPtr, VectorRegisterAllocator::RegisterPtr>;
using FunctionGenerator = std::function<RegisterPtr(CodeGenerator&, VectorRegisterAllocator&, 
                                                    ScalarRegisterAllocator&, const std::vector<RegisterPtr>&)>;

//----------------------------------------------------------------------------
// EnvironmentBase
//----------------------------------------------------------------------------
class EnvironmentBase
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Define identifier as corresponding register
    virtual void define(const std::string &name, RegisterPtr reg) = 0;
    
    //! Get the register to use for the named identifier
    virtual RegisterPtr getRegister(const std::string &name) = 0;
    
    //! Get the function generator to use for the named identifier
    virtual FunctionGenerator getFunctionGenerator(const std::string &name) = 0;

    //! Get stream to write code within this environment to
    virtual CodeGenerator &getCodeGenerator() = 0;

    //------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------
    RegisterPtr operator[] (const std::string &name)
    {
        return getRegister(name);
    }
};

//----------------------------------------------------------------------------
// EnvironmentInternal
//----------------------------------------------------------------------------
class EnvironmentInternal : public EnvironmentBase
{
public:
    EnvironmentInternal(EnvironmentBase &enclosing)
    :   m_Enclosing(enclosing)
    {
    }

    //---------------------------------------------------------------------------
    // EnvironmentBase virtuals
    //---------------------------------------------------------------------------
    virtual void define(const std::string &name, RegisterPtr reg) final;

    virtual RegisterPtr getRegister(const std::string &name) final;

    virtual FunctionGenerator getFunctionGenerator(const std::string &name) final;

    virtual CodeGenerator &getCodeGenerator() final;

private:
    //---------------------------------------------------------------------------
    // Members
    //---------------------------------------------------------------------------
    EnvironmentBase &m_Enclosing;
    std::unordered_map<std::string, RegisterPtr> m_LocalVariables;
};

//---------------------------------------------------------------------------
// Free functions
//---------------------------------------------------------------------------
void compile(const GeNN::Transpiler::Statement::StatementList &statements, EnvironmentInternal &environment, 
             const GeNN::Type::TypeContext &context, const GeNN::Transpiler::TypeChecker::ResolvedTypeMap &resolvedTypes,
             const std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool,
             std::optional<ScalarRegisterAllocator::RegisterPtr> maskRegister, 
             ScalarRegisterAllocator &scalarRegisterAllocator, VectorRegisterAllocator &vectorRegisterAllocator);
RegisterPtr compile(const GeNN::Transpiler::Expression::ExpressionPtr &expression, EnvironmentInternal &environment, 
                    const GeNN::Type::TypeContext &context, const GeNN::Transpiler::TypeChecker::ResolvedTypeMap &resolvedTypes,
                    const std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool,
                    ScalarRegisterAllocator &scalarRegisterAllocator, VectorRegisterAllocator &vectorRegisterAllocator);
