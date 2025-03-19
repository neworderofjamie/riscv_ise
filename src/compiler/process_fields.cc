#include "compiler/process_fields.h"

// Plog includes
#include <plog/Log.h>

// Compiler includes
#include "compiler/event_container.h"
#include "compiler/memory_allocator.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"

namespace
{
//----------------------------------------------------------------------------
// StateVisitor
//----------------------------------------------------------------------------
class ProcessFieldVisitor : public ModelComponentVisitor
{
public:
    ProcessFieldVisitor(std::shared_ptr<const ProcessGroup> processGroup,
                        BRAMAllocator &bramAllocator, ProcessFields &processFields)
    :   m_BRAMAllocator(bramAllocator), m_ProcessFields(processFields)
    {
        for(auto p : processGroup->getProcesses()) {
            p->accept(*this);
        }
    }

    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const EventContainer> eventContainer)
    {
        // Allocate BRAM for field pointer
        LOGD << "\tEvent container '" << eventContainer->getName() << "'";
        if(!m_CurrentStateFields.try_emplace(eventContainer, m_BRAMAllocator.get().allocate(4)).second) {
            throw std::runtime_error("Event container '" + eventContainer->getName() + "' encountered multiple times in model traversal");
        }
    }

    virtual void visit(std::shared_ptr<const ProcessGroup>)
    {
        assert(false);
    }

    virtual void visit(std::shared_ptr<const NeuronUpdateProcess> neuronUpdateProcess)
    {
        LOGD << "Neuron update process '" << neuronUpdateProcess->getName() << "'";
        assert(m_CurrentStateFields.empty());        
    
        // Visit variables
        for(auto &v : neuronUpdateProcess->getVariables()) {
            v.second->accept(*this);
        }

        // Visit output event containers
        for(auto &e : neuronUpdateProcess->getOutputEvents()) {
            e.second->accept(*this);
        }

        // Add process fields
        if(!m_ProcessFields.get().try_emplace(neuronUpdateProcess, m_CurrentStateFields).second) {
            throw std::runtime_error("Neuron update process '" + neuronUpdateProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentStateFields.clear();
    }

    virtual void visit(std::shared_ptr<const EventPropagationProcess> eventPropagationProcess)
    {
        LOGD << "Event propagation process '" << eventPropagationProcess->getName() << "'";
        assert(m_CurrentStateFields.empty());

        // Visit components
        eventPropagationProcess->getInputEvents()->accept(*this);
        eventPropagationProcess->getWeight()->accept(*this);
        eventPropagationProcess->getTarget()->accept(*this);

        // Add process fields
        if(!m_ProcessFields.get().try_emplace(eventPropagationProcess, m_CurrentStateFields).second) {
            throw std::runtime_error("Event propagation process '" + eventPropagationProcess->getName() + "' encountered multiple times in model traversal");
        }

        // Clear current state fields
        m_CurrentStateFields.clear();
    }

    virtual void visit(std::shared_ptr<const Variable> variable)
    {
        // Allocate BRAM for field pointer
        LOGD << "\tVariable '" << variable->getName() << "'";
        if(!m_CurrentStateFields.try_emplace(variable, m_BRAMAllocator.get().allocate(4)).second) {
            throw std::runtime_error("Variable '" + variable->getName() + "' encountered multiple times in model traversal");
        }
    }


private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::reference_wrapper<BRAMAllocator> m_BRAMAllocator;
    std::reference_wrapper<ProcessFields> m_ProcessFields;

    StateFields m_CurrentStateFields;
};
}

void addFields(std::shared_ptr<const ProcessGroup> processGroup, BRAMAllocator &bramAllocator, ProcessFields &fields)
{
    ProcessFieldVisitor visitor(processGroup, bramAllocator, fields);
}