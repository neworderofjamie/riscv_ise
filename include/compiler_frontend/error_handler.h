#pragma once

// Standard C++ includes
#include <string_view>

// Parser includes
#include "compiler_frontend/compiler_frontend_export.h"
#include "compiler_frontend/token.h"

//---------------------------------------------------------------------------
// CompilerFrontend::ErrorHandlerBase
//---------------------------------------------------------------------------
namespace CompilerFrontend
{
class ErrorHandlerBase
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void error(size_t line, std::string_view message) = 0;
    virtual void error(const Token &token, std::string_view message) = 0;
};

//---------------------------------------------------------------------------
// CompilerFrontend::ErrorHandler
//---------------------------------------------------------------------------
class COMPILER_FRONTEND_EXPORT ErrorHandler : public ErrorHandlerBase
{
public:
    explicit ErrorHandler(const std::string &context) : m_Context(context), m_Error(false)
    {
    }

    //------------------------------------------------------------------------
    // ErrorHandlerBase virtuals
    //------------------------------------------------------------------------
    virtual void error(size_t line, std::string_view message) final;
    virtual void error(const Token &token, std::string_view message) final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    bool hasError() const { return m_Error; }
    const std::string &getContext() const{ return m_Context; }

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void report(size_t line, std::string_view where, std::string_view message);
    
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::string m_Context;
    bool m_Error;
};

//---------------------------------------------------------------------------
// CompilerFrontend::SingleLineErrorHandler
//---------------------------------------------------------------------------
class COMPILER_FRONTEND_EXPORT SingleLineErrorHandler : public ErrorHandlerBase
{
public:
    SingleLineErrorHandler() : m_Error(false)
    {
    }

    //------------------------------------------------------------------------
    // ErrorHandlerBase virtuals
    //------------------------------------------------------------------------
    virtual void error(size_t line, std::string_view message) final;
    virtual void error(const Token &token, std::string_view message) final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    bool hasError() const { return m_Error; }

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void report(std::string_view where, std::string_view message);
    
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    bool m_Error;
};
}   // namespace CompilerFrontend
