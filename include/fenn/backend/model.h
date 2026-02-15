#pragma once

// Model includes
#include "model/model.h"

// FeNN backend includes
#include "fenn/backend/process.h"

// Forward declarations
namespace Model
{
class GraphVector;
class State;
}

//----------------------------------------------------------------------------
// FeNN::Backend::ProcessImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class Model : public ::Model::Model
{
public:
    using StateMemSpaceCompatibility = std::unordered_map<std::shared_ptr<const ::Model::State>, 
                                                          MemSpaceCompatibility>;

    Model(const Model::GraphVector &graphs);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getStateMemSpaceCompatibility() const{ return m_StateMemSpaceCompatibility; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    StateMemSpaceCompatibility m_StateMemSpaceCompatibility;
};
}