#include "genn/backend/runtime.h"

// Standard C++ includes
#include <numeric>

// Standard C includes
#include <cstring>

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/model.h"
#include "frontend/process_group.h"

// Transpiler includes
#include "transpiler/code_stream.h"
// FeNN backend includes
//#include "fenn/backend/events.h"
//#include "fenn/backend/model.h"
//#include "fenn/backend/process.h"
//#include "fenn/backend/kernel.h"

using namespace CompilerFrontend;
using namespace GeNN;

//------------------------------------------------------------------------
// GeNN::Backend::DeviceGeNN
//------------------------------------------------------------------------
namespace GeNN::Backend
{
DeviceGeNN::DeviceGeNN(size_t deviceIndex, int deviceID, Runtime &runtime)
:   Frontend::DeviceBase(deviceIndex), m_Runtime(runtime), m_DeviceID(deviceID)
{
    
}
//----------------------------------------------------------------------------
void DeviceGeNN::allocateDataStructures()
{
    
}
//----------------------------------------------------------------------------
void DeviceGeNN::populateDataStructures()
{
    
}

//----------------------------------------------------------------------------
// GeNN::Backend::Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels, 
                 const std::vector<int> &deviceIDs)
:   Frontend::Runtime(std::make_unique<Frontend::Model>(kernels), deviceIDs.size()),
    m_DeviceIDs(deviceIDs)
{
    // Loop through kernels
    for (const auto &k : getModel()->getKernels()) {
        // Get kernel code and reserve outer vecto
        const auto processGroups = k->getAllProcessGroups();
        auto &kernelCode = m_KernelCode[k];
        kernelCode.reserve(processGroups.size()
        
        // Loop through all process groups in kernel
        // **TODO** parallelise
        for(const auto &p : processGroups) {
            // Create new stream to write process group to
            std::ostringstream stringStream;
            CodeStream codeStream(stringStream);

            const auto &mergedProcessGroup = getMergedProcessGroups().at(p);
            const auto &mergedProcesses = mergedProcessGroup.getMergedProcesses();
 
            // **TODO** merged data structures
            
            codeStream << "extern \"C\" __global__ kernel(unsigned int timestep)";
            {
                CodeStream::Scope b(codeStream);

                // Loop through merged processes
                size_t idStart = 0;
                for(const auto &m : mergedProcesses) {
                    // Ensure process has proper base class
                    auto pi = std::dynamic_pointer_cast<const ProcessImplementation>(m.getArchetype());
                    if (!pi) {
                        throw std::runtime_error("GeNN backend runtime used with incompatible process");
                    }


                    env.getStream() << "// merged" << m.getIndex() << std::endl;

                    // If this is the first  group
                    if(idStart == 0) {
                        codeStream << "if(id < " << paddedSize << ")";
                    }
                    else {
                        codeStream << "if(id >= " << idStart << " && id < " << idStart + paddedSize << ")";
                    }
                    {
                        CodeStream::Scope b(codeStream);

                        if(gMerge.getMerged().size() == 1) {
                            EnvironmentExternal groupEnv(env);
                            codeStream << "struct Merged" << T::name << "Group" << m.getIndex() << " *group";
                            codeStream << " = &d_merged" << T::name << "Group" << m.getIndex() << "[0]; " << std::endl;
                            //groupEnv.add(Type::Uint32.addConst(), "id", "lid",
                            //             {groupEnv.addInitialiser("const unsigned int lid = id - " + std::to_string(idStart) + ";")});

                            // Use the starting thread ID of the whole merged group as group_start_id
                            //groupEnv.add(Type::Uint32.addConst(), "_group_start_id", std::to_string(idStart));
                            
                            // Launch handler
                            //handler(groupEnv, gMerge);
                            //pi->generateCode(groupEnv, m);
                        }
                        else {
                            // Perform bisect operation to get index of merged struct
                            codeStream << "unsigned int lo = 0;" << std::endl;
                            codeStream << "unsigned int hi = " << m.getMerged().size() << ";" << std::endl;
                            codeStream << "while(lo < hi)" << std::endl;
                            {
                                CodeStream::Scope b(codeStream);
                                codeStream << "const unsigned int mid = (lo + hi) / 2;" << std::endl;

                                codeStream << "if(id < d_merged" << T::name << "GroupStartID" << m.getIndex() << "[mid])";
                                {
                                    CodeStream::Scope b(codeStream);
                                    codeStream << "hi = mid;" << std::endl;
                                }
                                env.getStream() << "else";
                                {
                                    CodeStream::Scope b(codeStream);
                                    codeStream << "lo = mid + 1;" << std::endl;
                                }
                            }

                            // Use this to get reference to merged group structure
                            codeStream << "struct Merged" << T::name << "Group" << m.getIndex() << " *group";
                            codeStream << " = &d_merged" << T::name << "Group" << m.getIndex() << "[lo - 1]; " << std::endl;

                            // Get group start thread ID and use as group_start_id
                            EnvironmentExternal groupEnv(env);
                            groupEnv.add(Type::Uint32.addConst(), "_group_start_id", "groupStartID",
                                         {groupEnv.addInitialiser("const unsigned int groupStartID = d_merged" + T::name + "GroupStartID" + std::to_string(gMerge.getIndex()) + "[lo - 1];")});

                            // Use this to calculate local id within group
                            groupEnv.add(Type::Uint32.addConst(), "id", "lid",
                                         {groupEnv.addInitialiser("const unsigned int lid = id - $(_group_start_id);")});
                            
                            // Launch handler
                            pi->generateCode(groupEnv, m);
                        }

                        idStart += paddedSize;
                    }
                }
            }
            
            // Compile kernel code and add to vector of process group kernels to link together
            kernelCode.push_back(std::move(compileKernel(stringStream.str())));
        }
    }
}
//----------------------------------------------------------------------------
void Runtime::allocatePreamble()
{
    // Loop through devices and allocate data structures
    for(auto &d : getDevices()) {
        static_cast<DeviceFeNN*>(d.get())->allocateDataStructures();
    }
}
//----------------------------------------------------------------------------
void Runtime::allocatePostamble()
{
    
}
//----------------------------------------------------------------------------
void Runtime::populateFields(size_t p, const std::pair<uint32_t, MergedFields> &mergedFields,
                             std::shared_ptr<const Frontend::ModelComponent> owner)
{

}
}
