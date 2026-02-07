#include "fenn/backend/process.h"

// Model includes
#include "model/variable.h"

//----------------------------------------------------------------------------
// FeNN::Backend::NeuronUpdateProcess
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
void NeuronUpdateProcess::updateMemSpaceCompatibility(Model::VariablePtr variable, MemSpaceCompatibility &memSpaceCompatibility) const
{
    const auto var = std::find_if(getVariables().cbegin(), getVariables().cend(),
                                  [&variable](const auto &v){ return v.second == variable; });
    assert(var != neuronUpdateProcess->getVariables().cend());

    // Neuron variables can be located in URAM, LLM or both
    memSpaceCompatibility.bram = false;
    memSpaceCompatibility.dram = false;

    if(!memSpaceCompatibility.uram && !memSpaceCompatibility.llm && !memSpaceCompatibility.uram) {
        throw std::runtime_error("Neuron update process '" + getName()
                                 + "' variable array '" + variable->getName()
                                 + "' shared with incompatible processes");
    }
}

//----------------------------------------------------------------------------
// FeNN::Backend::EventPropagationProcess
//----------------------------------------------------------------------------
void EventPropagationProcess::updateMemSpaceCompatibility(Model::VariablePtr variable, MemSpaceCompatibility &memSpaceCompatibility) const
{
    // If variable is weight, it can only be located in URAM
    if(variable == getWeight()) {
        memSpaceCompatibility.llm = false;
        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.uramLLM = false;

        if(m_UseDRAMForWeights) {
            memSpaceCompatibility.uram = false;

            if(!memSpaceCompatibility.dram) {
                throw std::runtime_error("Event propagation process '" + getName() 
                                         + "' weight array '" + getWeight()->getName()
                                         + "' shared with incompatible processes");
            }
        }
        else {
            memSpaceCompatibility.dram = false;

            if(!memSpaceCompatibility.uram) {
                throw std::runtime_error("Event propagation process '" + getName() 
                                         + "' weight array '" + getWeight()->getName()
                                         + "' shared with incompatible processes");
            }
        }
    }
    // Otherwise, if variable's target
    else if(variable == getTarget()) {
        // It can't be located in BRAM or DRAM
        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.dram = false;

        // If it's sparse, it must be located in LLM
        if(getNumSparseConnectivityBits() > 0) {
            memSpaceCompatibility.uram = false;
            memSpaceCompatibility.uramLLM = false;

            if(!memSpaceCompatibility.llm) {
                throw std::runtime_error("Event propagation process '" + getName()
                                         + "' target array '" + getTarget()->getName()
                                         + "' shared with incompatible processes");
            }
        }
        // Otherwise, if it's delayed, it must be located in URAMLLM
        else if(getNumDelayBits() > 0) {
            memSpaceCompatibility.uram = false;
            memSpaceCompatibility.llm = false;

            if(!memSpaceCompatibility.uramLLM) {
                throw std::runtime_error("Event propagation process '" + getName()
                                         + "' target array '" + getTarget()->getName()
                                         + "' shared with incompatible processes");
            }
        }
    }
    else {
        assert(false);
    }
}
//----------------------------------------------------------------------------
void EventPropagationProcess::updateMaxRowLength(size_t &maxRowLength) const
{
    maxRowLength = std::max(maxRowLength, getMaxRowLength());
}

//----------------------------------------------------------------------------
// FeNN::Backend::RNGInitProcess
//----------------------------------------------------------------------------
void RNGInitProcess::updateMemSpaceCompatibility(Model::VariablePtr variable, MemSpaceCompatibility &memSpaceCompatibility) const
{
    assert (m_Variable == rngInitProcess->getSeed());

    // Seeds can only be stored in URAM
    memSpaceCompatibility.llm = false;
    memSpaceCompatibility.bram = false;
    memSpaceCompatibility.dram = false;
    memSpaceCompatibility.uramLLM = false;

    if(!memSpaceCompatibility.uram) {
        throw std::runtime_error("RNG init process '" + getName() 
                                 + "' seed array '" + getSeed()->getName()
                                 + "' shared with incompatible processes");
    }
}

//----------------------------------------------------------------------------
// FeNN::Backend::MemsetProcess
//----------------------------------------------------------------------------
void MemsetProcess::updateMemSpaceCompatibility(Model::VariablePtr variable, MemSpaceCompatibility &memSpaceCompatibility) const
{
    const auto target = std::get<Model::VariablePtr>(getTarget());
    assert(m_Variable == target);

    // **TODO** memset could handle anything
    memSpaceCompatibility.bram = false;
    memSpaceCompatibility.dram = false;

    if(!memSpaceCompatibility.llm && !memSpaceCompatibility.uram && !memSpaceCompatibility.uramLLM) {
        throw std::runtime_error("Memset process '" + getName()
                                 + "' target array '" + target->getName()
                                 + "' shared with incompatible processes");
    }
}


//----------------------------------------------------------------------------
// FeNN::Backend::BroadcastProcess
//----------------------------------------------------------------------------
void BroadcastProcess::updateMemSpaceCompatibility(Model::VariablePtr variable, MemSpaceCompatibility &memSpaceCompatibility) const
{
    // If variable is source, it can only be located in BRAM
    if(variable == getSource()) {
        memSpaceCompatibility.llm = false;
        memSpaceCompatibility.uram = false;
        memSpaceCompatibility.dram = false;
        memSpaceCompatibility.uramLLM = false;

        if(!memSpaceCompatibility.bram) {
            throw std::runtime_error("Broadcast process '" + getName() 
                                     + "' source array '" + getSource()->getName()
                                     + "' shared with incompatible processes");
        }
    }
    else {
        // Otherwise, if variable's target, it can only located in LLM or URAMLLM
        // **TODO** URAM would also be sensible IN THEORY 
        const auto target = std::get<Model::VariablePtr>(getTarget());
        assert(m_Variable == target);

        memSpaceCompatibility.bram = false;
        memSpaceCompatibility.uram = false;
        memSpaceCompatibility.dram = false;

        if(!memSpaceCompatibility.llm && !memSpaceCompatibility.uramLLM) {
            throw std::runtime_error("Broadcast process '" + getName()
                                     + "' target array '" + target->getName()
                                     + "' shared with incompatible processes");
        }
    }
}
}