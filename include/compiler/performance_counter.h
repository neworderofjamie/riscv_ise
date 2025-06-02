#pragma once

// Compiler includes
#include "model_component.h"

//----------------------------------------------------------------------------
// PerformanceCounter
//----------------------------------------------------------------------------
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
