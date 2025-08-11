#include "backend/backend_fenn.h"

//----------------------------------------------------------------------------
// BackendFeNNHW
//----------------------------------------------------------------------------
//! Backend for FeNN hardware
class BACKEND_EXPORT BackendFeNNHW : public BackendFeNN
{
public:
    using BackendFeNN::BackendFeNN;

    //------------------------------------------------------------------------
    // BackendFeNN virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<StateBase> createState() const override final;

protected:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Should kernels be generated with simulation or hardware signalling
    virtual bool shouldGenerateSimulationKernels() const override final { return false; }
};