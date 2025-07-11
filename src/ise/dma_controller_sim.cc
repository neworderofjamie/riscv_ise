#include "ise/dma_controller_sim.h"

// PLOG includes
#include <plog/Log.h>

// ISE includes
#include "ise/vector_processor.h"

//----------------------------------------------------------------------------
// DMAControllerSim
//----------------------------------------------------------------------------
DMAControllerSim::DMAControllerSim(VectorDataMemory &vectorDataMemory, size_t bufferSize)
:   m_VectorDataMemory(vectorDataMemory), m_Data(bufferSize), m_MM2SDelay(0),
    m_S2MMDelay(0), m_MM2SFSM(FSMState::IDLE), m_S2MMFSM(FSMState::IDLE), m_Registers{0}
{

}
//----------------------------------------------------------------------------
/*void DMAControllerSim::startWrite(uint32_t destination, uint32_t source, uint32_t size)
{
    LOGD << "Starting " << size << " byte DMA write from " << std::hex << source;
    if((destination & 63) != 0) {
        throw std::runtime_error("DMA writes to URAM must be 64 byte aligned");
    }

    if((source & 63) != 0) {
        throw std::runtime_error("DMA reads from mapped memory must be 64 byte aligned");
    }

    if((source + size) > m_Data.size()) {
        throw std::runtime_error("DMA cannot read from beyond buffer");
    }
    
    if(size >= ((1 << 19) - 1)) {
        LOGE << "Maximum size of hardware DMA exceeded";
    }

    // Write source and destination addresses to registers
    writeReg(Register::MM2S_SRC_ADDR, source);
    writeReg(Register::MM2S_DST_ADDR, destination);

    // Write count to registers
    writeReg(Register::MM2S_COUNT, size);

    // Run
    writeReg(Register::MM2S_CONTROL, 1);
}
//----------------------------------------------------------------------------
void DMAControllerSim::startRead(uint32_t destination, uint32_t source, uint32_t size)
{
    LOGD << "Starting " << size << " byte DMA read to " << std::hex << destination;
    if((source & 63) != 0) {
        throw std::runtime_error("DMA reads from URAM must be 64 byte aligned");
    }

    if((destination & 63) != 0) {
        throw std::runtime_error("DMA writes to mapped memory must be 64 byte aligned");
    }

    if((destination + size) > m_Data.size()) {
        throw std::runtime_error("DMA cannot write beyond buffer");
    }
    
    if(size >= ((1 << 19) - 1)) {
        LOGE << "Maximum size of hardware DMA exceeded";
    }

    // Write source and destination addresses to registers
    writeReg(Register::S2MM_SRC_ADDR, source);
    writeReg(Register::S2MM_DST_ADDR, destination);

    // Write count to registers
    writeReg(Register::S2MM_COUNT, size);

    // Run
    writeReg(Register::S2MM_CONTROL, 1);
}
//----------------------------------------------------------------------------
void DMAControllerSim::waitForWriteComplete() const
{

}
//----------------------------------------------------------------------------
void DMAControllerSim::waitForReadComplete() const
{

}*/
//----------------------------------------------------------------------------
void DMAControllerSim::writeReg(Register reg, uint32_t val)
{
    m_Registers[static_cast<int>(reg)] = val;
}
//----------------------------------------------------------------------------
uint32_t DMAControllerSim::readReg(Register reg) const
{
    return m_Registers[static_cast<int>(reg)];
}
//----------------------------------------------------------------------------
void DMAControllerSim::tick()
{
    // Tick MM2S FSM
    m_MM2SFSM.tick(
        // Enter
        [this](auto state)
        {
            // Get reference to status register
            uint32_t &statusRegister = m_Registers[static_cast<int>(Register::MM2S_STATUS)];

            // OR together all bits used to indicate state
            constexpr uint32_t stateMask = (static_cast<uint32_t>(StatusBits::STATE_IDLE)
                                            | static_cast<uint32_t>(StatusBits::STATE_START)
                                            | static_cast<uint32_t>(StatusBits::STATE_TRANSFER));
            if(state == FSMState::IDLE) {
                // Clear state bits and set idle
                statusRegister &= ~stateMask;
                statusRegister |= static_cast<uint32_t>(StatusBits::STATE_IDLE);
            }
            else if(state == FSMState::TRANSFERRING) {
                // Clear state bits and set transferring
                statusRegister &= ~stateMask;
                statusRegister |= static_cast<uint32_t>(StatusBits::STATE_TRANSFER);
    
                // Calculate delay
                assert(m_ActiveMM2S);
                m_MM2SDelay = 10 + (4 * (m_ActiveMM2S->count / 64));
            }
        },
        // Tick
        [this](auto state, auto transition)
        {
            // Get reference to status register
            uint32_t &statusRegister = m_Registers[static_cast<int>(Register::MM2S_STATUS)];

            // OR together all bits used to indicate transfer status
            constexpr uint32_t transferMask = (static_cast<uint32_t>(StatusBits::ERROR_INTERNAL)
                                               | static_cast<uint32_t>(StatusBits::ERROR_DECODE)
                                               | static_cast<uint32_t>(StatusBits::ERROR_SLAVE)
                                               | static_cast<uint32_t>(StatusBits::TRANSFER_OK));
            // Idling
            if(state == FSMState::IDLE) {
                // If we should start write
                if(readReg(Register::MM2S_CONTROL) & 1) {
                    // **TODO** verify register values

                    // Create active transfer
                    assert(!m_ActiveMM2S);
                    m_ActiveMM2S = Transfer{readReg(Register::MM2S_SRC_ADDR),
                                            readReg(Register::MM2S_DST_ADDR),
                                            readReg(Register::MM2S_COUNT)};
                    
                    // Zero control register
                    writeReg(Register::MM2S_CONTROL, 0);

                    // Transition to transferring
                    transition(FSMState::TRANSFERRING);
                }
            }
            // Transferring
            else if(state == FSMState::TRANSFERRING) {
                assert(m_ActiveMM2S);

                // Tick down delay
                m_MM2SDelay--;
                
                // If data is ready
                if(m_MM2SDelay <= 0) {
                    // Copy bytes from buffer into vector data memory
                    std::copy_n(m_Data.data() + m_ActiveMM2S->source,
                                m_ActiveMM2S->count,
                                m_VectorDataMemory.get().getData() + (m_ActiveMM2S->destination / 2));
                    
                    // Clear transfer bits and set transfer ok
                    statusRegister &= ~transferMask;
                    statusRegister |= static_cast<uint32_t>(StatusBits::TRANSFER_OK);
    

                    // Transition to idle
                    transition(FSMState::IDLE);
                }
            }
        });
    
    // Tick S2MM FSM
    m_S2MMFSM.tick(
        // Enter
        [this](auto state)
        {
            // Get reference to status register
            uint32_t &statusRegister = m_Registers[static_cast<int>(Register::S2MM_STATUS)];

            // OR together all bits used to indicate state
            constexpr uint32_t stateMask = (static_cast<uint32_t>(StatusBits::STATE_IDLE)
                                            | static_cast<uint32_t>(StatusBits::STATE_START)
                                            | static_cast<uint32_t>(StatusBits::STATE_TRANSFER));
            if(state == FSMState::IDLE) {
                // Clear state bits and set idle
                statusRegister &= ~stateMask;
                statusRegister |= static_cast<uint32_t>(StatusBits::STATE_IDLE);
            }
            else if(state == FSMState::TRANSFERRING) {
                // Clear state bits and set transferring
                statusRegister &= ~stateMask;
                statusRegister |= static_cast<uint32_t>(StatusBits::STATE_TRANSFER);
    
                // Calculate delay
                assert(m_ActiveS2MM);
                m_S2MMDelay = 10 + (4 * (m_ActiveS2MM->count / 64));
            }
        },
        // Tick
        [this](auto state, auto transition)
        {
            // Get reference to status register
            uint32_t &statusRegister = m_Registers[static_cast<int>(Register::S2MM_STATUS)];

            // OR together all bits used to indicate transfer status
            constexpr uint32_t transferMask = (static_cast<uint32_t>(StatusBits::ERROR_INTERNAL)
                                               | static_cast<uint32_t>(StatusBits::ERROR_DECODE)
                                               | static_cast<uint32_t>(StatusBits::ERROR_SLAVE)
                                               | static_cast<uint32_t>(StatusBits::TRANSFER_OK));
            // Idling
            if(state == FSMState::IDLE) {
                // If we should start write
                if(readReg(Register::S2MM_CONTROL) & 1) {
                    // **TODO** verify register values

                    // Create active transfer
                    assert(!m_ActiveS2MM);
                    m_ActiveS2MM = Transfer{readReg(Register::S2MM_SRC_ADDR),
                                            readReg(Register::S2MM_DST_ADDR),
                                            readReg(Register::S2MM_COUNT)};
                    
                    // Zero control register
                    writeReg(Register::S2MM_CONTROL, 0);

                    // Transition to transferring
                    transition(FSMState::TRANSFERRING);
                }
            }
            // Transferring
            else if(state == FSMState::TRANSFERRING) {
                assert(m_ActiveS2MM);

                // Tick down delay
                m_S2MMDelay--;
                
                // If data is ready
                if(m_S2MMDelay <= 0) {
                    // Copy bytes from vector data memory into buffer
                    std::copy_n(m_VectorDataMemory.get().getData() + (m_ActiveS2MM->destination / 2),
                                m_ActiveS2MM->count,
                                m_Data.data() + m_ActiveS2MM->source);
                    
                    // Clear transfer bits and set transfer ok
                    statusRegister &= ~transferMask;
                    statusRegister |= static_cast<uint32_t>(StatusBits::TRANSFER_OK);
    

                    // Transition to idle
                    transition(FSMState::IDLE);
                }
            }
        });
}