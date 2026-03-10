#include "fenn/backend/environment.h"

// GeNN includes
#include "gennUtils.h"

// GeNN transpiler includes
#include "transpiler/errorHandler.h"

// Assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/register_allocator.h"

using namespace GeNN;

//----------------------------------------------------------------------------
// FeNN::Backend::EnvironmentExternalBase
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
void EnvironmentExternalBase::define(const std::string &, Compiler::RegisterPtr)
{
    throw std::runtime_error("Cannot declare variable in external environment");
}
//------------------------------------------------------------------------
void EnvironmentExternalBase::define(const Transpiler::Token &, const GeNN::Type::ResolvedType &,
                                     Transpiler::ErrorHandlerBase &)
{
    throw std::runtime_error("Cannot declare variable in external environment");
}
//------------------------------------------------------------------------
Compiler::EnvironmentItem EnvironmentExternalBase::getContextItem(const std::string &name, 
                                                                  std::optional<Type::ResolvedType> type) const
{
    // If context includes a pretty-printing environment, get name from it
    if (std::get<1>(m_Context)) {
        return std::get<1>(m_Context)->getItem(name, type);
    }
    // Otherwise, give error
    else {
        throw std::runtime_error("Identifier '" + name + "' undefined");
    }
}
//----------------------------------------------------------------------------
std::vector<Type::ResolvedType> EnvironmentExternalBase::getContextTypes(const Transpiler::Token &name,
                                                                         Transpiler::ErrorHandlerBase &errorHandler)  const
{
    // If context includes a type-checking environment, get type from it
    if (std::get<0>(m_Context)) {
        return std::get<0>(m_Context)->getTypes(name, errorHandler);
    }
    // Otherwise, give error
    else {
        errorHandler.error(name, "Undefined identifier");
        throw Transpiler::TypeChecker::TypeCheckError();
    }
}
//----------------------------------------------------------------------------
Assembler::CodeGenerator &EnvironmentExternalBase::getContextCodeGenerator() const
{
    // If context includes a code stream
    if (std::get<2>(m_Context)) {
        return *std::get<2>(m_Context);
    }
    // Otherwise
    else {
        // Assert that there is a pretty printing environment
        assert(std::get<1>(m_Context));

        // Return its stream
        return std::get<1>(m_Context)->getCodeGenerator();
    }
}


//----------------------------------------------------------------------------
// FeNN::Backend::EnvironmentExternal
//----------------------------------------------------------------------------
Compiler::EnvironmentItem EnvironmentExternal::getItem(const std::string &name, 
                                                       std::optional<Type::ResolvedType> type)
{
    // If name isn't found in environment
    auto env = m_Environment.find(name);
    if (env == m_Environment.end()) {
        return getContextItem(name, type);
    }
    // Otherwise, get name from payload
    else {
        return std::get<1>(env->second);
    }
}
//----------------------------------------------------------------------------
Assembler::CodeGenerator &EnvironmentExternal::getCodeGenerator()
{
    return getContextCodeGenerator();
}
//----------------------------------------------------------------------------
std::vector<Type::ResolvedType> EnvironmentExternal::getTypes(const Transpiler::Token &name, 
                                                              Transpiler::ErrorHandlerBase &errorHandler)
{
    // If name isn't found in environment
    auto env = m_Environment.find(name.lexeme);
    if (env == m_Environment.end()) {
        return getContextTypes(name, errorHandler);
    }
    // Otherwise, return type of variables
    else {
        return {std::get<0>(env->second)};
    }
}
//----------------------------------------------------------------------------
void EnvironmentExternal::add(const GeNN::Type::ResolvedType &type, const std::string &name,
                              Compiler::EnvironmentItem value)
{
    if(!m_Environment.try_emplace(name, type, value).second) {
        throw std::runtime_error("Redeclaration of '" + std::string{name} + "'");
    }
}


//----------------------------------------------------------------------------
// FeNN::Backend::EnvironmentLibrary
//----------------------------------------------------------------------------
Compiler::EnvironmentItem EnvironmentLibrary::getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type)
{
    const auto [libTypeBegin, libTypeEnd] = m_Library.get().equal_range(name);
    if (libTypeBegin == libTypeEnd) {
        return getContextItem(name, type);
    }
    else {
        if (!type) {
            throw std::runtime_error("Ambiguous reference to '" + name + "' but no type provided to disambiguate");
        }
        const auto libType = std::find_if(libTypeBegin, libTypeEnd,
                                          [type](const auto &t) { return t.second.first == type; });
        assert(libType != libTypeEnd);
        return libType->second.second;
    }
}
//----------------------------------------------------------------------------
Assembler::CodeGenerator &EnvironmentLibrary::getCodeGenerator()
{
    return getContextCodeGenerator();
}
//------------------------------------------------------------------------
std::vector<Type::ResolvedType> EnvironmentLibrary::getTypes(const Transpiler::Token &name, Transpiler::ErrorHandlerBase &errorHandler)
{
    const auto [typeBegin, typeEnd] = m_Library.get().equal_range(name.lexeme);
    if (typeBegin == typeEnd) {
        return getContextTypes(name, errorHandler);
    }
    else {
        std::vector<Type::ResolvedType> types;
        types.reserve(std::distance(typeBegin, typeEnd));
        std::transform(typeBegin, typeEnd, std::back_inserter(types),
                        [](const auto &t) { return t.second.first; });
        return types;
    }
}

//----------------------------------------------------------------------------
// EnvironmentVectorLiteral
//----------------------------------------------------------------------------
EnvironmentLiteral::~EnvironmentLiteral()
{
    // Loop through literals
    for (const auto &l : m_Literals) {
        // If a register has been allocated, generate code to load value into register
        if (std::get<3>(l.second)) {
            getContextCodeGenerator().vlui(*std::get<3>(l.second), std::get<2>(l.second));
        }
    }

    // Write contents code
    getContextCodeGenerator() += m_Contents;
}
//------------------------------------------------------------------------
Compiler::EnvironmentItem EnvironmentLiteral::getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type)
{
    // If name isn't found in environment
    auto literal = m_Literals.find(name);
    if (literal == m_Literals.end()) {
        return getContextItem(name, type);
    }
    // Otherwise, if this is a literal for use in vector processor
    else if(std::get<2>(literal->second)) {
        // Allocate register
        const std::string registerContext = "V" + name;
        auto vectorReg = m_VectorRegisterAllocator.get().getRegister(registerContext.c_str());

        // Add reference to map and return register
        std::get<3>(literal->second) = vectorReg;
        return vectorReg;
    }
    // Otherwise, just return value
    else {
        return std::get<2>(literal->second);
    }
}
//------------------------------------------------------------------------
std::vector<GeNN::Type::ResolvedType> EnvironmentLiteral::getTypes(const GeNN::Transpiler::Token &name,
                                                                   GeNN::Transpiler::ErrorHandlerBase &errorHandler)
{
    // If name isn't found in environment
    auto literal = m_Literals.find(name.lexeme);
    if (literal == m_Literals.end()) {
        return getContextTypes(name, errorHandler);
    }
    // Otherwise, return type of variables
    else {
        return {std::get<0>(literal->second)};
    }
}
//------------------------------------------------------------------------
void EnvironmentLiteral::addVectorLiteral(const GeNN::Type::ResolvedType &type, const std::string &name, int16_t value)
{
    // Add literal name, type and value to map
    if (!m_Literals.emplace(std::piecewise_construct,
                            std::forward_as_tuple(name),
                            std::forward_as_tuple(type, value, true, nullptr)).second)
    {
        throw std::runtime_error("'" + name + "' already defined in literal environment");
    }
}
//------------------------------------------------------------------------
void EnvironmentLiteral::addScalarLiteral(const GeNN::Type::ResolvedType &type, const std::string &name, int value)
{
    // Add literal name, type and value to map
    if (!m_Literals.emplace(std::piecewise_construct,
                            std::forward_as_tuple(name),
                            std::forward_as_tuple(type, value, false, nullptr)).second)
    {
        throw std::runtime_error("'" + name + "' already defined in literal environment");
    }
}

//----------------------------------------------------------------------------
// EnvironmentMergedField
//----------------------------------------------------------------------------
EnvironmentMergedField::~EnvironmentMergedField()
{
    // Loop through fields
    for (const auto &f : m_Fields) {
        // If register has been allocated for this field i.e. it's in use
        if (std::get<2>(f.second)) {
            // Visit allocated register
            std::visit(
                Utils::Overload{
                    // If field has been allocated a SCALAR register, it is a pointer
                    [this, &f](FeNN::Assembler::ScalarRegisterAllocator::RegisterPtr s)
                    {
                        // Extract function to get pointer
                        auto getFieldPointerFn = std::get<MergedFields::GetFieldPointerFunc<const Frontend::Process>>(
                            std::get<1>(f.second));

                        // Add field **TODO** pointer size
                        const uint32_t offset = m_MergedFields.get().addField(getFieldPointerFn, 4);

                        // Load pointer into allocated register
                        getContextCodeGenerator().lw(*s, *m_FieldBaseReg, offset);
                    },
                    // If field has been allocated a VECTOR register, it is a constant
                    [this, &f](Assembler::VectorRegisterAllocator::RegisterPtr v)
                    {
                        // Extract function to get constant
                        auto getFieldConstantFn = std::get<MergedFields::GetFieldConstantFunc<const Frontend::Process>>(
                            std::get<1>(f.second));

                        //! Add field
                        const uint32_t offset = m_MergedFields.get().addField(getFieldConstantFn, 4);

                        // Load half from field into temporary register
                        auto STmp = m_ScalarRegisterAllocator.get().getRegister("STmp") ;
                        getContextCodeGenerator().lh(*STmp, *m_FieldBaseReg, offset);

                        // Fill allocated vector register
                        getContextCodeGenerator().vfill(*v, *STmp);
                    }},
                    std::get<2>(f.second).value());

        }
    }

    // Write contents code
    getContextCodeGenerator() += m_Contents;
}
//------------------------------------------------------------------------
Compiler::EnvironmentItem EnvironmentMergedField::getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type)
{
    // If name isn't found in environment
    auto field = m_Fields.find(name);
    if (field == m_Fields.end()) {
        return getContextItem(name, type);
    }
    // Otherwise
    else {
        // Allocate vector registers for constant value fields and scalar registers for pointer fields
        // **TODO** constants also useful in scalar registers!
        auto reg = std::visit(
            Utils::Overload{
                [this, &name](MergedFields::GetFieldConstantFunc<Frontend::Process>)
                {
                    const std::string registerContext = "V" + name;
                    return Compiler::RegisterPtr{m_VectorRegisterAllocator.get().getRegister(registerContext.c_str())};
                },
                [this, &name](MergedFields::GetFieldPointerFunc<Frontend::Process>)
                {
                    const std::string registerContext = "X" + name;
                    return Compiler::RegisterPtr{m_ScalarRegisterAllocator.get().getRegister(registerContext.c_str())};
                }},
                std::get<1>(field->second));

        // Add reference to map and return register
        std::get<2>(field->second) = reg;
        return reg;
    }
}
//------------------------------------------------------------------------
std::vector<GeNN::Type::ResolvedType> EnvironmentMergedField::getTypes(const GeNN::Transpiler::Token &name,
                                                                         GeNN::Transpiler::ErrorHandlerBase &errorHandler)
{
    // If name isn't found in environment
    auto field = m_Fields.find(name.lexeme);
    if (field == m_Fields.end()) {
        return getContextTypes(name, errorHandler);
    }
    // Otherwise, return typ
    else {
        return {std::get<0>(field->second)};
    }
}
}