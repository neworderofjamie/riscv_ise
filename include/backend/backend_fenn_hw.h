#include "backend/backend_fenn.h"

//----------------------------------------------------------------------------
// BackendFeNNHW
//----------------------------------------------------------------------------
//! Backend for FeNN hardware
class BACKEND_EXPORT BackendFeNNHW : public BackendFeNN
{
public:
    BackendFeNNHW(bool useDRAMForWeights = false, bool keepParamsInRegisters = true, 
                  RoundingMode neuronUpdateRoundingMode = RoundingMode::NEAREST,
                  off_t coreBaseAddress = 0x80000000);

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

private:
    off_t m_CoreBaseAddress;
};