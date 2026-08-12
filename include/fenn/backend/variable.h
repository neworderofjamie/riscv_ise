#pragma once

// Standard C++ includes
#include <memory>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/variable.h"

// FeNN assembler includes
#include "fenn/assembler/register_allocator.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/model.h"
#include "fenn/backend/backend_export.h"

// Forward declarations
namespace FeNN
{
namespace Assembler
{
class CodeGenerator;
}
}

//----------------------------------------------------------------------------
// FeNN::Backend::Variable
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class FENN_BACKEND_EXPORT Variable : public Frontend::Variable
{
public:
    using Frontend::Variable::Variable;

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                             size_t numDevices, const Frontend::Model &model, Frontend::DeviceBase &device) const override final;

    virtual ShapeStride getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                            size_t deviceIndex, size_t numDevices, const Frontend::Model &model) const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    std::vector<Compiler::RegisterPtr> genPreamble(Assembler::CodeGenerator &c, 
                                                   Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                                   Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                                   uint32_t varFieldOffset, std::optional<uint32_t> numTimesteps, 
                                                   Assembler::ScalarRegisterPtr fieldBaseReg, 
                                                   Assembler::ScalarRegisterPtr timeReg,
                                                   Assembler::ScalarRegisterPtr numVariableBytes,
                                                   bool hasTime, const Frontend::Model &model) const;

    //! Generate code to load vector register reg from memory before unrolled loop iteration r
    void genLoad(Compiler::EnvironmentBase &env, Assembler::VectorRegisterPtr reg, uint32_t r, 
                 const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const;

    //! Generate code to store vector register reg to memory after unrolled loop iteration r
    void genStore(Compiler::EnvironmentBase &env, Assembler::VectorRegisterPtr reg, uint32_t r, 
                  const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const;

    //! Generate code to advance pointer after numUnrolls unrolled 
    void genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls, 
                      Assembler::VectorRegisterPtr numUnrollBytesReg,
                      const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const;

    bool needsNumUnrollBytesReg(const Frontend::Model &model) const;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<Variable> create(const std::vector<size_t> &shape, const CompilerFrontend::Type::UnresolvedType &type, 
                                            const std::string &name = "")
    {
        return std::make_shared<Variable>(Private(), shape, type, name);
    }

private:
    MemSpace getMemSpace(const Frontend::Model &model) const;
};
}