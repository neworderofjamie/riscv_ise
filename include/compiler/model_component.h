#pragma once

//----------------------------------------------------------------------------
// ModelComponent
//----------------------------------------------------------------------------
//! Base class for things that can be added to models
class ModelComponent
{
public:
    ModelComponent(const ModelComponent&) = delete;
    virtual ~ModelComponent() = default;

protected:
    ModelComponent() = default;
};