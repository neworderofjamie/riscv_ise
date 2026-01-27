#include "fenn/backend/backend_fenn.h"

//----------------------------------------------------------------------------
// BackendFeNNHW
//----------------------------------------------------------------------------
//! Backend for FeNN hardware
class BACKEND_EXPORT BackendFeNNHW : public BackendFeNN
{
public:
    BackendFeNNHW(bool useDRAMForWeights = false, bool keepParamsInRegisters = true, 
                  RoundingMode neuronUpdateRoundingMode = RoundingMode::NEAREST,
                  unsigned int core = 0, unsigned int numCores = 1,
                  size_t dmaBufferSize = 512 * 1024);

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
    unsigned m_Core;
    unsigned int m_NumCores;
    size_t m_DMABufferSize;
};