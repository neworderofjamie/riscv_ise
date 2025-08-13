#pragma once

// Standard C++ includes
#include <unordered_map>

// GeNN includes
#include "transpiler/typeChecker.h"

// Compiler includes
#include "compiler/compiler.h"

// Forward declarations
class CodeGenerator;

//----------------------------------------------------------------------------
// EnvironmentExternalBase
//----------------------------------------------------------------------------
class EnvironmentExternalBase : public ::EnvironmentBase, public GeNN::Transpiler::TypeChecker::EnvironmentBase
{
public:
    explicit EnvironmentExternalBase(EnvironmentExternalBase &enclosing)
    :   m_Context{&enclosing, &enclosing, nullptr}
    {
    }

    explicit EnvironmentExternalBase(::EnvironmentBase &enclosing)
    :   m_Context{nullptr, &enclosing, nullptr}
    {
    }

    explicit EnvironmentExternalBase(CodeGenerator &os)
    :   m_Context{nullptr, nullptr, &os}
    {
    }

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual void define(const std::string &, RegisterPtr) override;

    //! Get stream to write code within this environment to
    virtual CodeGenerator &getCodeGenerator() final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual void define(const GeNN::Transpiler::Token &, const GeNN::Type::ResolvedType &,
                        GeNN::Transpiler::ErrorHandlerBase &) override;

protected:
    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    EnvironmentItem getContextItem(const std::string &name, 
                                   std::optional<GeNN::Type::ResolvedType> type = std::nullopt) const;

    //! Get vector of types from context if it provides this functionality
    std::vector<GeNN::Type::ResolvedType> getContextTypes(const GeNN::Transpiler::Token &name, 
                                                          GeNN::Transpiler::ErrorHandlerBase &errorHandler)  const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::tuple<GeNN::Transpiler::TypeChecker::EnvironmentBase*, ::EnvironmentBase*, CodeGenerator*> m_Context;
};

//----------------------------------------------------------------------------
// EnvironmentExternal
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

    explicit EnvironmentExternal(::EnvironmentBase &enclosing)
    :   EnvironmentExternalBase(enclosing)
    {
    }

    explicit EnvironmentExternal(CodeGenerator &os)
    :   EnvironmentExternalBase(os)
    {
    }

    EnvironmentExternal(const EnvironmentExternal&) = delete;

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual EnvironmentItem getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual std::vector<GeNN::Type::ResolvedType> getTypes(const GeNN::Transpiler::Token &name, 
                                                           GeNN::Transpiler::ErrorHandlerBase &errorHandler) final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Map a type (for type-checking) and a value (for pretty-printing) to an identifier
    void add(const GeNN::Type::ResolvedType &type, const std::string &name, std::variant<RegisterPtr, FunctionGenerator> value);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_map<std::string, std::tuple<GeNN::Type::ResolvedType, EnvironmentItem>> m_Environment;
};


//----------------------------------------------------------------------------
// EnvironmentLibrary
//----------------------------------------------------------------------------
class EnvironmentLibrary : public EnvironmentExternalBase
{
public:
    using Library = std::unordered_multimap<std::string, std::pair<GeNN::Type::ResolvedType, FunctionGenerator>>;

    explicit EnvironmentLibrary(EnvironmentExternalBase &enclosing, const Library &library)
    :   EnvironmentExternalBase(enclosing), m_Library(library)
    {
    }

    explicit EnvironmentLibrary(::EnvironmentBase &enclosing, const Library &library)
    :   EnvironmentExternalBase(enclosing), m_Library(library)
    {
    }

    explicit EnvironmentLibrary(CodeGenerator &os, const Library &library)
    :   EnvironmentExternalBase(os), m_Library(library)
    {
    }

    EnvironmentLibrary(const EnvironmentLibrary &) = delete;

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual EnvironmentItem getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual std::vector<GeNN::Type::ResolvedType> getTypes(const GeNN::Transpiler::Token &name,
                                                           GeNN::Transpiler::ErrorHandlerBase &errorHandler) final;

private:
    std::reference_wrapper<const Library> m_Library;
};