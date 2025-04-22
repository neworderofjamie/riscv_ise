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
    //------------------------------------------------------------------------
    // BackendFeNN virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                       StateBase *state) const override final;
    virtual std::unique_ptr<ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                       StateBase *state) const override final;
    virtual std::unique_ptr<IFieldArray> createFieldArray(const Model &model, StateBase *state) const override final;

    virtual std::unique_ptr<StateBase> createState() const override final;
};