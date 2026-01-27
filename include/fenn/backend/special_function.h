#pragma once

// Backend includes
#include "fenn/backend/environment.h"

// Forward declarations
class CodeGenerator;
class ScalarRegsiterAllocator;
class VectorRegisterAllocator;

//----------------------------------------------------------------------------
// SpecialFunctions
//----------------------------------------------------------------------------
namespace SpecialFunctions
{
//! How many elements are required in the LUT for each special function
size_t getLUTCount();

//----------------------------------------------------------------------------
// Exp
//----------------------------------------------------------------------------
namespace Exp
{
    //! Add overrides of exp() to library, allocate registers for constants and add to environment
    void add(CodeGenerator &codeGenerator, ScalarRegisterAllocator &scalarRegisterAllocator, 
             VectorRegisterAllocator &vectorRegisterAllocator, EnvironmentExternal &env,
             EnvironmentLibrary::Library &library, uint32_t lutField);
}
}