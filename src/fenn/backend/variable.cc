#include "fenn/backend/variable.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN backend includes
#include "fenn/backend/environment.h"
#include "fenn/backend/model.h"
#include "fenn/backend/runtime.h"

//----------------------------------------------------------------------------
// FeNN::Backend::Variable
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
std::unique_ptr<Frontend::ArrayBase> Variable::createArray(const std::vector<size_t> &shape, const std::vector<size_t> &strides, 
                                                           const Frontend::Model &model, Frontend::DeviceBase &device) const
{
    // Create array in correct memory space depending on compatibility
    switch(getMemSpace(model))
    {
    case MemSpace::DRAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in DRAM";
        return static_cast<DeviceFeNN&>(device).createDRAMArray(getType(), shape, strides);
    }
    case MemSpace::URAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in URAM";
        return static_cast<DeviceFeNN&>(device).createURAMArray(getType(), shape, strides);
    }
    case MemSpace::LLM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in LLM";
        return static_cast<DeviceFeNN&>(device).createLLMArray(getType(), shape, strides);
    }
    case MemSpace::BRAM:
    {
        LOGI_FENN_BACKEND << "Creating variable '" << getName() << "' array in BRAM";
        return static_cast<DeviceFeNN&>(device).createBRAMArray(getType(), shape, strides);
    }
    default:
        assert(false);
    }
}
//----------------------------------------------------------------------------
std::vector<Compiler::RegisterPtr> Variable::genPreamble(Assembler::CodeGenerator &c,
                                                         Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                                                         Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                                         uint32_t varFieldOffset, std::optional<uint32_t> numTimesteps,
                                                         Assembler::ScalarRegisterPtr fieldBaseReg,
                                                         Assembler::ScalarRegisterPtr timeReg,
                                                         Assembler::ScalarRegisterPtr numVariableBytes,
                                                         bool hasTime, const Frontend::Model &model) const
{
    // Create array in correct memory space depending on compatibility
    switch(getMemSpace(model))
    {
    case MemSpace::URAM:
    {
        // Allocate scalar register to hold address of variable
        auto readBufferReg = scalarRegisterAllocator.getRegister((getName() + "Buffer X").c_str());

        // Generate code to load address
        c.lw(*readBufferReg, *fieldBaseReg, varFieldOffset);

        // **TODO** currently this just handles providing entire simulation kernel worth of variable data or
        // recording variables for entire simulation - extend to support axonal delays and ring-buffer recording
        if (hasTime) {
            // Check there is a buffer entry for each timestep with one extra
            // **NOTE** variables get read from timestep and written to timestep + 1 hence extra buf
            if(getShape().getFirst() < (numTimesteps.value() + 1)) {
                throw std::runtime_error("Variables need to be buffered for " + std::to_string(numTimesteps.value() + 1u) + " timesteps");
            }

            ALLOCATE_SCALAR(STmp);
            c.mul(*STmp, *timeReg, *numVariableBytes);
            c.add(*readBufferReg, *readBufferReg, *STmp);

            // If variable is const, return read register
            if (getType().isConst) {
                return {readBufferReg};
            }
            // Otherwise, allocate second write register
            else {
                auto writeBufferReg = scalarRegisterAllocator.getRegister((getName() + "BufferWrite X").c_str());
                c.add(*writeBufferReg, *readBufferReg, *numVariableBytes);
                return {readBufferReg, writeBufferReg};
            }
        }
        else {
            return {readBufferReg};
        }
    }
    case MemSpace::LLM:
    {
        assert(!hasTime);

        // Allocate vector register to hold address of variable
        auto bufferReg = vectorRegisterAllocator.getRegister((getName() + "Buffer V").c_str());

        // Generate code to load address
        ALLOCATE_SCALAR(STmp);
        c.lw(*STmp, *fieldBaseReg, varFieldOffset);
        c.vfill(*bufferReg, *STmp);

        return {bufferReg};
    }
    /*case MemSpace::URAM_LLM:
    {
        // Check number of buffer timesteps is P.O.T.
        if(!::Common::Utils::isPOT(var.getShape().getFirst())) {
            throw std::runtime_error("When used as delayed event propagation targets, variables "
                                     "need to have a power-of-two number of buffer timesteps");
        }

        // Allocate scalar register to hold address of variable in URAM
        m_URAMBufferReg = scalarRegisterAllocator.getRegister((varName + "Buffer X").c_str());

        // Load URAM address from first word of field
        c.lw(*m_URAMBufferReg, *fieldBaseReg, varFieldOffset);

        // Allocate vector registers to hold address of variable in LLM and delay stride
        m_LLMBufferReg = vectorRegisterAllocator.getRegister((varName + "Buffer V").c_str());

        {
            ALLOCATE_SCALAR(STmp);
            ALLOCATE_SCALAR(STmp2);

            // Calculate time modulo delay buffer size
            c.andi(*STmp, *timeReg, var.getShape().getFirst() - 1);

            // Double to get starting offset in bytes
            c.slli(*STmp, *STmp, 1);

            // Load LLM address from second word of field, add offset and broadcast
            c.lw(*STmp2, *fieldBaseReg, varFieldOffset + 4);
            c.add(*STmp2, *STmp2, *STmp);
            c.vfill(*m_LLMBufferReg, *STmp2);
        }
    }*/
    default:
        assert(false);
    }
}
//----------------------------------------------------------------------------
void Variable::genLoad(Compiler::EnvironmentBase &env, Assembler::VectorRegisterPtr reg, uint32_t r,
                       const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const
{
    // Create array in correct memory space depending on compatibility
    switch(getMemSpace(model))
    {
    case MemSpace::URAM:
    {
        assert(state.size() >= 1);
        env.getCodeGenerator().vloadv(*reg, *std::get<Assembler::ScalarRegisterPtr>(state[0]), 64 * r);
        break;
    }
    case MemSpace::LLM:
    {
        assert(state.size() == 1);
        env.getCodeGenerator().vloadl(*reg, *std::get<Assembler::VectorRegisterPtr>(state[0]), 2 * r);
        break;
    }
    /*case MemSpace::URAM_LLM:
    {
        assert(state.size() == 2);
        // Load from LLM buffer into temporary register
        auto denDelayFrontReg = m_VectorRegisterAllocator.get().getRegister("DenDelayFront V");
        env.getCodeGenerator().vloadl(*denDelayFrontReg, *m_LLMBufferReg, m_DelayStride * r);   

        // Load from URAM buffer into state register
        env.getCodeGenerator().vloadv(*reg, *m_URAMBufferReg, 64 * r);

        // Write zero back to LLM
        env.getCodeGenerator().vstorel(*env.getVectorRegister("_zero"), *m_LLMBufferReg, m_DelayStride * r);

        // Add new input from LLM to state register
        env.getCodeGenerator().vadd_s(*reg, *reg, *denDelayFrontReg);
        break;
    }*/
    default:
        assert(false);
    }
}
//----------------------------------------------------------------------------
void Variable::genStore(Compiler::EnvironmentBase &env, Assembler::VectorRegisterPtr reg, uint32_t r,
                        const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const
{
    if (!getType().isConst) {
        // Create array in correct memory space depending on compatibility
        switch(getMemSpace(model))
        {
        case MemSpace::URAM:
        {
            if (state.size() == 1) {
                env.getCodeGenerator().vstore(*reg, *std::get<Assembler::ScalarRegisterPtr>(state[0]), 64 * r);
            }
            else  if (state.size() == 2) {
                env.getCodeGenerator().vstore(*reg, *std::get<Assembler::ScalarRegisterPtr>(state[1]), 64 * r);
            }
            else {
                assert(false);
            }
            break;
        }
        case MemSpace::LLM:
        {
            assert(state.size() == 1);
            env.getCodeGenerator().vstorel(*reg, *std::get<Assembler::VectorRegisterPtr>(state[0]), 2 * r);  
            break;
        }
        /*case MemSpace::URAM_LLM:
        {
            assert(state.size() == 2);
            env.getCodeGenerator().vstore(*reg, *std::get<Assembler::ScalarRegisterPtr>(state[0]), 64 * r);
            break;
        }*/
        default:
            assert(false);
        }
    }
}
//----------------------------------------------------------------------------
void Variable::genIncrement(Assembler::CodeGenerator &c, uint32_t numUnrolls,
                            Assembler::VectorRegisterPtr numUnrollBytesReg,
                            const std::vector<Compiler::RegisterPtr> &state, const Frontend::Model &model) const
{
    // Create array in correct memory space depending on compatibility
    switch(getMemSpace(model))
    {
    case MemSpace::URAM:
    {
        c.addi(*std::get<Assembler::ScalarRegisterPtr>(state[0]),
               *std::get<Assembler::ScalarRegisterPtr>(state[0]),
               64 * numUnrolls);

        if (state.size() == 2) {
            c.addi(*std::get<Assembler::ScalarRegisterPtr>(state[1]),
                   *std::get<Assembler::ScalarRegisterPtr>(state[1]),
                   64 * numUnrolls);
        }
        else if(state.size() != 1) {
            assert(false);
        }
        break;
    }
    case MemSpace::LLM:
    {
        assert(state.size() == 1);
        c.vadd(*std::get<Assembler::VectorRegisterPtr>(state[0]), 
               *std::get<Assembler::VectorRegisterPtr>(state[0]), 
               *numUnrollBytesReg);
        break;
    }
    /*case MemSpace::URAM_LLM:
    {
        assert(state.size() == 2);

        // Calculate how many bytes we need to advance LLM addresses
        // **TODO** VADDI instruction would save an instruction in this type of situation
        auto numUnrollBytesReg = m_VectorRegisterAllocator.get().getRegister("NumUnrollBytes V");
        c.vlui(*numUnrollBytesReg, numUnrolls * m_DelayStride);

        // Increment URAM and LLM pointers
        c.addi(*m_URAMBufferReg, *m_URAMBufferReg, 64 * numUnrolls);
        c.vadd(*m_LLMBufferReg, *m_LLMBufferReg, *numUnrollBytesReg);
        break;
    }*/
    default:
        assert(false);
    }
}
//----------------------------------------------------------------------------
bool Variable::needsNumUnrollBytesReg(const Frontend::Model &model) const
{
    return (getMemSpace(model) == MemSpace::LLM);
}
//----------------------------------------------------------------------------
MemSpace Variable::getMemSpace(const Frontend::Model &model) const
{
    return static_cast<const Model&>(model).getStateMemSpace(std::static_pointer_cast<const Frontend::State>(shared_from_this()), 
                                                             false/*getRuntime().shouldUseDRAMForWeights()*/);
}
}
