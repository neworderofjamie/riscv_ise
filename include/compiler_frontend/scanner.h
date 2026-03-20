#pragma once

// Standard C++ includes
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

// Compiler frontend includes
#include "compiler_frontend/compiler_frontend_export.h"
#include "compiler_frontend/token.h"
#include "compiler_frontend/type.h"

// Forward declarations
namespace CompilerFrontend
{
class ErrorHandlerBase;
}

//---------------------------------------------------------------------------
// CompilerFrontend::Scanner::Error
//---------------------------------------------------------------------------
namespace CompilerFrontend::Scanner
{
COMPILER_FRONTEND_EXPORT std::vector<Token> scanSource(const std::string_view &source, 
                                                       ErrorHandlerBase &errorHandler);

}   // namespace Scanner
