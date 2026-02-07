#pragma once

// Compiler includes
#include "model/model_component.h"

//----------------------------------------------------------------------------
// Model::PerformanceCounter
//----------------------------------------------------------------------------
namespace Model
{
class PerformanceCounter : public AcceptableModelComponent<PerformanceCounter, State>
{
public:
    PerformanceCounter(Private, const std::string &name)
    :   AcceptableModelComponent<PerformanceCounter, State>(name)
    {}

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<PerformanceCounter> create(const std::string &name = "")
    {
        return std::make_shared<PerformanceCounter>(Private(), name);
    }
};
}