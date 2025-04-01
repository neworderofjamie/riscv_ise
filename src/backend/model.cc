#include "backend/model.h"

// Plog includes
#include <plog/Log.h>

// Compiler includes
#include "compiler/event_container.h"
#include "compiler/process.h"
#include "compiler/process_group.h"
#include "compiler/variable.h"

namespace
{
//----------------------------------------------------------------------------
// Visitor
//----------------------------------------------------------------------------
class Visitor : public ModelComponentVisitor
{
public:
    Visitor(const std::vector<std::shared_ptr<const ProcessGroup>> processGroups, 
            Model::ProcessFields &processFields, Model::StateProcesses &stateProcesses)
    :   m_ProcessFields(processFields), m_StateProcesses(stateProcesses), m_FieldOffset(0)
    {
        // Loop through all process groups and visit
        for(const auto g : processGroups)  {
            g->accept(*this);
        }
    }

    uint32_t getNumFields() const { return m_FieldOffset / 4; }

private:
    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const EventContainer> eventContainer)
    {
        // Allocate BRAM for field pointer
        LOGD << "\tEvent container '" << eventContainer->getName() << "'";
        if(m_CurrentStateFields.try_emplace(eventContainer, m_FieldOffset).second) {
            m_FieldOffset += 4;
        }
        else {
            throw std::runtime_error("Event container '" + eventContainer->getName() + "' encountered multiple times in model traversal");
        }
    }

    virtual void visit(std::shared_ptr<const ProcessGroup> processGroup)
    {
        // Visit all the processes
        for(const auto p : processGroup->getProcesses()) {
            p->accept(*this);
        }
    }

    virtual void visit(std::shared_ptr<const NeuronUpdateProcess> neuronUpdateProcess)
    {
        LOGD << "Neuron update process '" << neuronUpdateProcess->getName() << "'";
        assert(m_CurrentStateFields.empty());        
    
        // Visit variables
        for(auto &v : neuronUpdateProcess->getVariables()) {
            v.second->accept(*this);
            m_StateProcesses.get()[v.second].push_back(neuronUpdateProcess);
        }

        // Visit output event containers
        for(auto &e : neuronUpdateProcess->getOutputEvents()) {
            e.second->accept(*this);
            m_StateProcesses.get()[e.second].push_back(neuronUpdateProcess);
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
        
        // Add back-references in state processes
        m_StateProcesses.get()[eventPropagationProcess->getInputEvents()].push_back(eventPropagationProcess);
        m_StateProcesses.get()[eventPropagationProcess->getWeight()].push_back(eventPropagationProcess);
        m_StateProcesses.get()[eventPropagationProcess->getTarget()].push_back(eventPropagationProcess);

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
        if(m_CurrentStateFields.try_emplace(variable, m_FieldOffset).second) {
            m_FieldOffset += 4;
        }
        else {
            throw std::runtime_error("Variable '" + variable->getName() + "' encountered multiple times in model traversal");
        }
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::reference_wrapper<Model::ProcessFields> m_ProcessFields;
    std::reference_wrapper<Model::StateProcesses> m_StateProcesses;
    Model::StateFields m_CurrentStateFields;
    uint32_t m_FieldOffset;
};
}

//----------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------
Model::Model(const std::vector<std::shared_ptr<const ProcessGroup>> processGroups)
:   m_ProcessGroups(processGroups)
{
    // Use visitor to populate process fields and 
    // state processes data structures from process groups
    Visitor visitor(processGroups, m_ProcessFields, m_StateProcesses);

    m_NumFields = visitor.getNumFields();
}
