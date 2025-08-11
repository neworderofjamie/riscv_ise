#include "backend/backend_fenn.h"

// Backend includes
#include "backend/backend_export.h"

//----------------------------------------------------------------------------
// BackendFeNNSim
//----------------------------------------------------------------------------
//! Backend for FeNN Instruction Set Simulator
class BACKEND_EXPORT BackendFeNNSim : public BackendFeNN
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
    virtual bool shouldGenerateSimulationKernels() const override final { return true; }
};