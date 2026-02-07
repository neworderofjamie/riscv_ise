#pragma once

// Backend includes
#include "fenn/backend/environment.h"

// Forward declarations
namespace FeNN::Assembler
{
class CodeGenerator;
}

namespace FeNN::Common
{
class ScalarRegisterAllocator;
class VectorRegisterAllocator;
}
//----------------------------------------------------------------------------
// SpecialFunctions
//----------------------------------------------------------------------------
namespace FeNN::Backend::SpecialFunctions
{
//! How many elements are required in the LUT for each special function
size_t getLUTCount();

//----------------------------------------------------------------------------
// Exp
//----------------------------------------------------------------------------
namespace Exp
{
    //! Add overrides of exp() to library, allocate registers for constants and add to environment
    void add(Assembler::CodeGenerator &codeGenerator, Common::ScalarRegisterAllocator &scalarRegisterAllocator, 
             Common::VectorRegisterAllocator &vectorRegisterAllocator, EnvironmentExternal &env,
             EnvironmentLibrary::Library &library, uint32_t lutField);
}
}