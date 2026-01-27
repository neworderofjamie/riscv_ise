#pragma once

// Standard C++ includes
#include <optional>
#include <unordered_map>
#include <variant>

// GeNN includes
#include "type.h"

// GeNN transpiler includes
#include "transpiler/statement.h"
#include "transpiler/typeChecker.h"

// Assembler includes
#include "fenn/assembler/register_allocator.h"

// Compiler includes
#include "fenn/compiler/fenn_compiler_export.h"

// Forward declarations
namespace FeNN::Assembler
{
class CodeGenerator;
}

namespace FeNN::Compiler
{
class EnvironmentBase;
using RegisterPtr = std::variant<FeNN::Assembler::ScalarRegisterAllocator::RegisterPtr, 
                                 FeNN::Assembler::VectorRegisterAllocator::RegisterPtr>;
using FunctionGenerator = std::function<std::pair<RegisterPtr, bool>(EnvironmentBase&, FeNN::Assembler::VectorRegisterAllocator&, 
                                                                     FeNN::Assembler::ScalarRegisterAllocator&, 
                                                                     FeNN::Assembler::ScalarRegisterAllocator::RegisterPtr, 
                                                                     const std::vector<RegisterPtr>&)>;

using EnvironmentItem = std::variant<RegisterPtr, FunctionGenerator>;

// What rounding mode to use for multiplication
enum class RoundingMode
{
    TO_ZERO,
    NEAREST,
    STOCHASTIC,
};

//----------------------------------------------------------------------------
// EnvironmentBase
//----------------------------------------------------------------------------
class EnvironmentBase
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Define identifier as corresponding register
    virtual void define(const std::string &name, RegisterPtr reg) = 0;
    
    //! Get the register to use for the named identifier
    virtual EnvironmentItem getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) = 0;

    //! Get stream to write code within this environment to
    virtual FeNN::Assembler::CodeGenerator &getCodeGenerator() = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    RegisterPtr getRegister(const std::string &name)
    {
        return std::get<RegisterPtr>(getItem(name));
    }

    FunctionGenerator getFunctionGenerator(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt)
    {
        return std::get<FunctionGenerator>(getItem(name, type));
    }

    FeNN::Assembler::ScalarRegisterAllocator::RegisterPtr getScalarRegister(const std::string &name)
    {
        return std::get<FeNN::Assembler::ScalarRegisterAllocator::RegisterPtr>(getRegister(name));
    }

    FeNN::Assembler::VectorRegisterAllocator::RegisterPtr getVectorRegister(const std::string &name)
    {
        return std::get<FeNN::Assembler::VectorRegisterAllocator::RegisterPtr>(getRegister(name));
    }

    //------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------
    RegisterPtr operator[] (const std::string &name)
    {
        return getRegister(name);
    }
};

//----------------------------------------------------------------------------
// EnvironmentInternal
//----------------------------------------------------------------------------
class FENN_COMPILER_EXPORT EnvironmentInternal : public EnvironmentBase
{
public:
    EnvironmentInternal(EnvironmentBase &enclosing)
    :   m_Enclosing(enclosing)
    {
    }

    //---------------------------------------------------------------------------
    // EnvironmentBase virtuals
    //---------------------------------------------------------------------------
    virtual void define(const std::string &name, RegisterPtr reg) final;

    virtual EnvironmentItem getItem(const std::string &name, std::optional<GeNN::Type::ResolvedType> type = std::nullopt) final;

    virtual FeNN::Assembler::CodeGenerator &getCodeGenerator() final;

private:
    //---------------------------------------------------------------------------
    // Members
    //---------------------------------------------------------------------------
    EnvironmentBase &m_Enclosing;
    std::unordered_map<std::string, RegisterPtr> m_LocalVariables;
};

//---------------------------------------------------------------------------
// Free functions
//---------------------------------------------------------------------------
FENN_COMPILER_EXPORT void compile(const GeNN::Transpiler::Statement::StatementList &statements, EnvironmentInternal &environment,
                                  const GeNN::Type::TypeContext &context, const GeNN::Transpiler::TypeChecker::ResolvedTypeMap &resolvedTypes,
                                  GeNN::Transpiler::ErrorHandlerBase &errorHandler, const std::unordered_map<int16_t, FeNN::Assembler::VectorRegisterAllocator::RegisterPtr> &literalPool,
                                  FeNN::Assembler::ScalarRegisterAllocator::RegisterPtr maskRegister, RoundingMode roundingMode,
                                  FeNN::Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, FeNN::Assembler::VectorRegisterAllocator &vectorRegisterAllocator);
}   // namespace FeNN::Compiler