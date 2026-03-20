#pragma once

// Standard C++ includes
#include <unordered_map>

// Compiler frontend includes
#include "compiler_frontend/type_checker.h"

// FeNN assembler includes
#include "fenn/assembler/assembler.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/fields.h"

// Forward declarations
namespace FeNN::Assembler
{
class ScalarRegisterAllocator;
class VectorRegisterAllocator;
}

//----------------------------------------------------------------------------
// FeNN::Backend::EnvironmentExternalBase
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class EnvironmentExternalBase : public Compiler::EnvironmentBase, public CompilerFrontend::TypeChecker::EnvironmentBase
{
public:
    explicit EnvironmentExternalBase(EnvironmentExternalBase &enclosing)
    :   m_Context{&enclosing, &enclosing, nullptr}
    {
    }

    explicit EnvironmentExternalBase(Compiler::EnvironmentBase &enclosing)
    :   m_Context{nullptr, &enclosing, nullptr}
    {
    }

    explicit EnvironmentExternalBase(Assembler::CodeGenerator &c)
    :   m_Context{nullptr, nullptr, &c}
    {
    }

    EnvironmentExternalBase(EnvironmentExternalBase &enclosing, Assembler::CodeGenerator &c)
    :   m_Context{&enclosing, &enclosing, &c}
    {
    }

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual void define(const std::string &, Compiler::RegisterPtr) override final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual void define(const CompilerFrontend::Token &, const CompilerFrontend::Type::ResolvedType &,
                        CompilerFrontend::ErrorHandlerBase &) override final;

protected:
    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    Compiler::EnvironmentItem getContextItem(const std::string &name, 
                                             std::optional<CompilerFrontend::Type::ResolvedType> type = std::nullopt) const;

    //! Get vector of types from context if it provides this functionality
    std::vector<CompilerFrontend::ResolvedType> getContextTypes(const CompilerFrontend::Token &name, 
                                                                CompilerFrontend::ErrorHandlerBase &errorHandler)  const;

    //! Get code generator exposed by context
    Assembler::CodeGenerator &getContextCodeGenerator() const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::tuple<CompilerFrontend::TypeChecker::EnvironmentBase*, 
               Compiler::EnvironmentBase*, 
               Assembler::CodeGenerator*> m_Context;
};

//----------------------------------------------------------------------------
// FeNN::Backend::EnvironmentExternal
//----------------------------------------------------------------------------
class EnvironmentExternal : public EnvironmentExternalBase
{
public:
    explicit EnvironmentExternal(EnvironmentExternalBase &enclosing)
    :   EnvironmentExternalBase(enclosing)
    {
    }

    explicit EnvironmentExternal(EnvironmentExternal &enclosing)
        : EnvironmentExternalBase(enclosing)
    {
    }

    explicit EnvironmentExternal(Compiler::EnvironmentBase &enclosing)
    :   EnvironmentExternalBase(enclosing)
    {
    }

    explicit EnvironmentExternal(Assembler::CodeGenerator &c)
    :   EnvironmentExternalBase(c)
    {
    }

    EnvironmentExternal(EnvironmentExternalBase &enclosing, Assembler::CodeGenerator &c)
    :   EnvironmentExternalBase(enclosing, c)
    {
    }

    EnvironmentExternal(const EnvironmentExternal&) = delete;

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual Compiler::EnvironmentItem getItem(const std::string &name, 
                                              std::optional<CompilerFrontend::Type::ResolvedType> type = std::nullopt) override final;

    virtual Assembler::CodeGenerator &getCodeGenerator() override final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual std::vector<CompilerFrontend::Type::ResolvedType> getTypes(const CompilerFrontend::Token &name, 
                                                                       CompilerFrontend::ErrorHandlerBase &errorHandler) override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Map a type (for type-checking) and a value (for pretty-printing) to an identifier
    void add(const CompilerFrontend::ResolvedType &type, const std::string &name, Compiler::EnvironmentItem value);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_map<std::string, std::tuple<CompilerFrontend::Type::ResolvedType, Compiler::EnvironmentItem>> m_Environment;
};


//----------------------------------------------------------------------------
// FeNN::Backend::EnvironmentLibrary
//----------------------------------------------------------------------------
class EnvironmentLibrary : public EnvironmentExternalBase
{
public:
    using Library = std::unordered_multimap<std::string, std::pair<CompilerFrontend::Type::ResolvedType, Compiler::FunctionGenerator>>;

    explicit EnvironmentLibrary(EnvironmentExternalBase &enclosing, const Library &library)
    :   EnvironmentExternalBase(enclosing), m_Library(library)
    {
    }

    explicit EnvironmentLibrary(Compiler::EnvironmentBase &enclosing, const Library &library)
    :   EnvironmentExternalBase(enclosing), m_Library(library)
    {
    }

    explicit EnvironmentLibrary(Assembler::CodeGenerator &c, const Library &library)
    :   EnvironmentExternalBase(c), m_Library(library)
    {
    }

    EnvironmentLibrary(EnvironmentExternalBase &enclosing, Assembler::CodeGenerator &c, const Library &library)
    :   EnvironmentExternalBase(enclosing, c), m_Library(library)
    {
    }

    EnvironmentLibrary(const EnvironmentLibrary &) = delete;

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual Compiler::EnvironmentItem getItem(const std::string &name, std::optional<CompilerFrontend::Type::ResolvedType> type = std::nullopt) override final;

    virtual Assembler::CodeGenerator &getCodeGenerator() override final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual std::vector<CompilerFrontend::Type::ResolvedType> getTypes(const CompilerFrontend::Token &name,
                                                                       CompilerFrontend::ErrorHandlerBase &errorHandler) final;

private:
    std::reference_wrapper<const Library> m_Library;
};
}
