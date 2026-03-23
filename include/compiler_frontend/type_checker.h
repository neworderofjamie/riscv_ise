#pragma once

// Standard C++ includes
#include <functional>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

// Compiler frontend includes
#include "compiler_frontend/compiler_frontend_export.h"
#include "compiler_frontend/statement.h"
#include "compiler_frontend/type.h"

// Forward declarations
namespace CompilerFrontend
{
class ErrorHandlerBase;
struct Token;
}

//---------------------------------------------------------------------------
// CompilerFrontend::TypeChecker::TypeCheckError
//---------------------------------------------------------------------------
namespace CompilerFrontend::TypeChecker
{
class TypeCheckError : public std::runtime_error
{
public:
    TypeCheckError() : std::runtime_error("")
    {
    }
};

//---------------------------------------------------------------------------
// CompilerFrontend::TypeChecker::EnvironmentBase
//---------------------------------------------------------------------------
class COMPILER_FRONTEND_EXPORT EnvironmentBase
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void define(const Token &name, const Type::ResolvedType &type, ErrorHandlerBase &errorHandler) = 0;
    virtual std::vector<Type::ResolvedType> getTypes(const Token &name, ErrorHandlerBase &errorHandler) = 0;

    //---------------------------------------------------------------------------
    // Public API
    //---------------------------------------------------------------------------
    Type::ResolvedType getType(const Token &name, ErrorHandlerBase &errorHandler);
};

//---------------------------------------------------------------------------
// CompilerFrontend::TypeChecker::EnvironmentInternal
//---------------------------------------------------------------------------
class COMPILER_FRONTEND_EXPORT EnvironmentInternal : public EnvironmentBase
{
public:
    EnvironmentInternal(EnvironmentBase &enclosing)
    :   m_Enclosing(enclosing)
    {
    }

    //---------------------------------------------------------------------------
    // EnvironmentBase virtuals
    //---------------------------------------------------------------------------
    virtual void define(const Token &name, const Type::ResolvedType &type, ErrorHandlerBase &errorHandler) final;
    virtual std::vector<Type::ResolvedType> getTypes(const Token &name, ErrorHandlerBase &errorHandler) final;

private:
    //---------------------------------------------------------------------------
    // Members
    //---------------------------------------------------------------------------
    EnvironmentBase &m_Enclosing;
    std::unordered_map<std::string, Type::ResolvedType> m_Types;
};


//---------------------------------------------------------------------------
// Typedefines
//---------------------------------------------------------------------------
typedef std::unordered_map<const Expression::Base*, Type::ResolvedType> ResolvedTypeMap;
typedef std::function<void(EnvironmentBase&, ErrorHandlerBase&)> StatementHandler;

//---------------------------------------------------------------------------
// Free functions
//---------------------------------------------------------------------------
COMPILER_FRONTEND_EXPORT ResolvedTypeMap typeCheck(const Statement::StatementList &statements, EnvironmentInternal &environment, 
                                                   ErrorHandlerBase &errorHandler, StatementHandler forEachSynapseHandler = nullptr);

COMPILER_FRONTEND_EXPORT ResolvedTypeMap typeCheck(const Expression::Base *expression, EnvironmentInternal &environment, 
                                                   ErrorHandlerBase &errorHandler);
}   // namespace CompilerFrontend::TypeChecker
