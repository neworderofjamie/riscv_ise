#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler includes
#include "compiler/event_container.h"
#include "compiler/parameter.h"
#include "compiler/process.h"
#include "compiler/variable.h"

//----------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------
class Model
{
public:
    Model() = default;
    Model(const Model&) = delete;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const EventContainer *addEventContainer(const Shape &shape)
    {
        m_Components.push_back(std::make_unique<EventContainer>(shape));
        return static_cast<const EventContainer*>(m_Components.back().get());
    }

    const Parameter *addParameter(const GeNN::Type::NumericValue &value)
    {
        m_Components.push_back(std::make_unique<Parameter>(value));
        return static_cast<const Parameter*>(m_Components.back().get());
    }

    const SpikeInputProcess *addSpikeInputProcess(const EventContainer *outputEvents)
    {
        m_Components.push_back(std::make_unique<SpikeInputProcess>(outputEvents));
        return static_cast<const SpikeInputProcess*>(m_Components.back().get());
    }

    const NeuronUpdateProcess *addNeuronUpdateProcess(
        const std::string &code, const ParameterMap &parameters = {}, 
        const VariableMap &variables = {}, const EventContainerMap &outputEvents = {})
    {
        m_Components.push_back(
            std::make_unique<NeuronUpdateProcess>(code, parameters, variables, outputEvents));
        return static_cast<const NeuronUpdateProcess*>(m_Components.back().get());
    }

    const EventPropagationProcess *addEventPropagationProcess(
        const EventContainer *inputEvents, const Variable *weight, const Variable *target)
    {
        m_Components.push_back(
            std::make_unique<EventPropagationProcess>(inputEvents, weight, target));
        return static_cast<const EventPropagationProcess*>(m_Components.back().get());
    }

    const Variable *addVariable(const Shape &shape, const GeNN::Type::ResolvedType &type)
    {
        m_Components.push_back(std::make_unique<Variable>(shape, type));
        return static_cast<const Variable*>(m_Components.back().get());
    }


private:
    
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<std::unique_ptr<ModelComponent>> m_Components;
};