#pragma once

// Compiler includes
#include "model/model_component.h"

//----------------------------------------------------------------------------
// Model::PerformanceCounter
//----------------------------------------------------------------------------
namespace Model
{
class PerformanceCounter : public AcceptableState<PerformanceCounter>
{
public:
    PerformanceCounter(Private, const std::string &name)
    :   AcceptableState<PerformanceCounter>(name)
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