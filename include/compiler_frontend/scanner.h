#pragma once

// Standard C++ includes
#include <string_view>
#include <vector>

// Compiler frontend includes
#include "compiler_frontend/compiler_frontend_export.h"
#include "compiler_frontend/token.h"

// Forward declarations
namespace CompilerFrontend
{
class ErrorHandlerBase;
namespace Type
{
struct ResolvedType;
}
}

//---------------------------------------------------------------------------
// CompilerFrontend::Scanner::Error
//---------------------------------------------------------------------------
namespace CompilerFrontend::Scanner
{
COMPILER_FRONTEND_EXPORT std::vector<Token> scanSource(const std::string_view &source, 
                                                       ErrorHandlerBase &errorHandler,
                                                       const Type::ResolvedType &defaultRealLiteralType);

}   // namespace Scanner
