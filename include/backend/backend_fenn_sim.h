#include "backend/backend_fenn.h"

//----------------------------------------------------------------------------
// BackendFeNNSim
//----------------------------------------------------------------------------
//! Base class for FeNN backends
//! **NOTE** this is a temporary, simplified version of GeNN's BackendBase
class BackendFeNNSim : public BackendFeNN
{
public:
    //------------------------------------------------------------------------
    // BackendFeNN virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                       StateBase *state) const override final;
    virtual std::unique_ptr<ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                       StateBase *state) const override final;

    virtual std::unique_ptr<StateBase> createState() const override final;
};