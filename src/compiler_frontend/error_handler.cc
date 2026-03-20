#include "compiler_frontend/error_handler.h"

// Common includes
#include "common/logging.h"

//----------------------------------------------------------------------------
// CompilerFrontend::ErrorHandler
//----------------------------------------------------------------------------
namespace CompilerFrontend
{
void ErrorHandler::error(size_t line, std::string_view message)
{
    report(line, "", message);
}
//----------------------------------------------------------------------------  
void ErrorHandler::error(const Token &token, std::string_view message)
{
    if(token.type == Token::Type::END_OF_FILE) {
        report(token.line, " at end", message);
    }
    else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}
//----------------------------------------------------------------------------
void ErrorHandler::report(size_t line, std::string_view where, std::string_view message) 
{
    LOGE_COMPILER_FRONTEND << "[" << m_Context << ", line " << line << "] Error" << where << ": " << message;
    m_Error = true;
}

//----------------------------------------------------------------------------
// CompilerFrontend::SingleLineErrorHandler
//----------------------------------------------------------------------------
void SingleLineErrorHandler::error(size_t, std::string_view message)
{
    report("", message);
}
//----------------------------------------------------------------------------  
void SingleLineErrorHandler::error(const Token &token, std::string_view message)
{
    if(token.type == Token::Type::END_OF_FILE) {
        report(" at end", message);
    }
    else {
        report(" at '" + token.lexeme + "'", message);
    }
}
//----------------------------------------------------------------------------
void SingleLineErrorHandler::report(std::string_view where, std::string_view message) 
{
    LOGE_COMPILER_FRONTEND << "Error" << where << ": " << message;
    m_Error = true;
}
}   // namespace CompilerFrontend
