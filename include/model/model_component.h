#pragma once

// Standard C++ includes
#include <memory>
#include <string>

// Boost includes
#include <sha1.hpp>

// Forward declarations
namespace Model
{
class Model;
}

//----------------------------------------------------------------------------
// Model::ModelComponent
//----------------------------------------------------------------------------
namespace Model
{
//----------------------------------------------------------------------------
// Model::ModelComponent
//----------------------------------------------------------------------------
//! Base class for things that can be added to models
class ModelComponent : public std::enable_shared_from_this<ModelComponent>
{
public:
    ModelComponent(const ModelComponent&) = delete;
    virtual ~ModelComponent() = default;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const std::string &getName() const{ return m_Name; }

protected:
    struct Private{ explicit Private() = default; };

    ModelComponent(const std::string &name) : m_Name(name)
    {}

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::string m_Name;
};

//----------------------------------------------------------------------------
// Model::State
//----------------------------------------------------------------------------
class State : public ModelComponent
{
protected:
    using ModelComponent::ModelComponent;
};


//----------------------------------------------------------------------------
// Model::Stateful
//----------------------------------------------------------------------------
class Stateful : public ModelComponent
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const = 0;

protected:
    using ModelComponent::ModelComponent;
};


//----------------------------------------------------------------------------
// Model::Process
//----------------------------------------------------------------------------
class Process : public Stateful
{
protected:
    using Stateful::Stateful;

    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const = 0;
};
}