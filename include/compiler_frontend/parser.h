#pragma once

// Standard C++ includes
#include <memory>
#include <set>
#include <vector>

// Compiler frontend includes
#include "compiler_frontend/expression.h"
#include "compiler_frontend/compiler_frontend_export.h"
#include "compiler_frontend/statement.h"
#include "compiler_frontend/token.h"
#include "compiler_frontend/type.h"

// Forward declarations
namespace CompilerFrontend
{
class ErrorHandlerBase;
}

//---------------------------------------------------------------------------
// CompilerFrontend::ParseError
//---------------------------------------------------------------------------
namespace CompilerFrontend::Parser
{
class ParseError : public std::runtime_error
{
public:
    ParseError() : std::runtime_error("")
    {
    }
};

//! Parse expression from tokens
COMPILER_FRONTEND_EXPORT Expression::ExpressionPtr parseExpression(const std::vector<Token> &tokens, ErrorHandlerBase &errorHandler);

//! Parse block item list from tokens
/*! Block item lists are function body scope list of statements */
COMPILER_FRONTEND_EXPORT Statement::StatementList parseBlockItemList(const std::vector<Token> &tokens, ErrorHandlerBase &errorHandler);

//! Parse type from tokens
COMPILER_FRONTEND_EXPORT const Type::ResolvedType parseNumericType(const std::vector<Token> &tokens, ErrorHandlerBase &errorHandler);

}   // MiniParse::MiniParse
