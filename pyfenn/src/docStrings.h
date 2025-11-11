/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_AcceptableModelComponent = R"doc()doc";

static const char *__doc_AcceptableModelComponent_accept = R"doc()doc";

static const char *__doc_ArrayBase =
R"doc(Base class for arrays created by backend
**NOTE** this is a temporary, simplified version of GeNN's ArrayBase)doc";

static const char *__doc_ArrayBase_ArrayBase = R"doc()doc";

static const char *__doc_ArrayBase_getCount = R"doc()doc";

static const char *__doc_ArrayBase_getHostPointer = R"doc(Get array host pointer)doc";

static const char *__doc_ArrayBase_getHostPointer_2 = R"doc()doc";

static const char *__doc_ArrayBase_getSizeBytes = R"doc()doc";

static const char *__doc_ArrayBase_getType = R"doc()doc";

static const char *__doc_ArrayBase_m_Count = R"doc()doc";

static const char *__doc_ArrayBase_m_HostPointer = R"doc()doc";

static const char *__doc_ArrayBase_m_Type = R"doc()doc";

static const char *__doc_ArrayBase_memsetHostPointer = R"doc(Memset the host pointer)doc";

static const char *__doc_ArrayBase_pullFromDevice = R"doc(Copy entire array from device)doc";

static const char *__doc_ArrayBase_pushToDevice = R"doc(Copy entire array to device)doc";

static const char *__doc_ArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_ArrayBase_setHostPointer = R"doc()doc";

static const char *__doc_BRAMAllocator = R"doc()doc";

static const char *__doc_BRAMAllocator_BRAMAllocator = R"doc()doc";

static const char *__doc_BRAMArrayBase = R"doc(Base class for arrays located in FeNN's BRAM)doc";

static const char *__doc_BRAMArrayBase_getBRAMPointer = R"doc()doc";

static const char *__doc_BRAMArrayBase_m_BRAMPointer = R"doc()doc";

static const char *__doc_BRAMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_BRAMArrayBase_setBRAMPointer = R"doc()doc";

static const char *__doc_BRAMFieldArray = R"doc()doc";

static const char *__doc_BRAMFieldArray_pushFieldsToDevice = R"doc(Copy field data to device)doc";

static const char *__doc_BRAMFieldArray_setFieldArray = R"doc(Sets field at offset to point to array)doc";

static const char *__doc_BackendFeNN =
R"doc(Base class for FeNN backends
**NOTE** this is a temporary, simplified version of GeNN's BackendBase)doc";

static const char *__doc_BackendFeNN_2 = R"doc()doc";

static const char *__doc_BackendFeNN_3 = R"doc()doc";

static const char *__doc_BackendFeNNHW = R"doc(Backend for FeNN hardware)doc";

static const char *__doc_BackendFeNNHW_BackendFeNNHW = R"doc()doc";

static const char *__doc_BackendFeNNHW_createState = R"doc()doc";

static const char *__doc_BackendFeNNHW_m_Core = R"doc()doc";

static const char *__doc_BackendFeNNHW_m_DMABufferSize = R"doc()doc";

static const char *__doc_BackendFeNNHW_m_NumCores = R"doc()doc";

static const char *__doc_BackendFeNNHW_shouldGenerateSimulationKernels = R"doc(Should kernels be generated with simulation or hardware signalling)doc";

static const char *__doc_BackendFeNNSim = R"doc(Backend for FeNN Instruction Set Simulator)doc";

static const char *__doc_BackendFeNNSim_BackendFeNNSim = R"doc()doc";

static const char *__doc_BackendFeNNSim_createState = R"doc()doc";

static const char *__doc_BackendFeNNSim_m_DMABufferSize = R"doc()doc";

static const char *__doc_BackendFeNNSim_shouldGenerateSimulationKernels = R"doc(Should kernels be generated with simulation or hardware signalling)doc";

static const char *__doc_BackendFeNN_BackendFeNN = R"doc()doc";

static const char *__doc_BackendFeNN_createArray = R"doc()doc";

static const char *__doc_BackendFeNN_createArray_2 = R"doc()doc";

static const char *__doc_BackendFeNN_createArray_3 = R"doc()doc";

static const char *__doc_BackendFeNN_createArray_4 = R"doc()doc";

static const char *__doc_BackendFeNN_createState = R"doc()doc";

static const char *__doc_BackendFeNN_generateKernel = R"doc()doc";

static const char *__doc_BackendFeNN_generateSimulationKernel = R"doc()doc";

static const char *__doc_BackendFeNN_getRequiredStateObjects = R"doc(Get mapping of IDs to state objects for any backend-specific state this model is going to require)doc";

static const char *__doc_BackendFeNN_m_KeepParamsInRegisters = R"doc()doc";

static const char *__doc_BackendFeNN_m_NeuronUpdateRoundingMode = R"doc()doc";

static const char *__doc_BackendFeNN_m_UseDRAMForWeights = R"doc()doc";

static const char *__doc_BackendFeNN_shouldGenerateSimulationKernels = R"doc(Should kernels be generated with simulation or hardware signalling)doc";

static const char *__doc_BroadcastProcess = R"doc()doc";

static const char *__doc_BroadcastProcess_2 = R"doc(Process for 'broadcasting' one scalar array across multiple vector lanes)doc";

static const char *__doc_BroadcastProcess_BroadcastProcess = R"doc()doc";

static const char *__doc_BroadcastProcess_create = R"doc()doc";

static const char *__doc_BroadcastProcess_getSource = R"doc()doc";

static const char *__doc_BroadcastProcess_getTarget = R"doc()doc";

static const char *__doc_BroadcastProcess_m_Source = R"doc()doc";

static const char *__doc_BroadcastProcess_m_Target = R"doc()doc";

static const char *__doc_CodeGenerator = R"doc()doc";

static const char *__doc_CodeGenerator_2 = R"doc()doc";

static const char *__doc_CodeGenerator_3 = R"doc()doc";

static const char *__doc_DMABufferAllocator = R"doc()doc";

static const char *__doc_DMABufferAllocator_DMABufferAllocator = R"doc()doc";

static const char *__doc_DRAMArrayBase = R"doc(Base class for arrays located in DRAM, accessed using DMA controller)doc";

static const char *__doc_DRAMArrayBase_getDRAMPointer = R"doc()doc";

static const char *__doc_DRAMArrayBase_m_DRAMPointer = R"doc()doc";

static const char *__doc_DRAMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_DRAMArrayBase_setDRAMPointer = R"doc()doc";

static const char *__doc_EnvironmentBase = R"doc()doc";

static const char *__doc_EnvironmentBase_2 = R"doc()doc";

static const char *__doc_EnvironmentBase_define = R"doc(Define identifier as corresponding register)doc";

static const char *__doc_EnvironmentBase_getCodeGenerator = R"doc(Get stream to write code within this environment to)doc";

static const char *__doc_EnvironmentBase_getFunctionGenerator = R"doc()doc";

static const char *__doc_EnvironmentBase_getItem = R"doc(Get the register to use for the named identifier)doc";

static const char *__doc_EnvironmentBase_getRegister = R"doc()doc";

static const char *__doc_EnvironmentBase_getScalarRegister = R"doc()doc";

static const char *__doc_EnvironmentBase_getVectorRegister = R"doc()doc";

static const char *__doc_EnvironmentBase_operator_array = R"doc()doc";

static const char *__doc_EnvironmentExternal = R"doc()doc";

static const char *__doc_EnvironmentExternalBase = R"doc()doc";

static const char *__doc_EnvironmentExternalBase_EnvironmentExternalBase = R"doc()doc";

static const char *__doc_EnvironmentExternalBase_EnvironmentExternalBase_2 = R"doc()doc";

static const char *__doc_EnvironmentExternalBase_EnvironmentExternalBase_3 = R"doc()doc";

static const char *__doc_EnvironmentExternalBase_define = R"doc()doc";

static const char *__doc_EnvironmentExternalBase_define_2 = R"doc()doc";

static const char *__doc_EnvironmentExternalBase_getCodeGenerator = R"doc(Get stream to write code within this environment to)doc";

static const char *__doc_EnvironmentExternalBase_getContextItem = R"doc()doc";

static const char *__doc_EnvironmentExternalBase_getContextTypes = R"doc(Get vector of types from context if it provides this functionality)doc";

static const char *__doc_EnvironmentExternalBase_m_Context = R"doc()doc";

static const char *__doc_EnvironmentExternal_EnvironmentExternal = R"doc()doc";

static const char *__doc_EnvironmentExternal_EnvironmentExternal_2 = R"doc()doc";

static const char *__doc_EnvironmentExternal_EnvironmentExternal_3 = R"doc()doc";

static const char *__doc_EnvironmentExternal_EnvironmentExternal_4 = R"doc()doc";

static const char *__doc_EnvironmentExternal_EnvironmentExternal_5 = R"doc()doc";

static const char *__doc_EnvironmentExternal_add = R"doc(Map a type (for type-checking) and a value (for pretty-printing) to an identifier)doc";

static const char *__doc_EnvironmentExternal_getItem = R"doc()doc";

static const char *__doc_EnvironmentExternal_getTypes = R"doc()doc";

static const char *__doc_EnvironmentExternal_m_Environment = R"doc()doc";

static const char *__doc_EnvironmentInternal = R"doc()doc";

static const char *__doc_EnvironmentInternal_EnvironmentInternal = R"doc()doc";

static const char *__doc_EnvironmentInternal_define = R"doc()doc";

static const char *__doc_EnvironmentInternal_getCodeGenerator = R"doc()doc";

static const char *__doc_EnvironmentInternal_getItem = R"doc()doc";

static const char *__doc_EnvironmentInternal_m_Enclosing = R"doc()doc";

static const char *__doc_EnvironmentInternal_m_LocalVariables = R"doc()doc";

static const char *__doc_EnvironmentLibrary = R"doc()doc";

static const char *__doc_EnvironmentLibrary_EnvironmentLibrary = R"doc()doc";

static const char *__doc_EnvironmentLibrary_EnvironmentLibrary_2 = R"doc()doc";

static const char *__doc_EnvironmentLibrary_EnvironmentLibrary_3 = R"doc()doc";

static const char *__doc_EnvironmentLibrary_EnvironmentLibrary_4 = R"doc()doc";

static const char *__doc_EnvironmentLibrary_getItem = R"doc()doc";

static const char *__doc_EnvironmentLibrary_getTypes = R"doc()doc";

static const char *__doc_EnvironmentLibrary_m_Library = R"doc()doc";

static const char *__doc_EventContainer = R"doc()doc";

static const char *__doc_EventContainer_2 = R"doc()doc";

static const char *__doc_EventContainer_3 = R"doc()doc";

static const char *__doc_EventContainer_4 = R"doc()doc";

static const char *__doc_EventContainer_EventContainer = R"doc()doc";

static const char *__doc_EventContainer_create = R"doc()doc";

static const char *__doc_EventContainer_getNumBufferTimesteps = R"doc()doc";

static const char *__doc_EventContainer_getShape = R"doc()doc";

static const char *__doc_EventContainer_m_NumBufferTimesteps = R"doc()doc";

static const char *__doc_EventContainer_m_Shape = R"doc()doc";

static const char *__doc_EventPropagationProcess = R"doc()doc";

static const char *__doc_EventPropagationProcess_2 = R"doc()doc";

static const char *__doc_EventPropagationProcess_EventPropagationProcess = R"doc()doc";

static const char *__doc_EventPropagationProcess_create = R"doc()doc";

static const char *__doc_EventPropagationProcess_getInputEvents = R"doc()doc";

static const char *__doc_EventPropagationProcess_getMaxRowLength = R"doc()doc";

static const char *__doc_EventPropagationProcess_getNumDelayBits = R"doc()doc";

static const char *__doc_EventPropagationProcess_getNumSourceNeurons = R"doc()doc";

static const char *__doc_EventPropagationProcess_getNumSparseConnectivityBits = R"doc()doc";

static const char *__doc_EventPropagationProcess_getNumTargetNeurons = R"doc()doc";

static const char *__doc_EventPropagationProcess_getTarget = R"doc()doc";

static const char *__doc_EventPropagationProcess_getWeight = R"doc()doc";

static const char *__doc_EventPropagationProcess_m_InputEvents = R"doc()doc";

static const char *__doc_EventPropagationProcess_m_MaxRowLength = R"doc()doc";

static const char *__doc_EventPropagationProcess_m_NumDelayBits = R"doc()doc";

static const char *__doc_EventPropagationProcess_m_NumSourceNeurons = R"doc()doc";

static const char *__doc_EventPropagationProcess_m_NumSparseConnectivityBits = R"doc()doc";

static const char *__doc_EventPropagationProcess_m_NumTargetNeurons = R"doc()doc";

static const char *__doc_EventPropagationProcess_m_Target = R"doc()doc";

static const char *__doc_EventPropagationProcess_m_Weight = R"doc()doc";

static const char *__doc_IFieldArray =
R"doc(Interface for object, probably backed by some sort of array,
that anages in-device memory fields structure)doc";

static const char *__doc_IFieldArray_pushFieldsToDevice = R"doc(Copy field data to device)doc";

static const char *__doc_IFieldArray_setFieldArray = R"doc(Sets field at offset to point to array)doc";

static const char *__doc_LLMAllocator = R"doc()doc";

static const char *__doc_LLMAllocator_LLMAllocator = R"doc()doc";

static const char *__doc_LLMArrayBase = R"doc(Base class for arrays located in FeNN's lane-local memories)doc";

static const char *__doc_LLMArrayBase_LLMArrayBase = R"doc()doc";

static const char *__doc_LLMArrayBase_getLLMPointer = R"doc()doc";

static const char *__doc_LLMArrayBase_m_LLMPointer = R"doc()doc";

static const char *__doc_LLMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_LLMArrayBase_setLLMPointer = R"doc()doc";

static const char *__doc_MemoryAllocator = R"doc()doc";

static const char *__doc_MemoryAllocator_MemoryAllocator = R"doc()doc";

static const char *__doc_MemoryAllocator_MemoryAllocator_2 = R"doc()doc";

static const char *__doc_MemoryAllocator_allocate = R"doc()doc";

static const char *__doc_MemoryAllocator_getFreeBytes = R"doc()doc";

static const char *__doc_MemoryAllocator_m_AlignementBytes = R"doc()doc";

static const char *__doc_MemoryAllocator_m_Context = R"doc()doc";

static const char *__doc_MemoryAllocator_m_HighWaterBytes = R"doc()doc";

static const char *__doc_MemoryAllocator_m_SizeBytes = R"doc()doc";

static const char *__doc_MemsetProcess = R"doc()doc";

static const char *__doc_MemsetProcess_2 = R"doc(Process for memsetting variables)doc";

static const char *__doc_MemsetProcess_MemsetProcess = R"doc()doc";

static const char *__doc_MemsetProcess_create = R"doc()doc";

static const char *__doc_MemsetProcess_getTarget = R"doc()doc";

static const char *__doc_MemsetProcess_m_Target = R"doc()doc";

static const char *__doc_Model = R"doc(Backend-agnostic datastructure build from processes user wishes to deploy)doc";

static const char *__doc_Model_2 = R"doc()doc";

static const char *__doc_ModelComponent = R"doc(Base class for things that can be added to models)doc";

static const char *__doc_ModelComponentVisitor = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_2 = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_3 = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_4 = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_5 = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_6 = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_7 = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_8 = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_9 = R"doc()doc";

static const char *__doc_ModelComponentVisitor_visit_10 = R"doc()doc";

static const char *__doc_ModelComponent_ModelComponent = R"doc()doc";

static const char *__doc_ModelComponent_ModelComponent_2 = R"doc()doc";

static const char *__doc_ModelComponent_Private = R"doc()doc";

static const char *__doc_ModelComponent_Private_Private = R"doc()doc";

static const char *__doc_ModelComponent_accept = R"doc()doc";

static const char *__doc_ModelComponent_getName = R"doc()doc";

static const char *__doc_ModelComponent_m_Name = R"doc()doc";

static const char *__doc_Model_Model = R"doc()doc";

static const char *__doc_Model_getBackendFields = R"doc()doc";

static const char *__doc_Model_getNumFieldBytes = R"doc()doc";

static const char *__doc_Model_getProcessGroups = R"doc()doc";

static const char *__doc_Model_getStateProcesses = R"doc()doc";

static const char *__doc_Model_getStatefulFields = R"doc()doc";

static const char *__doc_Model_m_BackendFields = R"doc()doc";

static const char *__doc_Model_m_NumFieldBytes = R"doc()doc";

static const char *__doc_Model_m_ProcessGroups = R"doc()doc";

static const char *__doc_Model_m_StateProcesses = R"doc()doc";

static const char *__doc_Model_m_StatefulFields = R"doc()doc";

static const char *__doc_NeuronUpdateProcess = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_2 = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_NeuronUpdateProcess = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_create = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_getNumNeurons = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_getOutputEvents = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_getParameters = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_getTokens = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_getVariables = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_m_NumNeurons = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_m_OutputEvents = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_m_Parameters = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_m_Tokens = R"doc()doc";

static const char *__doc_NeuronUpdateProcess_m_Variables = R"doc()doc";

static const char *__doc_Parameter = R"doc()doc";

static const char *__doc_Parameter_2 = R"doc()doc";

static const char *__doc_Parameter_3 = R"doc()doc";

static const char *__doc_Parameter_Parameter = R"doc()doc";

static const char *__doc_Parameter_create = R"doc()doc";

static const char *__doc_Parameter_getType = R"doc()doc";

static const char *__doc_Parameter_getValue = R"doc()doc";

static const char *__doc_Parameter_m_Type = R"doc()doc";

static const char *__doc_Parameter_m_Value = R"doc()doc";

static const char *__doc_PerformanceCounter = R"doc()doc";

static const char *__doc_PerformanceCounter_2 = R"doc()doc";

static const char *__doc_PerformanceCounter_PerformanceCounter = R"doc()doc";

static const char *__doc_PerformanceCounter_create = R"doc()doc";

static const char *__doc_Process = R"doc()doc";

static const char *__doc_Process_2 = R"doc()doc";

static const char *__doc_Process_3 = R"doc()doc";

static const char *__doc_ProcessGroup = R"doc()doc";

static const char *__doc_ProcessGroup_2 = R"doc()doc";

static const char *__doc_ProcessGroup_3 = R"doc()doc";

static const char *__doc_ProcessGroup_4 = R"doc()doc";

static const char *__doc_ProcessGroup_ProcessGroup = R"doc()doc";

static const char *__doc_ProcessGroup_create = R"doc()doc";

static const char *__doc_ProcessGroup_getPerformanceCounter = R"doc()doc";

static const char *__doc_ProcessGroup_getProcesses = R"doc()doc";

static const char *__doc_ProcessGroup_m_PerformanceCounter = R"doc()doc";

static const char *__doc_ProcessGroup_m_Processes = R"doc()doc";

static const char *__doc_RNGInitProcess = R"doc()doc";

static const char *__doc_RNGInitProcess_2 = R"doc()doc";

static const char *__doc_RNGInitProcess_RNGInitProcess = R"doc()doc";

static const char *__doc_RNGInitProcess_create = R"doc()doc";

static const char *__doc_RNGInitProcess_getSeed = R"doc()doc";

static const char *__doc_RNGInitProcess_m_Seed = R"doc()doc";

static const char *__doc_RoundingMode = R"doc()doc";

static const char *__doc_RoundingMode_NEAREST = R"doc()doc";

static const char *__doc_RoundingMode_STOCHASTIC = R"doc()doc";

static const char *__doc_RoundingMode_TO_ZERO = R"doc()doc";

static const char *__doc_Runtime = R"doc()doc";

static const char *__doc_Runtime_Runtime = R"doc()doc";

static const char *__doc_Runtime_allocate = R"doc()doc";

static const char *__doc_Runtime_getArray = R"doc()doc";

static const char *__doc_Runtime_getSOCPower = R"doc()doc";

static const char *__doc_Runtime_getState = R"doc()doc";

static const char *__doc_Runtime_m_Arrays = R"doc()doc";

static const char *__doc_Runtime_m_Backend = R"doc()doc";

static const char *__doc_Runtime_m_FieldArray = R"doc()doc";

static const char *__doc_Runtime_m_Model = R"doc()doc";

static const char *__doc_Runtime_m_State = R"doc()doc";

static const char *__doc_Runtime_run = R"doc()doc";

static const char *__doc_Runtime_setInstructions = R"doc()doc";

static const char *__doc_Runtime_startRun = R"doc()doc";

static const char *__doc_Runtime_waitRun = R"doc()doc";

static const char *__doc_ScalarRegsiterAllocator = R"doc()doc";

static const char *__doc_Shape = R"doc()doc";

static const char *__doc_Shape_Shape = R"doc()doc";

static const char *__doc_Shape_Shape_2 = R"doc()doc";

static const char *__doc_Shape_getBatchSize = R"doc()doc";

static const char *__doc_Shape_getDims = R"doc()doc";

static const char *__doc_Shape_getFlattenedSize = R"doc()doc";

static const char *__doc_Shape_getNumNeurons = R"doc()doc";

static const char *__doc_Shape_getNumSourceNeurons = R"doc()doc";

static const char *__doc_Shape_getNumTargetNeurons = R"doc()doc";

static const char *__doc_Shape_m_Dims = R"doc()doc";

static const char *__doc_Shape_operator_eq = R"doc()doc";

static const char *__doc_Shape_operator_ne = R"doc()doc";

static const char *__doc_Shape_toString = R"doc()doc";

static const char *__doc_SpecialFunctions_Exp_add = R"doc(Add overrides of exp() to library, allocate registers for constants and add to environment)doc";

static const char *__doc_SpecialFunctions_getLUTCount = R"doc(How many elements are required in the LUT for each special function)doc";

static const char *__doc_State = R"doc()doc";

static const char *__doc_State_2 = R"doc()doc";

static const char *__doc_State_3 = R"doc()doc";

static const char *__doc_StateBase = R"doc(Base class for runtime state objects created by backend)doc";

static const char *__doc_StateBase_2 = R"doc()doc";

static const char *__doc_StateBase_StateBase = R"doc()doc";

static const char *__doc_StateBase_StateBase_2 = R"doc()doc";

static const char *__doc_StateBase_createBRAMArray = R"doc()doc";

static const char *__doc_StateBase_createDRAMArray = R"doc()doc";

static const char *__doc_StateBase_createFieldArray = R"doc()doc";

static const char *__doc_StateBase_createLLMArray = R"doc()doc";

static const char *__doc_StateBase_createURAMArray = R"doc()doc";

static const char *__doc_StateBase_createURAMLLMArray = R"doc()doc";

static const char *__doc_StateBase_getBRAMAllocator = R"doc()doc";

static const char *__doc_StateBase_getBRAMAllocator_2 = R"doc()doc";

static const char *__doc_StateBase_getLLMAllocator = R"doc()doc";

static const char *__doc_StateBase_getLLMAllocator_2 = R"doc()doc";

static const char *__doc_StateBase_getSOCPower = R"doc()doc";

static const char *__doc_StateBase_getURAMAllocator = R"doc()doc";

static const char *__doc_StateBase_getURAMAllocator_2 = R"doc()doc";

static const char *__doc_StateBase_m_BRAMAllocator = R"doc(Allocators for BRAM, URAM and Lane-Local Memories)doc";

static const char *__doc_StateBase_m_LLMAllocator = R"doc()doc";

static const char *__doc_StateBase_m_URAMAllocator = R"doc()doc";

static const char *__doc_StateBase_setInstructions = R"doc()doc";

static const char *__doc_StateBase_startRun = R"doc()doc";

static const char *__doc_StateBase_waitRun = R"doc()doc";

static const char *__doc_Stateful = R"doc()doc";

static const char *__doc_Stateful_2 = R"doc()doc";

static const char *__doc_URAMAllocator = R"doc()doc";

static const char *__doc_URAMAllocator_URAMAllocator = R"doc()doc";

static const char *__doc_URAMArrayBase = R"doc(Base class for arrays located in FeNN's URAM)doc";

static const char *__doc_URAMArrayBase_URAMArrayBase = R"doc()doc";

static const char *__doc_URAMArrayBase_getURAMPointer = R"doc()doc";

static const char *__doc_URAMArrayBase_m_URAMPointer = R"doc()doc";

static const char *__doc_URAMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_URAMArrayBase_setURAMPointer = R"doc()doc";

static const char *__doc_URAMLLMArrayBase =
R"doc(Base class for arrays which are allocated in URAM but also have a delayed input in LLM
Typically used for implementing neuron variables with dendritically-delayed input)doc";

static const char *__doc_URAMLLMArrayBase_URAMLLMArrayBase = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_getLLMCount = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_getLLMPointer = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_getLLMSizeBytes = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_getURAMPointer = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_m_LLMCount = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_m_LLMPointer = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_m_URAMPointer = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_URAMLLMArrayBase_setLLMPointer = R"doc()doc";

static const char *__doc_URAMLLMArrayBase_setURAMPointer = R"doc()doc";

static const char *__doc_Variable = R"doc()doc";

static const char *__doc_Variable_2 = R"doc()doc";

static const char *__doc_Variable_3 = R"doc()doc";

static const char *__doc_Variable_4 = R"doc()doc";

static const char *__doc_Variable_Variable = R"doc()doc";

static const char *__doc_Variable_create = R"doc()doc";

static const char *__doc_Variable_getNumBufferTimesteps = R"doc()doc";

static const char *__doc_Variable_getShape = R"doc()doc";

static const char *__doc_Variable_getType = R"doc()doc";

static const char *__doc_Variable_m_NumBufferTimesteps = R"doc()doc";

static const char *__doc_Variable_m_Shape = R"doc()doc";

static const char *__doc_Variable_m_Type = R"doc()doc";

static const char *__doc_VectorRegisterAllocator = R"doc()doc";

static const char *__doc_compile = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

