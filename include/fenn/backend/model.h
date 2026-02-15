#pragma once

// Model includes
#include "model/model.h"

// Forward declarations
namespace Model
{
class GraphVector;
class State;
}

namespace boost::uuids::detail
{
class sha1;
}

//----------------------------------------------------------------------------
// FeNN::Backend::MemSpaceCompatibility
//----------------------------------------------------------------------------
//! Structure specifying which memory spaces a variable can be implemented in
namespace FeNN::Backend
{
struct MemSpaceCompatibility
{
    MemSpaceCompatibility()
    :   llm(true), uram(true), bram(true), dram(true), uramLLM(true)
    {}

    void updateHash(boost::uuids::detail::sha1 &hash) const;

    bool llm;
    bool uram;
    bool bram;
    bool dram;
    bool uramLLM;
};

//----------------------------------------------------------------------------
// FeNN::Backend::Model
//----------------------------------------------------------------------------
class Model : public ::Model::Model
{
public:
    using StateMemSpaceCompatibility = std::unordered_map<std::shared_ptr<const ::Model::State>, 
                                                          MemSpaceCompatibility>;

    Model(const Model::GraphVector &graphs, bool useDRAMForWeights = true);

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