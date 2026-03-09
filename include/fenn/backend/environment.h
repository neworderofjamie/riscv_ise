#pragma once

// Standard C++ includes
#include <unordered_map>

// GeNN includes
#include "transpiler/typeChecker.h"

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
class EnvironmentExternalBase : public Compiler::EnvironmentBase, public GeNN::Transpiler::TypeChecker::EnvironmentBase
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

    explicit EnvironmentExternalBase(Assembler::CodeGenerator &os)
    :   m_Context{nullptr, nullptr, &os}
    {
    }

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual void define(const std::string &, Compiler::RegisterPtr) override final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual void define(const GeNN::Transpiler::Token &, const GeNN::Type::ResolvedType &,
                        GeNN::Transpiler::ErrorHandlerBase &) override final;

protected:
    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    Compiler::EnvironmentItem getContextItem(const std::string &name, 
                                             std::optional<GeNN::Type::ResolvedType> type = std::nullopt) const;

    //! Get vector of types from context if it provides this functionality
    std::vector<GeNN::Type::ResolvedType> getContextTypes(const GeNN::Transpiler::Token &name, 
                                                          GeNN::Transpiler::ErrorHandlerBase &errorHandler)  const;

    //! Get code generator exposed by context
    Assembler::CodeGenerator &getContextCodeGenerator() const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::tuple<GeNN::Transpiler::TypeChecker::EnvironmentBase*, 
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

    explicit EnvironmentExternal(Assembler::CodeGenerator &os)
    :   EnvironmentExternalBase(os)
    {
    }

    EnvironmentExternal(const EnvironmentExternal&) = delete;

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual Compiler::EnvironmentItem getItem(const std::string &name, 
                                              std::optional<GeNN::Type::ResolvedType> type = std::nullopt) override final;

    virtual Assembler::CodeGenerator &getCodeGenerator() override final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual std::vector<GeNN::Type::ResolvedType> getTypes(const GeNN::Transpiler::Token &name, 
                                                           GeNN::Transpiler::ErrorHandlerBase &errorHandler) override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Map a type (for type-checking) and a value (for pretty-printing) to an identifier
    void add(const GeNN::Type::ResolvedType &type, const std::string &name, Compiler::EnvironmentItem value);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_map<std::string, std::tuple<GeNN::Type::ResolvedType, Compiler::EnvironmentItem>> m_Environment;
};


//----------------------------------------------------------------------------
// FeNN::Backend::EnvironmentLibrary
//----------------------------------------------------------------------------
class EnvironmentLibrary : public EnvironmentExternalBase
{
public:
    using Library = std::unordered_multimap<std::string, std::pair<GeNN::Type::ResolvedType, Compiler::FunctionGenerator>>;

    explicit EnvironmentLibrary(EnvironmentExternalBase &enclosing, const Library &library)
    :   EnvironmentExternalBase(enclosing), m_Library(library)
    {
    }

    explicit EnvironmentLibrary(Compiler::EnvironmentBase &enclosing, const Library &library)
    :   EnvironmentExternalBase(enclosing), m_Library(library)
    {
    }

    explicit EnvironmentLibrary(Assembler::CodeGenerator &os, const Library &library)
    :   EnvironmentExternalBase(os), m_Library(library)
    {
    }

    EnvironmentLibrary(const EnvironmentLibrary &) = delete;

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual Compiler::EnvironmentItem getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) override final;

    virtual Assembler::CodeGenerator &getCodeGenerator() override final;

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual std::vector<GeNN::Type::ResolvedType> getTypes(const GeNN::Transpiler::Token &name,
                                                           GeNN::Transpiler::ErrorHandlerBase &errorHandler) final;

private:
    std::reference_wrapper<const Library> m_Library;
};

//----------------------------------------------------------------------------
// EnvironmentVectorLiteral
//----------------------------------------------------------------------------
class EnvironmentVectorLiteral : public EnvironmentExternalBase
{
public:
    explicit EnvironmentVectorLiteral(EnvironmentExternalBase &enclosing,
                                      Assembler::VectorRegisterAllocator &vectorRegisterAllocator)
    :   EnvironmentExternalBase(enclosing), m_VectorRegisterAllocator(vectorRegisterAllocator)
    {
    }

    explicit EnvironmentVectorLiteral(EnvironmentExternal &enclosing,
                                      Assembler::VectorRegisterAllocator &vectorRegisterAllocator)
    : EnvironmentExternalBase(enclosing), m_VectorRegisterAllocator(vectorRegisterAllocator)
    {
    }

    explicit EnvironmentVectorLiteral(Compiler::EnvironmentBase &enclosing,
                                      Assembler::VectorRegisterAllocator &vectorRegisterAllocator)
    :   EnvironmentExternalBase(enclosing), m_VectorRegisterAllocator(vectorRegisterAllocator)
    {
    }

    explicit EnvironmentVectorLiteral(Assembler::CodeGenerator &os,
                                      Assembler::VectorRegisterAllocator &vectorRegisterAllocator)
    :   EnvironmentExternalBase(os), m_VectorRegisterAllocator(vectorRegisterAllocator)
    {
    }

    EnvironmentVectorLiteral(const EnvironmentVectorLiteral&) = delete;
    virtual ~EnvironmentVectorLiteral();

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual Compiler::EnvironmentItem getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) override final;

    virtual Assembler::CodeGenerator &getCodeGenerator() override final{ return m_Contents; }

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual std::vector<GeNN::Type::ResolvedType> getTypes(const GeNN::Transpiler::Token &name,
                                                           GeNN::Transpiler::ErrorHandlerBase &errorHandler) final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void addLiteral(const GeNN::Type::ResolvedType &type, const std::string &name, int16_t value);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Mapping between names and types, values and registers associated with literal values
    std::unordered_map<std::string, 
                       std::tuple<GeNN::Type::ResolvedType, int16_t,
                                  Assembler::VectorRegisterAllocator::RegisterPtr>> m_Literals;
    
    //! Vector register allocator
    std::reference_wrapper<Assembler::VectorRegisterAllocator> m_VectorRegisterAllocator;

    // Code generator used to generate body of environment
    Assembler::CodeGenerator m_Contents;
};

//----------------------------------------------------------------------------
// EnvironmentMergedField
//----------------------------------------------------------------------------
class EnvironmentMergedField : public EnvironmentExternalBase
{
    using NullableRegisterPtr = std::variant<std::monostate, 
                                             Assembler::ScalarRegisterAllocator::RegisterPtr, 
                                             Assembler::VectorRegisterAllocator::RegisterPtr>;
public:
    explicit EnvironmentMergedField(EnvironmentExternalBase &enclosing, 
                                    Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                    MergedFields &mergedFields)
    :   EnvironmentExternalBase(enclosing), m_VectorRegisterAllocator(vectorRegisterAllocator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator), m_MergedFields(mergedFields)
    {
    }

    explicit EnvironmentMergedField(EnvironmentExternal &enclosing,
                                    Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                    MergedFields &mergedFields)
    :   EnvironmentExternalBase(enclosing), m_VectorRegisterAllocator(vectorRegisterAllocator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator), m_MergedFields(mergedFields)
    {
    }

    explicit EnvironmentMergedField(Compiler::EnvironmentBase &enclosing,
                                    Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                    MergedFields &mergedFields)
    :   EnvironmentExternalBase(enclosing), m_VectorRegisterAllocator(vectorRegisterAllocator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator), m_MergedFields(mergedFields)
    {
    }

    explicit EnvironmentMergedField(Assembler::CodeGenerator &os,
                                    Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                    Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                    MergedFields &mergedFields)
    :   EnvironmentExternalBase(os), m_VectorRegisterAllocator(vectorRegisterAllocator),
        m_ScalarRegisterAllocator(scalarRegisterAllocator), m_MergedFields(mergedFields)
    {
    }

    EnvironmentMergedField(const EnvironmentMergedField&) = delete;
    virtual ~EnvironmentMergedField();

    //------------------------------------------------------------------------
    // Assembler::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual Compiler::EnvironmentItem getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) override final;

    virtual Assembler::CodeGenerator &getCodeGenerator() override final{ return m_Contents; }

    //------------------------------------------------------------------------
    // TypeChecker::EnvironmentBase virtuals
    //------------------------------------------------------------------------
    virtual std::vector<GeNN::Type::ResolvedType> getTypes(const GeNN::Transpiler::Token &name,
                                                           GeNN::Transpiler::ErrorHandlerBase &errorHandler) final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    template<typename P>
    void addField(const GeNN::Type::ResolvedType &type, const std::string &name, 
                  MergedFields::GetFieldValueFunc<P> getFieldValue);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    

    //! Mapping between names and types, getters and registers associated with fields
    std::unordered_map<std::string, 
                       std::tuple<GeNN::Type::ResolvedType, 
                                  MergedFields::GetFieldValueFunc<Frontend::Process>,
                                  NullableRegisterPtr>> m_Fields;

    //! Register allocators
    std::reference_wrapper<Assembler::ScalarRegisterAllocator> m_ScalarRegisterAllocator;
    std::reference_wrapper<Assembler::VectorRegisterAllocator> m_VectorRegisterAllocator;

    //! Merged field object to add any required fields to
    std::reference_wrapper<MergedFields> m_MergedFields;

    // Code generator used to generate body of environment
    Assembler::CodeGenerator m_Contents;
};
}