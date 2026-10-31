#pragma once

// Frontend includes
#include "frontend/runtime.h"

// GeNN backend includes
#include "genn/backend/backend_export.h"
#include "genn/backend/fields.h"

// Forward declarations
namespace Frontend
{
class EventSource;
class ProcessGroup;
}
namespace GeNN::Backend
{
class Runtime;
}

//----------------------------------------------------------------------------
// GeNN::Backend::DeviceGeNN
//----------------------------------------------------------------------------
namespace GeNN::Backend
{
class GENN_BACKEND_EXPORT DeviceGeNN : public Frontend::DeviceBase
{
public:
    DeviceFeNN(size_t deviceIndex, int deviceID, Runtime &runtime);

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const CompilerFrontend::Type::ResolvedType &type,
                                                             const std::vector<size_t> &shape, const std::vector<size_t> &strides) = 0;

    //------------------------------------------------------------------------
    // DeviceBase virtuals
    //------------------------------------------------------------------------
    //virtual uint64_t readPerformanceCounter(std::shared_ptr<const Frontend::ProcessGroup> group,
    //                                        const std::string &counter) const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void allocateDataStructures();
    void populateDataStructures();

protected:
    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    const auto &getRuntime() const{ return m_Runtime.get(); }
    auto &getRuntime(){ return m_Runtime.get(); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::reference_wrapper<Runtime> m_Runtime;
    int m_DeviceID;
};

//----------------------------------------------------------------------------
// GeNN::Backend::Runtime
//----------------------------------------------------------------------------
class GENN_BACKEND_EXPORT Runtime : public Frontend::Runtime
{
public:
   Runtime(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels, 
           const std::vector<int> &deviceIDs);
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getKernelCode(std::shared_ptr<const Frontend::Kernel> kernel) const{ return m_KernelCode.at(kernel); }
    
protected:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Use runtime compiler to compiler kernel into PTX/other intermediate
    std::vector<char> compileKernel(const std::string &kernelCode) const = 0;
    
    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    //! Backend-specific logic to run at beginning of allocate function
    virtual void allocatePreamble() override;

    //! Backend-specific logic to run at end of allocate function
    virtual void allocatePostamble() override;
    
    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    const auto &getDeviceIDs() const{ return m_DeviceIDs; }

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void populateFields(size_t p, const std::pair<uint32_t, MergedFields> &mergedFields,
                        std::shared_ptr<const Frontend::ModelComponent> owner);

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Map of kernel pointers to maps of process groups to code
    std::unordered_map<std::shared_ptr<const Frontend::Kernel>, 
                       std::unordered_map<std::shared_ptr<const Frontend::ProcessGroup>,
                                          std::vector<char>>> m_KernelCode;
    
    std::vector<int> m_DeviceIDs;
};
}
