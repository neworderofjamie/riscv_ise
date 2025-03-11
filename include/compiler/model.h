#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler includes
#include "compiler/model_component.h"

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
    template<typename C, typename... Args>
    const C *addComponent(Args&&... args)
    {
        m_Components.push_back(std::make_unique<C>(std::forward<Args>(args)...));
        return static_cast<const C*>(m_Components.back().get());
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<std::unique_ptr<ModelComponent>> m_Components;
};