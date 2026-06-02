#pragma once

// Standard C++ includes
#include <memory>
#include <string>

// Forward declarations
namespace Frontend
{
class ArrayBase;
class DeviceBase;
class Model;
class Shape;
}

namespace boost::uuids::detail
{
class sha1;
}

// Macros
#define UPDATE_HASH_CLASS_NAME(CLASS_NAME) ::Common::Utils::updateHash(#CLASS_NAME, hash);

//----------------------------------------------------------------------------
// Frontend::ModelComponent
//----------------------------------------------------------------------------
namespace Frontend
{
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
// Frontend::State
//----------------------------------------------------------------------------
class State : public ModelComponent
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual const Shape &getShape() const = 0;
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const = 0;

    virtual std::unique_ptr<ArrayBase> createArray(const Shape &deviceShape, const Model &model, 
                                                   DeviceBase &device) const = 0;

protected:
    using ModelComponent::ModelComponent;
};
}
