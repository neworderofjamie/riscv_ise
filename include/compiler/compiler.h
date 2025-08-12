#pragma once

// Standard C++ includes
#include <optional>
#include <unordered_map>
#include <variant>

// GeNN includes
#include "type.h"

// GeNN transpiler includes
#include "transpiler/statement.h"
#include "transpiler/typeChecker.h"

// Assembler includes
#include "assembler/register_allocator.h"

// Compiler includes
#include "compiler/compiler_export.h"

// Forward declarations
class CodeGenerator;
class EnvironmentBase;

//----------------------------------------------------------------------------
// GeNN::CodeGenerator::FeNN::Compiler
//----------------------------------------------------------------------------
using RegisterPtr = std::variant<ScalarRegisterAllocator::RegisterPtr, VectorRegisterAllocator::RegisterPtr>;
using FunctionGenerator = std::function<std::pair<RegisterPtr, bool>(EnvironmentBase&, VectorRegisterAllocator&,
                                                                     ScalarRegisterAllocator&, 
                                                                     ScalarRegisterAllocator::RegisterPtr, 
                                                                     const std::vector<RegisterPtr>&)>;
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
    virtual FunctionGenerator getFunctionGenerator(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) = 0;

    //! Get stream to write code within this environment to
    virtual CodeGenerator &getCodeGenerator() = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    ScalarRegisterAllocator::RegisterPtr getScalarRegister(const std::string &name)
    {
        return std::get<ScalarRegisterAllocator::RegisterPtr>(getRegister(name));
    }

    VectorRegisterAllocator::RegisterPtr getVectorRegister(const std::string &name)
    {
        return std::get<VectorRegisterAllocator::RegisterPtr>(getRegister(name));
    }

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
class COMPILER_EXPORT EnvironmentInternal : public EnvironmentBase
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

    virtual FunctionGenerator getFunctionGenerator(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) final;

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
COMPILER_EXPORT void compile(const GeNN::Transpiler::Statement::StatementList &statements, EnvironmentInternal &environment, 
                             const GeNN::Type::TypeContext &context, const GeNN::Transpiler::TypeChecker::ResolvedTypeMap &resolvedTypes,
                             GeNN::Transpiler::ErrorHandlerBase &errorHandler, const std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool,
                             ScalarRegisterAllocator::RegisterPtr maskRegister, 
                             ScalarRegisterAllocator &scalarRegisterAllocator, VectorRegisterAllocator &vectorRegisterAllocator);
