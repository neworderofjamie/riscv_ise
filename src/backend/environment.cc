#include "backend/environment.h"

// GeNN transpiler includes
#include "transpiler/errorHandler.h"

// Assembler includes
#include "assembler/assembler.h"

using namespace GeNN;

//----------------------------------------------------------------------------
// EnvironmentExternalBase
//----------------------------------------------------------------------------
void EnvironmentExternalBase::define(const std::string &, RegisterPtr)
{
    throw std::runtime_error("Cannot declare variable in external environment");
}
//----------------------------------------------------------------------------
CodeGenerator &EnvironmentExternalBase::getCodeGenerator()
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
//------------------------------------------------------------------------
void EnvironmentExternalBase::define(const Transpiler::Token &, const GeNN::Type::ResolvedType &,
                                     Transpiler::ErrorHandlerBase &)
{
    throw std::runtime_error("Cannot declare variable in external environment");
}
//------------------------------------------------------------------------
EnvironmentItem EnvironmentExternalBase::getContextItem(const std::string &name, 
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
// EnvironmentExternal
//----------------------------------------------------------------------------
EnvironmentItem EnvironmentExternal::getItem(const std::string &name, std::optional<Type::ResolvedType> type)
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
                              std::variant<RegisterPtr, FunctionGenerator> value)
{
    if(!m_Environment.try_emplace(name, type, value).second) {
        throw std::runtime_error("Redeclaration of '" + std::string{name} + "'");
    }
}


//----------------------------------------------------------------------------
// EnvironmentLibrary
//----------------------------------------------------------------------------
EnvironmentItem EnvironmentLibrary::getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type)
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