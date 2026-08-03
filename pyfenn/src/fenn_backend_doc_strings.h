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


static const char *__doc_FeNN_Assembler_CodeGenerator = R"doc()doc";

static const char *__doc_FeNN_Assembler_CodeGenerator_2 = R"doc()doc";

static const char *__doc_FeNN_Assembler_CodeGenerator_3 = R"doc()doc";

static const char *__doc_FeNN_Assembler_CodeGenerator_4 = R"doc()doc";

static const char *__doc_FeNN_Assembler_CodeGenerator_5 = R"doc()doc";

static const char *__doc_FeNN_Assembler_CodeGenerator_6 = R"doc()doc";

static const char *__doc_FeNN_Assembler_ScalarRegisterAllocator = R"doc()doc";

static const char *__doc_FeNN_Assembler_ScalarRegisterAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Assembler_VectorRegisterAllocator = R"doc()doc";

static const char *__doc_FeNN_Assembler_VectorRegisterAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMAllocator_BRAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase = R"doc(Base class for arrays located in FeNN's BRAM)doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_getBRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_m_BRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_setBRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_BroadcastProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_getAllEventSinks = R"doc(Get vector of event source objects used by this process)doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_getAllEventSources = R"doc(Get vector of event source objects used by this process)doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_getAllState = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_getSource = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_getTarget = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_m_Source = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_m_Target = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_updateCompatibleSplitDimensions = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_updateMergeHash = R"doc()doc";

static const char *__doc_FeNN_Backend_DMABufferAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DMABufferAllocator_DMABufferAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DRAMArrayBase = R"doc(Base class for arrays located in DRAM, accessed using DMA controller)doc";

static const char *__doc_FeNN_Backend_DRAMArrayBase_getDRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_DRAMArrayBase_m_DRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_DRAMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_FeNN_Backend_DRAMArrayBase_setDRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_DenseEventPropagationProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_getAllState = R"doc(Get vector of state objects used by this process)doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_getWeight = R"doc()doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_m_Weight = R"doc()doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_updateCompatibleSplitDimensions =
R"doc(Update the compatible split dimensions of a state object (which should be
one used by this process) with any constraints imposed by this process))doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_updateMaxDMABufferSize = R"doc(Update the max-row length to support this process)doc";

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

static const char *__doc_FeNN_Backend_DeviceFeNN = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_DeviceFeNNHW = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createBRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createDRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createLLMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createURAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createURAMLLMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_getDMABuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_getDMABuffer_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_getDMABufferAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_getDMABufferAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_getDevice = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_getDevice_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_loadKernel = R"doc(Load kernel onto device)doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_m_DMABuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_m_DMABufferAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_m_Device = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_runCurrentKernel = R"doc(Run current kernel on device)doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_DeviceFeNNSim = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_createBRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_createDRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_createLLMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_createURAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_createURAMLLMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getDMABufferAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getDMABufferAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getDMAController = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getDMAController_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getRISCV = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getRISCV_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_loadKernel = R"doc(Load kernel onto device)doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_m_DMABufferAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_m_DMAController = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_m_RISCV = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_m_Router = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_runCurrentKernel = R"doc(Run current kernel on device)doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_DeviceFeNN = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createBRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createDRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createFieldArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createLLMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createPerformanceCounter = R"doc(Create suitable array for performance counter on this device)doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createURAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createURAMLLMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getBRAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getBRAMAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getFieldArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getFieldArray_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getLLMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getLLMAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getRuntime = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getRuntime_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getURAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_getURAMAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_BRAMAllocator = R"doc(Allocators for BRAM, URAM and Lane-Local Memories)doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_FieldArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_LLMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_Runtime = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_URAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_EnvironmentExternalBase = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_EnvironmentExternalBase_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_EnvironmentExternalBase_3 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_EnvironmentExternalBase_4 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_define = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_define_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_getContextCodeGenerator = R"doc(Get code generator exposed by context)doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_getContextItem = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_getContextTypes = R"doc(Get vector of types from context if it provides this functionality)doc";

static const char *__doc_FeNN_Backend_EnvironmentExternalBase_m_Context = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_EnvironmentExternal = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_EnvironmentExternal_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_EnvironmentExternal_3 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_EnvironmentExternal_4 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_EnvironmentExternal_5 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_EnvironmentExternal_6 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_add = R"doc(Map a type (for type-checking) and a value (for pretty-printing) to an identifier)doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_getCodeGenerator = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_getItem = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_getTypes = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentExternal_m_Environment = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_EnvironmentLibrary = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_EnvironmentLibrary_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_EnvironmentLibrary_3 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_EnvironmentLibrary_4 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_EnvironmentLibrary_5 = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_getCodeGenerator = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_getItem = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_getTypes = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLibrary_m_Library = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentLiteral = R"doc()doc";

static const char *__doc_FeNN_Backend_EnvironmentMergedField = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_EventChannel = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_create = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_genEmit = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_genIncrement = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_genPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_generateEventLoop = R"doc(Generate code to implement event loop)doc";

static const char *__doc_FeNN_Backend_EventDrivenProcessImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_EventDrivenProcessImplementation_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_EventDrivenProcessImplementation_generateCode = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_EventSinkBuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_create = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_genEmit = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_genIncrement = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_genPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_createBitArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genBitArrayEmit = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genBitArrayIncrement = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genBitArrayPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genEmit = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genIncrement = R"doc(Generate code to advance pointer after numUnrolls unrolled)doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_EventSourceBuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_create = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_generateArchetypeEventLoop = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_generateEventLoop = R"doc(Generate code to implement event loop)doc";

static const char *__doc_FeNN_Backend_EventSourceImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceImplementation_generateEventLoop = R"doc(Generate code to implement event loop)doc";

static const char *__doc_FeNN_Backend_KernelImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_3 = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_KernelImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_generateCode = R"doc(Generate code to implement process)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSinkIDBase = R"doc(Get the base ID of this event sink)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSinkIDs = R"doc(Get map of event sinks to their IDs)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSourceProcessGroup = R"doc(Get the process group in this kernel)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSourceProcesses = R"doc(Get map of event sources to the processes they provide input to)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getNumNeuronIDBits = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getNumPopulationIDBits = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSinkIDs = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSourceProcessGroup = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSourceProcesses = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_NumNeuronIDBits = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_NumPopulationIDBits = R"doc()doc";

static const char *__doc_FeNN_Backend_LLMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_LLMAllocator_LLMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_LLMArrayBase = R"doc(Base class for arrays located in FeNN's lane-local memories)doc";

static const char *__doc_FeNN_Backend_LLMArrayBase_LLMArrayBase = R"doc()doc";

static const char *__doc_FeNN_Backend_LLMArrayBase_getLLMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_LLMArrayBase_m_LLMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_LLMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_FeNN_Backend_LLMArrayBase_setLLMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_MemSpace = R"doc()doc";

static const char *__doc_FeNN_Backend_MemSpace_BRAM = R"doc()doc";

static const char *__doc_FeNN_Backend_MemSpace_DRAM = R"doc()doc";

static const char *__doc_FeNN_Backend_MemSpace_LLM = R"doc()doc";

static const char *__doc_FeNN_Backend_MemSpace_URAM = R"doc()doc";

static const char *__doc_FeNN_Backend_MemSpace_URAM_LLM = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator_MemoryAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator_MemoryAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator_allocate = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator_getFreeBytes = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator_m_AlignementBytes = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator_m_Context = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator_m_HighWaterBytes = R"doc()doc";

static const char *__doc_FeNN_Backend_MemoryAllocator_m_SizeBytes = R"doc()doc";

static const char *__doc_FeNN_Backend_MemsetProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_MemsetProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_MemsetProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_MemsetProcess_generateLLMMemset = R"doc()doc";

static const char *__doc_FeNN_Backend_MemsetProcess_generateURAMMemset = R"doc()doc";

static const char *__doc_FeNN_Backend_MemsetProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_MemsetProcess_updateMergeHash = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields_MergedFields = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields_addField = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields_addField_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields_getFields = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields_getSize = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields_m_Fields = R"doc()doc";

static const char *__doc_FeNN_Backend_MergedFields_m_NextFieldOffset = R"doc()doc";

static const char *__doc_FeNN_Backend_Model = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_Model = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_getStateMemSpace = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_m_StateCompatibleMemSpaces = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_updateMergeHash = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_ProcessImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_generateCode = R"doc(Generate code to implement process)doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_getMaxUnroll = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_m_MaxUnroll = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_setMaxUnroll = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_updateMaxDMABufferSize = R"doc(Update the max-row length to support this process)doc";

static const char *__doc_FeNN_Backend_RNGInitProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_RNGInitProcess_RNGInitProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_RNGInitProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_RNGInitProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_RNGInitProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_RNGInitProcess_updateMergeHash = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_3 = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_4 = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_RuntimeHW = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_createDevice = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_getDeviceControl = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_getDeviceControl_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_m_DMABufferSize = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_m_DeviceControl = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_m_ParentDMABuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_RuntimeSim = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_createDevice = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_m_SharedBus = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_Runtime = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_allocatePostamble = R"doc(Backend-specific logic to run at end of allocate function)doc";

static const char *__doc_FeNN_Backend_Runtime_allocatePreamble = R"doc(Backend-specific logic to run at beginning of allocate function)doc";

static const char *__doc_FeNN_Backend_Runtime_getDMABufferSize = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_getKernelCode = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_getMergedEventSources = R"doc(Get merged event sources)doc";

static const char *__doc_FeNN_Backend_Runtime_getNeuronRoundingMode = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_DMABufferSize = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_KeepParamsInRegisters = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_KernelCode = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_MergedEventSourceFields = R"doc(Map from process groups to start addresses and merged fields associated with event sources)doc";

static const char *__doc_FeNN_Backend_Runtime_m_MergedEventSources = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_MergedProcessFields = R"doc(Map from process groups to start addresses and merged fields associated with processes)doc";

static const char *__doc_FeNN_Backend_Runtime_m_NeuronUpdateRoundingMode = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_NumFieldBytes = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_UseDRAMForWeights = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_populateFields = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_shouldKeepParamsInRegisters = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_shouldUseDRAMForWeights = R"doc()doc";

static const char *__doc_FeNN_Backend_SimpleKernel = R"doc()doc";

static const char *__doc_FeNN_Backend_SimpleKernel_SimpleKernel = R"doc()doc";

static const char *__doc_FeNN_Backend_SimpleKernel_create = R"doc()doc";

static const char *__doc_FeNN_Backend_SimpleKernel_generateCode = R"doc(Generate code to implement process)doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel = R"doc()doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel_SimulationLoopKernel = R"doc()doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel_create = R"doc()doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel_generateCode = R"doc(Generate code to implement process)doc";

static const char *__doc_FeNN_Backend_SpecialFunctions_Exp_add = R"doc(Add overrides of exp() to library, allocate registers for constants and add to environment)doc";

static const char *__doc_FeNN_Backend_SpecialFunctions_getLUTCount = R"doc(How many elements are required in the LUT for each special function)doc";

static const char *__doc_FeNN_Backend_TimeDrivenProcessImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_TimeDrivenProcessImplementation_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_TimeDrivenProcessImplementation_generateCode = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMAllocator_URAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMArrayBase = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMArrayBase_URAMArrayBase = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMArrayBase_getURAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMArrayBase_m_URAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_FeNN_Backend_URAMArrayBase_setURAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase =
R"doc(Base class for arrays which are allocated in URAM but also have a delayed input in LLM
Typically used for implementing neuron variables with dendritically-delayed input)doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_URAMLLMArrayBase = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_getLLMCount = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_getLLMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_getLLMShape = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_getLLMSizeBytes = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_getURAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_m_LLMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_m_LLMShape = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_m_URAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_setLLMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_URAMLLMArrayBase_setURAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_create = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_genIncrement = R"doc(Generate code to advance pointer after numUnrolls unrolled)doc";

static const char *__doc_FeNN_Backend_Variable_genLoad = R"doc(Generate code to load vector register reg from memory before unrolled loop iteration r)doc";

static const char *__doc_FeNN_Backend_Variable_genPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_genStore = R"doc(Generate code to store vector register reg to memory after unrolled loop iteration r)doc";

static const char *__doc_FeNN_Backend_Variable_getMemSpace = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_needsNumUnrollBytesReg = R"doc()doc";

static const char *__doc_FeNN_Backend_operator_band = R"doc()doc";

static const char *__doc_FeNN_Backend_operator_bor = R"doc()doc";

static const char *__doc_FeNN_Backend_operator_iand = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_2 = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_define = R"doc(Define identifier as corresponding register)doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_getCodeGenerator = R"doc(Get stream to write code within this environment to)doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_getFunctionGenerator = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_getItem = R"doc(Get the register to use for the named identifier)doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_getLiteral = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_getRegister = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_getScalarRegister = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_getVectorRegister = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentBase_operator_array = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentInternal = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentInternal_EnvironmentInternal = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentInternal_define = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentInternal_getCodeGenerator = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentInternal_getItem = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentInternal_m_Enclosing = R"doc()doc";

static const char *__doc_FeNN_Compiler_EnvironmentInternal_m_LocalVariables = R"doc()doc";

static const char *__doc_FeNN_Compiler_RoundingMode = R"doc()doc";

static const char *__doc_FeNN_Compiler_RoundingMode_NEAREST = R"doc()doc";

static const char *__doc_FeNN_Compiler_RoundingMode_STOCHASTIC = R"doc()doc";

static const char *__doc_FeNN_Compiler_RoundingMode_TO_ZERO = R"doc()doc";

static const char *__doc_FeNN_Compiler_compile = R"doc()doc";

static const char *__doc_FeNN_Disassembler_disassemble = R"doc()doc";

static const char *__doc_Frontend_ArrayBase = R"doc()doc";

static const char *__doc_Frontend_DeviceBase = R"doc()doc";

static const char *__doc_Frontend_EventSink =
R"doc(Event sinks take events from model components
which emit them e.g. NeuronUpdateProcess
**NOTE** inherits State virtually to avoid diamond problem when EventSink AND EventSource are inherited)doc";

static const char *__doc_Frontend_EventSource = R"doc()doc";

static const char *__doc_Frontend_EventSource_2 = R"doc()doc";

static const char *__doc_Frontend_ModelComponent = R"doc()doc";

static const char *__doc_Frontend_Process = R"doc()doc";

static const char *__doc_Frontend_Process_2 = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_2 = R"doc()doc";

static const char *__doc_Frontend_State = R"doc()doc";

static const char *__doc_Frontend_State_2 = R"doc()doc";

static const char *__doc_boost_uuids_detail_sha1 = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

