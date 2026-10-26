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

static const char *__doc_FeNN_Assembler_CodeGenerator_7 = R"doc()doc";

static const char *__doc_FeNN_Assembler_ScalarRegisterAllocator = R"doc()doc";

static const char *__doc_FeNN_Assembler_ScalarRegisterAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Assembler_ScalarRegisterAllocator_3 = R"doc()doc";

static const char *__doc_FeNN_Assembler_VectorRegisterAllocator = R"doc()doc";

static const char *__doc_FeNN_Assembler_VectorRegisterAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Assembler_VectorRegisterAllocator_3 = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMAllocator_BRAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase = R"doc(Base class for arrays located in FeNN's BRAM)doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_getBRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_m_BRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_readDevice = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_readDevice_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_FeNN_Backend_BRAMArrayBase_setBRAMPointer = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_BroadcastProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_BroadcastProcess_generateArchetypeCode = R"doc()doc";

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

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_DelayEventPropagationProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_getAllState = R"doc(Get vector of state objects used by this process)doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_getNumDelayBits = R"doc()doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_getWeight = R"doc()doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_m_NumDelayBits = R"doc()doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_m_Weight = R"doc()doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_updateCompatibleSplitDimensions =
R"doc(Update the compatible split dimensions of a state object (which should be
one used by this process) with any constraints imposed by this process))doc";

static const char *__doc_FeNN_Backend_DelayEventPropagationProcess_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

static const char *__doc_FeNN_Backend_DendriticDelayUpdateProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_DendriticDelayUpdateProcess_DendriticDelayUpdateProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_DendriticDelayUpdateProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_DendriticDelayUpdateProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_DendriticDelayUpdateProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_DendriticDelayUpdateProcess_updateMergeHash = R"doc()doc";

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

static const char *__doc_FeNN_Backend_DenseEventPropagationProcess_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

static const char *__doc_FeNN_Backend_DeviceFeNN = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_DeviceFeNNHW = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createBRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createDRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createLLMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNHW_createURAMArray = R"doc()doc";

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

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getDMABufferAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getDMABufferAllocator_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getDMAController = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getDMAController_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getRISCV = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getRISCV_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_getSlaveBufferOverflow = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_loadKernel = R"doc(Load kernel onto device)doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_m_DMABufferAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_m_DMAController = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_m_RISCV = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_m_Router = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNNSim_runCurrentKernel = R"doc(Run current kernel on device)doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_DeviceFeNN = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_allocateDataStructures = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createBRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createDRAMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createLLMArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_createURAMArray = R"doc()doc";

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

static const char *__doc_FeNN_Backend_DeviceFeNN_m_DMARowBufferA = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_DMARowBufferB = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_FieldArray = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_KernelEventSinkSourceTableArrays = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_KernelEventSourcePopulationTableArrays = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_LLMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_PerformanceCounters = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_Runtime = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_m_URAMAllocator = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_populateDataStructures = R"doc()doc";

static const char *__doc_FeNN_Backend_DeviceFeNN_readPerformanceCounter = R"doc()doc";

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

static const char *__doc_FeNN_Backend_EventChannel = R"doc(**NOTE** this class isn't really necessary but it makes binding easier)doc";

static const char *__doc_FeNN_Backend_EventChannelSink = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannelSink_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannelSink_genEmit = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannelSink_genIncrement = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannelSink_genPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannelSink_getArrayShapeStride = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannelSink_getNumNeuronIDBits = R"doc(Get the number of bits this sink requires to encode its neuron IDs)doc";

static const char *__doc_FeNN_Backend_EventChannelSink_getPopulationID = R"doc(Get population ID this sink uses to encode its neuron IDs if this is fixed)doc";

static const char *__doc_FeNN_Backend_EventChannelSource = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannelSource_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannelSource_generateDRAMEventLoop = R"doc(Generate code to implement event loop for when weights are stored in DRAM)doc";

static const char *__doc_FeNN_Backend_EventChannelSource_generateURAMEventLoop = R"doc(Generate code to implement event loop for when weights are stored in URAM)doc";

static const char *__doc_FeNN_Backend_EventChannelSource_getArrayShapeStride = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_create = R"doc()doc";

static const char *__doc_FeNN_Backend_EventChannel_create_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_EventDrivenProcessImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_EventDrivenProcessImplementation_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_EventDrivenProcessImplementation_generateCode = R"doc()doc";

static const char *__doc_FeNN_Backend_EventDrivenProcessImplementation_getWeight = R"doc()doc";

static const char *__doc_FeNN_Backend_EventDrivenProcessImplementation_getWeightStride = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_create = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_genEmit = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_genIncrement = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_genPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkBuffer_getArrayShapeStride = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation = R"doc(Base class for event sinks into which processes send events)doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_createBitArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genBitArrayEmit = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genBitArrayIncrement = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genBitArrayPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genEmit = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genIncrement = R"doc(Generate code to advance pointer after numUnrolls unrolled)doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_genPreamble = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkImplementation_getBitArrayShapeStride = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSinkRouterKeyImplementation = R"doc(Base class for event sinks which require router keys to be allocated)doc";

static const char *__doc_FeNN_Backend_EventSinkRouterKeyImplementation_getNumNeuronIDBits = R"doc(Get the number of bits this sink requires to encode its neuron IDs)doc";

static const char *__doc_FeNN_Backend_EventSinkRouterKeyImplementation_getPopulationID = R"doc(Get population ID this sink uses to encode its neuron IDs if this is fixed)doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_EventSourceBuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_create = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_generateArchetypeDRAMEventLoop = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_generateArchetypeURAMEventLoop = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_generateDRAMEventLoop = R"doc(Generate code to implement event loop for when weights are stored in DRAM)doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_generateEventLoop = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_generateURAMEventLoop = R"doc(Generate code to implement event loop for when weights are stored in URAM)doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_getArrayShapeStride = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_m_LongEvents = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceBuffer_updateMergeHash = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_EventSourceImplementation_generateDRAMEventLoop = R"doc(Generate code to implement event loop for when weights are stored in DRAM)doc";

static const char *__doc_FeNN_Backend_EventSourceImplementation_generateURAMEventLoop = R"doc(Generate code to implement event loop for when weights are stored in URAM)doc";

static const char *__doc_FeNN_Backend_GenX320 = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320EventChannelSink = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320EventChannelSink_GenX320EventChannelSink = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320EventChannelSink_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320EventChannelSink_getArrayShapeStride = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320EventChannelSink_getNumNeuronIDBits = R"doc(Get the number of bits this sink requires to encode its neuron IDs)doc";

static const char *__doc_FeNN_Backend_GenX320EventChannelSink_getPopulationID = R"doc(Get population ID this sink uses to encode its neuron IDs if this is fixed)doc";

static const char *__doc_FeNN_Backend_GenX320_GenX320 = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320_create = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320_getSource = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320_m_Sink = R"doc()doc";

static const char *__doc_FeNN_Backend_GenX320_m_Source = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_3 = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_KernelImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_generateCode = R"doc(Generate code to implement process)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSinkIDBase = R"doc(Get the base ID of this event sink)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSinkIDs = R"doc(Get map of event sinks to their IDs)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSinkSourceTable = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSourceProcessGroup = R"doc(Get the process group in this kernel)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSourceProcessTableOffsets = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSourceProcessTableSize = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getEventSourceProcesses = R"doc(Get map of event sources to the processes they provide input to)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getMergedEventSources = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getNumNeuronIDBits = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_getNumPopulationIDBits = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSinkIDs = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSinkSourceTable = R"doc(Mapping between event sinks and)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSourceProcessGroup = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSourceProcessTableOffsets = R"doc(Offsets into event source process table)doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSourceProcessTableSize = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_EventSourceProcesses = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_MergedEventSources = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_NumNeuronIDBits = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_m_NumPopulationIDBits = R"doc()doc";

static const char *__doc_FeNN_Backend_KernelImplementation_requiresPerformanceCounters = R"doc()doc";

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

static const char *__doc_FeNN_Backend_Model_getPerformanceCounterArrayOffset = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_getPerformanceCounterArraySize = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_getStateMemSpace = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_m_PerformanceCounterArrayOffsets = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_m_PerformanceCounterArraySize = R"doc()doc";

static const char *__doc_FeNN_Backend_Model_m_StateCompatibleMemSpaces = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_NeuronUpdateProcess_updateMergeHash = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_ProcessImplementation = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_getMaxUnroll = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_m_MaxUnroll = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_setMaxUnroll = R"doc()doc";

static const char *__doc_FeNN_Backend_ProcessImplementation_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

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

static const char *__doc_FeNN_Backend_RuntimeHW_RunCurrentKernelCommand = R"doc(Command for running kernel on all devices)doc";

static const char *__doc_FeNN_Backend_RuntimeHW_RunCurrentKernelCommand_postamble = R"doc(Run any code required after the command execures on the main thread)doc";

static const char *__doc_FeNN_Backend_RuntimeHW_RunCurrentKernelCommand_preamble = R"doc(Run any code required before the command execures on the main thread)doc";

static const char *__doc_FeNN_Backend_RuntimeHW_RuntimeHW = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_createDevice = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_createRunCurrentKernelCommand = R"doc(Factory method to create a run current kernel command object)doc";

static const char *__doc_FeNN_Backend_RuntimeHW_getDeviceControl = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_getDeviceControl_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_m_DMABufferSize = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_m_DeviceControl = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeHW_m_ParentDMABuffer = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_2 = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_RunCurrentKernelCommand = R"doc(Command for running kernel on all devices)doc";

static const char *__doc_FeNN_Backend_RuntimeSim_RunCurrentKernelCommand_preamble = R"doc(Run any code required before the command execures on the main thread)doc";

static const char *__doc_FeNN_Backend_RuntimeSim_RuntimeSim = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_allocatePostamble = R"doc(Backend-specific logic to run at end of allocate function)doc";

static const char *__doc_FeNN_Backend_RuntimeSim_createDevice = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_createRunCurrentKernelCommand = R"doc(Factory method to create a run current kernel command object)doc";

static const char *__doc_FeNN_Backend_RuntimeSim_dumpMemories = R"doc(Dump the various memories on all devices to file)doc";

static const char *__doc_FeNN_Backend_RuntimeSim_getSaturationCount = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_getSlaveBufferOverflow = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_m_SharedBus = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_m_SpikeInjectData = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_m_SpikeInjectThread = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_resetSharedBus = R"doc()doc";

static const char *__doc_FeNN_Backend_RuntimeSim_spikeInjectThread = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_Runtime = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_allocatePostamble = R"doc(Backend-specific logic to run at end of allocate function)doc";

static const char *__doc_FeNN_Backend_Runtime_allocatePreamble = R"doc(Backend-specific logic to run at beginning of allocate function)doc";

static const char *__doc_FeNN_Backend_Runtime_getDMABufferSize = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_getDMARowBufferSize = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_getKernelCode = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_getKernelMergedProcessAddresses = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_getNeuronRoundingMode = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_getNumFieldBytes = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_DMABufferSize = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_DMARowBufferSize = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_KeepParamsInRegisters = R"doc()doc";

static const char *__doc_FeNN_Backend_Runtime_m_KernelCode = R"doc(Map of kernel pointers to code)doc";

static const char *__doc_FeNN_Backend_Runtime_m_KernelMergedProcessAddresses = R"doc(Map of pointers into kernels for code to handle merged event propagation processes)doc";

static const char *__doc_FeNN_Backend_Runtime_m_MergedEventSourceFields = R"doc(Map from process groups to start addresses and merged fields associated with event sources)doc";

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

static const char *__doc_FeNN_Backend_SimpleKernel_requiresPerformanceCounters = R"doc()doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel = R"doc()doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel_SimulationLoopKernel = R"doc()doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel_create = R"doc()doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel_generateCode = R"doc(Generate code to implement process)doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel_m_TargetClockCycles = R"doc()doc";

static const char *__doc_FeNN_Backend_SimulationLoopKernel_requiresPerformanceCounters = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_SparseEventPropagationProcess = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_create = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_generateArchetypeCode = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_getAllState = R"doc(Get vector of state objects used by this process)doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_getNumSparseConnectivityBits = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_getWeight = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_m_NumSparseConnectivityBits = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_m_Weight = R"doc()doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_updateCompatibleMemSpace = R"doc(Update the memory compatibility of a variable associated with this process)doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_updateCompatibleSplitDimensions =
R"doc(Update the compatible split dimensions of a state object (which should be
one used by this process) with any constraints imposed by this process))doc";

static const char *__doc_FeNN_Backend_SparseEventPropagationProcess_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

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

static const char *__doc_FeNN_Backend_Utils_getSplitDimension = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_create = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_createArray = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_getArrayShapeStride = R"doc()doc";

static const char *__doc_FeNN_Backend_Variable_getMemSpace = R"doc()doc";

static const char *__doc_FeNN_Backend_operator_band = R"doc()doc";

static const char *__doc_FeNN_Backend_operator_bor = R"doc()doc";

static const char *__doc_FeNN_Backend_operator_iand = R"doc()doc";

static const char *__doc_FeNN_Backend_operator_ior = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_allocateScalarAndZero = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_allocateScalarSeedAndInit = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_allocateVectorAndZero = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_allocateVectorSeedAndInit = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_dumpCOE = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_dumpHEX = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_getSeedData = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_loadBinaryData = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_loadScalars = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_loadVectors = R"doc()doc";

static const char *__doc_FeNN_Common_AppUtils_writeSpikes = R"doc()doc";

static const char *__doc_FeNN_Common_Bgen = R"doc()doc";

static const char *__doc_FeNN_Common_BgenCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_BgenCtrl_bias_rstn_hv = R"doc()doc";

static const char *__doc_FeNN_Common_BgenCtrl_bias_rstn_lv = R"doc()doc";

static const char *__doc_FeNN_Common_BgenCtrl_burst_transfer_hv_bank_0 = R"doc()doc";

static const char *__doc_FeNN_Common_BgenCtrl_burst_transfer_hv_bank_1 = R"doc()doc";

static const char *__doc_FeNN_Common_BgenCtrl_burst_transfer_lv_bank_0 = R"doc()doc";

static const char *__doc_FeNN_Common_BgenCtrl_burst_transfer_lv_bank_1 = R"doc()doc";

static const char *__doc_FeNN_Common_Bgen_bias_ctrl = R"doc()doc";

static const char *__doc_FeNN_Common_Bgen_bias_en = R"doc()doc";

static const char *__doc_FeNN_Common_Bgen_buf_stg = R"doc()doc";

static const char *__doc_FeNN_Common_Bgen_ibtype_sel = R"doc()doc";

static const char *__doc_FeNN_Common_Bgen_pull_sel = R"doc()doc";

static const char *__doc_FeNN_Common_Bgen_single = R"doc()doc";

static const char *__doc_FeNN_Common_Bias_Diff = R"doc()doc";

static const char *__doc_FeNN_Common_Bias_DiffOff = R"doc()doc";

static const char *__doc_FeNN_Common_Bias_DiffOn = R"doc()doc";

static const char *__doc_FeNN_Common_Bias_FO = R"doc()doc";

static const char *__doc_FeNN_Common_Bias_HPF = R"doc()doc";

static const char *__doc_FeNN_Common_Bias_Refr = R"doc()doc";

static const char *__doc_FeNN_Common_Bit = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_Bit = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_Bit_2 = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_Bit_3 = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_Bit_4 = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_Bit_5 = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_Bit_6 = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_Bit_7 = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_operator_unsigned_int = R"doc()doc";

static const char *__doc_FeNN_Common_Bit_v = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_BranchType = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_BranchType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_BranchType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_enumerated_BEQ = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_enumerated_BGE = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_enumerated_BGEU = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_enumerated_BLT = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_enumerated_BLTU = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_enumerated_BNE = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_name = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_names = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_operator_FeNN_Common_BranchType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_size = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_value = R"doc()doc";

static const char *__doc_FeNN_Common_BranchType_values = R"doc()doc";

static const char *__doc_FeNN_Common_CSR = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MARCHID = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MASTER_EVENT_BITFIELD = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MASTER_EVENT_ID_BASE = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MASTER_SEND_BARRIER = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MCAUSE = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MCONFIGPTRID = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MCOUNTINHIBIT = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MCYCLE = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MCYCLEH = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MEPC = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MHARTID = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MHPEVENT3 = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MHPEVENT31 = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MHPMCOUNTER3 = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MHPMCOUNTER31 = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MHPMCOUNTERH3 = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MHPMCOUNTERH31 = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MIE = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MIMPID = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MINSTRET = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MINSTRETH = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MIP = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MISA = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MM2S_CONTROL = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MM2S_COUNT = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MM2S_DST_ADDR = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MM2S_SRC_ADDR = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MM2S_STATUS = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MSCRATCH = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MSTATUS = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MSTATUSH = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MTVAL = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MTVEC = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MTVT = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_MVENDORID = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_S2MM_CONTROL = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_S2MM_COUNT = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_S2MM_DST_ADDR = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_S2MM_SRC_ADDR = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_S2MM_STATUS = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_SLAVE_BARRIER_COUNT = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_SLAVE_BUFFER_OVERFLOW_COUNT = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_SLAVE_EVENT_END_ADDRESS = R"doc()doc";

static const char *__doc_FeNN_Common_CSR_SLAVE_EVENT_START_ADDRESS = R"doc()doc";

static const char *__doc_FeNN_Common_ChipId = R"doc()doc";

static const char *__doc_FeNN_Common_ChipId_chip_id = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_clk_control_inversion = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_clk_out_en = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_clk_out_gating_enable = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_clk_timeout = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_drop_nbackpressure = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_enable = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_frame_fixed_size_enable = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_hot_disable_enable = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_output_data_format = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_output_fifo_bypass = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_output_if_mode = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_output_width = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_packed_fixed_rate_enable = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_packed_fixed_size_enable = R"doc()doc";

static const char *__doc_FeNN_Common_CpiPipelineControl_packet_pad_empty_enable = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_2 = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_DMABuffer = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_DMABuffer_2 = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_getData = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_getData_2 = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_getPhysicalAddress = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_getSize = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_m_Data = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_m_PhysicalAddress = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_m_Size = R"doc()doc";

static const char *__doc_FeNN_Common_DMABuffer_m_UnmapData = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_DMAController = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_MM2S_CONTROL = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_MM2S_COUNT = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_MM2S_DST_ADDR = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_MM2S_SRC_ADDR = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_MM2S_STATUS = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_S2MM_CONTROL = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_S2MM_COUNT = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_S2MM_DST_ADDR = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_S2MM_SRC_ADDR = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_Register_S2MM_STATUS = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits_ERROR_DECODE = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits_ERROR_INTERNAL = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits_ERROR_SLAVE = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits_RUNNING = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits_STATE_IDLE = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits_STATE_START = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits_STATE_TRANSFER = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_StatusBits_TRANSFER_OK = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_m_RegisterUIO = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_readReg = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_startRead = R"doc(Start read (from URAM to DMA buffer))doc";

static const char *__doc_FeNN_Common_DMAController_startWrite = R"doc(Start write (from source DMA buffer to URAM))doc";

static const char *__doc_FeNN_Common_DMAController_waitForReadComplete = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_waitForWriteComplete = R"doc()doc";

static const char *__doc_FeNN_Common_DMAController_writeReg = R"doc()doc";

static const char *__doc_FeNN_Common_Device = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_DeviceControl = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_getGPIO = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_getGPIO_2 = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_getGenX320 = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_getGenX320_2 = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_getMIPICSI2Receiver = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_getMIPICSI2Receiver_2 = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_m_GPIOUIO = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_m_GenX320 = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_m_MIPICSI2Receiver = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_setEnabled = R"doc()doc";

static const char *__doc_FeNN_Common_DeviceControl_setILATrigger = R"doc()doc";

static const char *__doc_FeNN_Common_Device_Device = R"doc()doc";

static const char *__doc_FeNN_Common_Device_getDMAController = R"doc()doc";

static const char *__doc_FeNN_Common_Device_getDMAController_2 = R"doc()doc";

static const char *__doc_FeNN_Common_Device_getDataMemory = R"doc()doc";

static const char *__doc_FeNN_Common_Device_getDataMemory_2 = R"doc()doc";

static const char *__doc_FeNN_Common_Device_getInstructionMemory = R"doc()doc";

static const char *__doc_FeNN_Common_Device_getInstructionMemory_2 = R"doc()doc";

static const char *__doc_FeNN_Common_Device_getSOCPower = R"doc()doc";

static const char *__doc_FeNN_Common_Device_m_DMAController = R"doc()doc";

static const char *__doc_FeNN_Common_Device_m_DataMemoryUIO = R"doc()doc";

static const char *__doc_FeNN_Common_Device_m_InstructionMemoryUIO = R"doc()doc";

static const char *__doc_FeNN_Common_Device_memcpyDataFromDevice = R"doc()doc";

static const char *__doc_FeNN_Common_Device_memcpyDataToDevice = R"doc()doc";

static const char *__doc_FeNN_Common_Device_uploadCode = R"doc()doc";

static const char *__doc_FeNN_Common_Device_waitOnNonZero = R"doc()doc";

static const char *__doc_FeNN_Common_DigSoftReset = R"doc()doc";

static const char *__doc_FeNN_Common_DigSoftReset_analog_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_DigSoftReset_digital_csr_srst = R"doc()doc";

static const char *__doc_FeNN_Common_DigSoftReset_digital_pipe_srst = R"doc()doc";

static const char *__doc_FeNN_Common_DigSoftReset_pdl_override = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_Control = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_Control_endianness = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_Control_format = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_ExternalOutputAdapter = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_ExternalOutputAdapter_atomic_qos_mode = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_ExternalOutputAdapter_qos_timeout = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_OutputInterfaceControl = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_OutputInterfaceControl_start_of_frame_timeout = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_PipelineControl = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_PipelineControl_bypass = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_PipelineControl_drop_nbackpressure = R"doc()doc";

static const char *__doc_FeNN_Common_EDF_PipelineControl_enable = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_AhvtDroppingControl = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_AhvtDroppingControl_drop_all_td_when_drop_geq = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_AhvtDroppingControl_h_dropping_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_AhvtDroppingControl_status = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_AhvtDroppingControl_t_dropping_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_AhvtDroppingControl_t_dropping_lut_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_AhvtDroppingControl_v_dropping_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_DelayFifoFlushAndBypass = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_DelayFifoFlushAndBypass_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_DelayFifoFlushAndBypass_status = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_alldr_evt_drop_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_avg_drop_rate_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_df_non_td_vect_drop_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_df_td_vect_drop_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_erc_td_evt_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_first_module_tag_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_hdr_evt_drop_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_in_td_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_last_module_tag_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_tdr_evt_drop_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_MonitoringEventControl_vdr_evt_drop_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_PipelineControl = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_PipelineControl_bypass = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_PipelineControl_drop_nbackpressure = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_PipelineControl_enable = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_RefPeriodFlavor = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_RefPeriodFlavor_avg_drop_rate_delayed = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_RefPeriodFlavor_reference_period = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_ResetTdropCounterOnMtagFirst = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_ResetTdropCounterOnMtagFirst_en = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_TdTargetEventCount = R"doc()doc";

static const char *__doc_FeNN_Common_ERC_TdTargetEventCount_val = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_EventFormat = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_EventFormat_2 = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_EventFormat_3 = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_enumerated_EVT2 = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_enumerated_EVT21 = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_enumerated_EVT3 = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_name = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_names = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_operator_FeNN_Common_EventFormat_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_size = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_value = R"doc()doc";

static const char *__doc_FeNN_Common_EventFormat_values = R"doc()doc";

static const char *__doc_FeNN_Common_EvtIcnClkCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_EvtIcnClkCtrl_esp_clk_en = R"doc()doc";

static const char *__doc_FeNN_Common_EvtIcnClkCtrl_evt_icn_clk_div = R"doc()doc";

static const char *__doc_FeNN_Common_EvtIcnClkCtrl_evt_icn_clk_en = R"doc()doc";

static const char *__doc_FeNN_Common_EvtIcnClkCtrl_evt_icn_clk_switch = R"doc()doc";

static const char *__doc_FeNN_Common_EvtIcnClkCtrl_ro_clk_en = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320 = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_GenX320 = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_applyROI = R"doc(Apply the configured ROI windows)doc";

static const char *__doc_FeNN_Common_GenX320_clearRegBits = R"doc(Clear bits indicated by *mask* in register  at *address*)doc";

static const char *__doc_FeNN_Common_GenX320_clearRegBits_2 = R"doc(Clear bits indicated by *mask* in register S)doc";

static const char *__doc_FeNN_Common_GenX320_disableERC = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_enableERC = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_m_CamI2C = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_m_EventFormat = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_m_GPIUIO = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_m_MIPICSI2Receiver = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_m_MuxI2C = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_m_Streaming = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_readReg = R"doc(Read the 32-bit camera register specified by address)doc";

static const char *__doc_FeNN_Common_GenX320_readReg_2 = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_readRegFields = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_resetBias = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_resetROI = R"doc(Reset ROI to full frame)doc";

static const char *__doc_FeNN_Common_GenX320_setBias = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_setBias_2 = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_setEventFormat = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_setROIWindow = R"doc(Set the ROI specified by inde)doc";

static const char *__doc_FeNN_Common_GenX320_setRegBits = R"doc(Set bits indicated by *mask* in register at *address*)doc";

static const char *__doc_FeNN_Common_GenX320_setRegBits_2 = R"doc(Set bits indicated by *mask* in register S)doc";

static const char *__doc_FeNN_Common_GenX320_startStreaming = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_stopStreaming = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_waitBoot = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_writeReg = R"doc(Write value to the 32-bit camera register specified by address)doc";

static const char *__doc_FeNN_Common_GenX320_writeReg_2 = R"doc()doc";

static const char *__doc_FeNN_Common_GenX320_writeRegFields = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_I2CInterface = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_I2CInterface_2 = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_m_I2C = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_m_SlaveAddress = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_read = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_read_2 = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_read_3 = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_write = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_write_2 = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_write_3 = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_writeRead = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_writeRead_2 = R"doc()doc";

static const char *__doc_FeNN_Common_I2CInterface_writeRead_3 = R"doc()doc";

static const char *__doc_FeNN_Common_IoCtrl2 = R"doc()doc";

static const char *__doc_FeNN_Common_IoCtrl2_sync_en = R"doc()doc";

static const char *__doc_FeNN_Common_IoCtrl2_sync_enzi = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_LoadType = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_LoadType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_LoadType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_enumerated_LB = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_enumerated_LBU = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_enumerated_LH = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_enumerated_LHU = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_enumerated_LW = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_name = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_names = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_operator_FeNN_Common_LoadType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_size = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_value = R"doc()doc";

static const char *__doc_FeNN_Common_LoadType_values = R"doc()doc";

static const char *__doc_FeNN_Common_Logging_Channel = R"doc()doc";

static const char *__doc_FeNN_Common_Logging_Channel_CHANNEL_FENN_ASSEMBLER = R"doc()doc";

static const char *__doc_FeNN_Common_Logging_Channel_CHANNEL_FENN_BACKEND = R"doc()doc";

static const char *__doc_FeNN_Common_Logging_Channel_CHANNEL_FENN_COMMON = R"doc()doc";

static const char *__doc_FeNN_Common_Logging_Channel_CHANNEL_FENN_COMPILER = R"doc()doc";

static const char *__doc_FeNN_Common_Logging_Channel_CHANNEL_FENN_ISE = R"doc()doc";

static const char *__doc_FeNN_Common_Logging_init = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_2 = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_CoreConfigBits = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_CoreConfigBits_CORE_ENABLE = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_CoreConfigBits_SOFT_RESET = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_CoreStatusBits = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_CoreStatusBits_RESET_DISABLE_IN_PROCESS = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_CoreStatusBits_SHORT_PACKET_FIFO_FULL = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_CoreStatusBits_SHORT_PACKET_FIFO_NOT_EMPTY = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_CoreStatusBits_STREAM_LINE_BUFFER_FULL = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_MIPICSI2Receiver = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_Register = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_Register_CORE_CONFIG = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_Register_CORE_STATUS = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_Register_INT_STATUS = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_Register_PROTOCOL_CONFIG = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_getActiveLanes = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_getMaxLanes = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_getPacketCount = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_getRegBit = R"doc(Get a typed bit from an enumerated register)doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_isCoreEnabled = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_isResetDisableInProgress = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_isShortPacketFIFOFull = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_isShortPacketFIFONotempty = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_isStreamLineBufferFull = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_m_RegisterUIO = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_readReg = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_setCoreEnabled = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_setInterruptStatus = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_setLanes = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_setRegBit = R"doc(Set a typed bit in an enumerated register)doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_setSoftReset = R"doc()doc";

static const char *__doc_FeNN_Common_MIPICSI2Receiver_writeReg = R"doc()doc";

static const char *__doc_FeNN_Common_Mbx_CpuSoftReset = R"doc()doc";

static const char *__doc_FeNN_Common_Mbx_CpuSoftReset_cpu_soft_reset = R"doc()doc";

static const char *__doc_FeNN_Common_Mbx_Misc = R"doc()doc";

static const char *__doc_FeNN_Common_Mbx_Misc_misc = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_BlFrame = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_BlFrame_ck_lane_hs = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_BlFrame_enable = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_BlFrame_val = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl_busy = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl_channel = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl_data_type = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl_empty = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl_enable = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl_frame_sync_en = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl_line_sync_en = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Ctrl_pkt_size = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_FrameCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_FrameCtrl_fix_rate_empty_pkt = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_FrameCtrl_frame_fix_rate_en = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_FrameCtrl_frame_fix_size_en = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_FrameCtrl_pkt_fix_rate_en = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_FrameCtrl_pkt_fix_size_en = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_FrameCtrl_pkt_timeout_en = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_ByteCnt = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_ByteCnt_val = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_Ctrl = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_Ctrl_clear = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_Ctrl_enable = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_Ctrl_trigger = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_FrameCnt = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_FrameCnt_val = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_FramePeriod = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_FramePeriod_val = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_IncPktCnt = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_IncPktCnt_val = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_PadCnt = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_PadCnt_val = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_PktCnt = R"doc()doc";

static const char *__doc_FeNN_Common_MipiCsi_Stat_PktCnt_val = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_OpImmType = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_OpImmType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_OpImmType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_ADDI = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_ANDI = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_CLZ = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_CPOP = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_CTZ = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_ORI = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_SEXT_B = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_SEXT_H = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_SLLI = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_SLTI = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_SLTIU = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_SRAI = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_SRLI = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_enumerated_XORI = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_name = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_names = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_operator_FeNN_Common_OpImmType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_size = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_value = R"doc()doc";

static const char *__doc_FeNN_Common_OpImmType_values = R"doc()doc";

static const char *__doc_FeNN_Common_OpType = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_OpType = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_OpType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_OpType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_ADD = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_AND = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_MAX = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_MAXU = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_MIN = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_MINU = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_MUL = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_OR = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SH1ADD = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SH2ADD = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SH3ADD = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SLL = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SLT = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SLTU = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SRA = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SRL = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_SUB = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_enumerated_XOR = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_name = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_names = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_operator_FeNN_Common_OpType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_size = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_value = R"doc()doc";

static const char *__doc_FeNN_Common_OpType_values = R"doc()doc";

static const char *__doc_FeNN_Common_PllCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_PllCtrl_pl_enable = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_ROIMode = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_ROIMode_2 = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_ROIMode_3 = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_enumerated_ROI = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_enumerated_RONI = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_name = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_names = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_operator_FeNN_Common_ROIMode_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_size = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_value = R"doc()doc";

static const char *__doc_FeNN_Common_ROIMode_values = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_Ctrl = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_Ctrl_px_iphoto_en = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_Ctrl_px_roi_halt_programming = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_Ctrl_px_row_mon_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_Ctrl_px_sw_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_Ctrl_roi_td_en = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_Ctrl_td_shadow_trigger = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterChickenBit = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterChickenBit_driver_register_if_en = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterChickenBit_hold_time = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterCtrl_master_busy = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterCtrl_master_done = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterCtrl_master_en = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterCtrl_master_mode = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterCtrl_master_run = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_MasterCtrl_win_nb = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_WinArray = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_WinArray_roi_win_end = R"doc()doc";

static const char *__doc_FeNN_Common_ROI_WinArray_roi_win_start = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_LpCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_LpCtrl_lp_cnt_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_LpCtrl_lp_keep_th = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_LpCtrl_lp_output_disable = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_cpm_record_mode_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_delay_ro_td_int_x_act_fal = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_delay_ro_td_int_x_act_ris = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_drop_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_drop_on_full_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_erc_self_test_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_ro_analog_pipe_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_ro_avoid_bress_td = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_ro_digital_pipe_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_ro_flip_x = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_ro_flip_y = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_ro_inv_pol_td = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_ro_self_test_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_ReadoutCtrl_ro_test_pixel_mux_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_ack_y_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_ack_y_set = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_act_pdy_drive = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_act_pu_drive = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_addr_y_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_addr_y_stat_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_arb_y_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_int_x_act_pu = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_int_x_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_int_x_stat_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_int_y_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_int_y_stat_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_reqx_ctrllast_bypass = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_sendreq_y_rstn = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TdCtrl_ro_td_sendreq_y_stat_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TimeBaseCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TimeBaseCtrl_external_mode = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TimeBaseCtrl_external_mode_enable = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TimeBaseCtrl_th_every_64us_en = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TimeBaseCtrl_time_base_enable = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TimeBaseCtrl_time_base_mode = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TimeBaseCtrl_time_base_srst = R"doc()doc";

static const char *__doc_FeNN_Common_Readout_TimeBaseCtrl_us_counter_max = R"doc()doc";

static const char *__doc_FeNN_Common_Reg = R"doc()doc";

static const char *__doc_FeNN_Common_RegBase = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X0 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X1 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X10 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X11 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X12 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X13 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X14 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X15 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X16 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X17 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X18 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X19 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X2 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X20 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X21 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X22 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X23 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X24 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X25 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X26 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X27 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X28 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X29 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X3 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X30 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X31 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X4 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X5 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X6 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X7 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X8 = R"doc()doc";

static const char *__doc_FeNN_Common_Reg_X9 = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_afk_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_cpi_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_dmem_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_ehc_stc_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_erc_dl_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_erc_ilg_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_erc_tdrop_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_imem_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_mipi_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_InitN_rom_initn = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1 = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1_cp_pd = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1_dmem_pd = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1_erc_dl_pd = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1_erc_ilg_pd = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1_erc_tdrop_pd = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1_imem_pd = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1_mipi_pd = R"doc()doc";

static const char *__doc_FeNN_Common_SRAM_Pd1_rom_pd = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_AUIPC = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_BRANCH = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_JAL = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_JALR = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_LOAD = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_LUI = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_OP = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_OP_IMM = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_STORE = R"doc()doc";

static const char *__doc_FeNN_Common_StandardOpCode_SYSTEM = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_StoreType = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_StoreType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_StoreType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_enumerated_SB = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_enumerated_SH = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_enumerated_SW = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_name = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_names = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_operator_FeNN_Common_StoreType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_size = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_value = R"doc()doc";

static const char *__doc_FeNN_Common_StoreType_values = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_StreamingSource = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_StreamingSource_2 = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_StreamingSource_3 = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_enumerated_PIXEL_ARRAY = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_enumerated_RO_PATTERN = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_enumerated_TS_PATTERN = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_name = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_names = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_operator_FeNN_Common_StreamingSource_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_size = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_value = R"doc()doc";

static const char *__doc_FeNN_Common_StreamingSource_values = R"doc()doc";

static const char *__doc_FeNN_Common_SysClkCtrl = R"doc()doc";

static const char *__doc_FeNN_Common_SysClkCtrl_phy_clk_div2 = R"doc()doc";

static const char *__doc_FeNN_Common_SysClkCtrl_phy_clk_off_count = R"doc()doc";

static const char *__doc_FeNN_Common_SysClkCtrl_phy_clk_on_count = R"doc()doc";

static const char *__doc_FeNN_Common_SysClkCtrl_sys_clk_auto_mode = R"doc()doc";

static const char *__doc_FeNN_Common_SysClkCtrl_sys_clk_en = R"doc()doc";

static const char *__doc_FeNN_Common_SysClkCtrl_sys_clk_switch = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_SystemType = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_SystemType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_SystemType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_CSRRC = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_CSRRCI = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_CSRRS = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_CSRRSI = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_CSRRW = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_CSRRWI = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_EBREAK = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_ECALL = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_name = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_names = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_operator_FeNN_Common_SystemType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_size = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_value = R"doc()doc";

static const char *__doc_FeNN_Common_SystemType_values = R"doc()doc";

static const char *__doc_FeNN_Common_TdRoiXArray = R"doc()doc";

static const char *__doc_FeNN_Common_TdRoiXArray_value = R"doc()doc";

static const char *__doc_FeNN_Common_TdRoiYArray = R"doc()doc";

static const char *__doc_FeNN_Common_TdRoiYArray_value = R"doc()doc";

static const char *__doc_FeNN_Common_UIO = R"doc()doc";

static const char *__doc_FeNN_Common_UIO_UIO = R"doc()doc";

static const char *__doc_FeNN_Common_UIO_getData = R"doc()doc";

static const char *__doc_FeNN_Common_UIO_getData_2 = R"doc()doc";

static const char *__doc_FeNN_Common_UIO_getSize = R"doc()doc";

static const char *__doc_FeNN_Common_UIO_m_Data = R"doc()doc";

static const char *__doc_FeNN_Common_UIO_m_Size = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_VLoadType = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_VLoadType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_VLoadType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_enumerated_VLOAD = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_enumerated_VLOAD_L = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_enumerated_VLOAD_R0 = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_enumerated_VLOAD_R1 = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_name = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_names = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_operator_FeNN_Common_VLoadType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_size = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_value = R"doc()doc";

static const char *__doc_FeNN_Common_VLoadType_values = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_VMovType = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_VMovType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_VMovType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_enumerated_VEXTRACT = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_enumerated_VFILL = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_name = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_names = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_operator_FeNN_Common_VMovType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_size = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_value = R"doc()doc";

static const char *__doc_FeNN_Common_VMovType_values = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_VOpImmType = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_VOpImmType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_VOpImmType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_enumerated_VSLLI = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_enumerated_VSRAI = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_enumerated_VSRAI_RN = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_enumerated_VSRAI_RS = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_name = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_names = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_operator_FeNN_Common_VOpImmType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_size = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_value = R"doc()doc";

static const char *__doc_FeNN_Common_VOpImmType_values = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_VOpType = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_VOpType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_VOpType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_VADD = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_VAND = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_VMUL = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_VMUL_RN = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_VMUL_RS = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_VSLL = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_VSRA = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_enumerated_VSUB = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_name = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_names = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_operator_FeNN_Common_VOpType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_size = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_value = R"doc()doc";

static const char *__doc_FeNN_Common_VOpType_values = R"doc()doc";

static const char *__doc_FeNN_Common_VReg = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V0 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V1 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V10 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V11 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V12 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V13 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V14 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V15 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V16 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V17 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V18 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V19 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V2 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V20 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V21 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V22 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V23 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V24 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V25 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V26 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V27 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V28 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V29 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V3 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V30 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V31 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V4 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V5 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V6 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V7 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V8 = R"doc()doc";

static const char *__doc_FeNN_Common_VReg_V9 = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_VSpcType = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_VSpcType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_VSpcType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_enumerated_VANDADD = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_enumerated_VRNG = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_name = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_names = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_operator_FeNN_Common_VSpcType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_size = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_value = R"doc()doc";

static const char *__doc_FeNN_Common_VSpcType_values = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_VStoreType = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_VStoreType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_VStoreType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_enumerated_VSTORE = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_enumerated_VSTORE_L = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_name = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_names = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_operator_FeNN_Common_VStoreType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_size = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_value = R"doc()doc";

static const char *__doc_FeNN_Common_VStoreType_values = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_VTstType = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_VTstType_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_VTstType_3 = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_enumerated_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_enumerated_VTEQ = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_enumerated_VTGE = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_enumerated_VTLT = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_enumerated_VTNE = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_name = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_names = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_operator_FeNN_Common_VTstType_enumerated = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_size = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_value = R"doc()doc";

static const char *__doc_FeNN_Common_VTstType_values = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VLOAD = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VLUI = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VMOV = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VSEL = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VSOP = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VSOP_IMM = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VSPC = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VSTORE = R"doc()doc";

static const char *__doc_FeNN_Common_VectorOpCode_VTST = R"doc()doc";

static const char *__doc_FeNN_Common_addQuadrant = R"doc()doc";

static const char *__doc_FeNN_Common_addQuadrant_2 = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_putNamesInThisScopeAlso_BEQ = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_putNamesInThisScopeAlso_BGE = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_putNamesInThisScopeAlso_BGEU = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_putNamesInThisScopeAlso_BLT = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_putNamesInThisScopeAlso_BLTU = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_putNamesInThisScopeAlso_BNE = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_BranchType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_EventFormat_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_EventFormat_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_EventFormat_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_EventFormat_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_EventFormat_putNamesInThisScopeAlso_EVT2 = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_EventFormat_putNamesInThisScopeAlso_EVT21 = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_EventFormat_putNamesInThisScopeAlso_EVT3 = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_EventFormat_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_putNamesInThisScopeAlso_LB = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_putNamesInThisScopeAlso_LBU = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_putNamesInThisScopeAlso_LH = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_putNamesInThisScopeAlso_LHU = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_putNamesInThisScopeAlso_LW = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_LoadType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_ADDI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_ANDI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_CLZ = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_CPOP = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_CTZ = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_ORI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_SEXT_B = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_SEXT_H = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_SLLI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_SLTI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_SLTIU = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_SRAI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_SRLI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_putNamesInThisScopeAlso_XORI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpImmType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_ADD = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_AND = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_MAX = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_MAXU = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_MIN = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_MINU = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_MUL = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_OR = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SH1ADD = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SH2ADD = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SH3ADD = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SLL = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SLT = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SLTU = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SRA = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SRL = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_SUB = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_putNamesInThisScopeAlso_XOR = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_OpType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_ROIMode_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_ROIMode_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_ROIMode_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_ROIMode_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_ROIMode_putNamesInThisScopeAlso_ROI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_ROIMode_putNamesInThisScopeAlso_RONI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_ROIMode_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_putNamesInThisScopeAlso_SB = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_putNamesInThisScopeAlso_SH = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_putNamesInThisScopeAlso_SW = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StoreType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StreamingSource_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StreamingSource_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StreamingSource_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StreamingSource_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StreamingSource_putNamesInThisScopeAlso_PIXEL_ARRAY = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StreamingSource_putNamesInThisScopeAlso_RO_PATTERN = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StreamingSource_putNamesInThisScopeAlso_TS_PATTERN = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_StreamingSource_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_CSRRC = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_CSRRCI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_CSRRS = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_CSRRSI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_CSRRW = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_CSRRWI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_EBREAK = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_ECALL = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_SystemType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_putNamesInThisScopeAlso_VLOAD = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_putNamesInThisScopeAlso_VLOAD_L = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_putNamesInThisScopeAlso_VLOAD_R0 = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_putNamesInThisScopeAlso_VLOAD_R1 = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VLoadType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VMovType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VMovType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VMovType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VMovType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VMovType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VMovType_putNamesInThisScopeAlso_VEXTRACT = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VMovType_putNamesInThisScopeAlso_VFILL = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VMovType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_putNamesInThisScopeAlso_VSLLI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_putNamesInThisScopeAlso_VSRAI = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_putNamesInThisScopeAlso_VSRAI_RN = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_putNamesInThisScopeAlso_VSRAI_RS = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpImmType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_VADD = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_VAND = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_VMUL = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_VMUL_RN = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_VMUL_RS = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_VSLL = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_VSRA = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_putNamesInThisScopeAlso_VSUB = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VOpType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VSpcType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VSpcType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VSpcType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VSpcType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VSpcType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VSpcType_putNamesInThisScopeAlso_VANDADD = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VSpcType_putNamesInThisScopeAlso_VRNG = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VSpcType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VStoreType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VStoreType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VStoreType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VStoreType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VStoreType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VStoreType_putNamesInThisScopeAlso_VSTORE = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VStoreType_putNamesInThisScopeAlso_VSTORE_L = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VStoreType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_initialized = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_name_array = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_name_storage = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_putNamesInThisScopeAlso = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_putNamesInThisScopeAlso_INVALID = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_putNamesInThisScopeAlso_VTEQ = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_putNamesInThisScopeAlso_VTGE = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_putNamesInThisScopeAlso_VTLT = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_putNamesInThisScopeAlso_VTNE = R"doc()doc";

static const char *__doc_FeNN_Common_better_enums_data_VTstType_raw_names = R"doc()doc";

static const char *__doc_FeNN_Common_decodeBType = R"doc()doc";

static const char *__doc_FeNN_Common_decodeIType = R"doc()doc";

static const char *__doc_FeNN_Common_decodeJType = R"doc()doc";

static const char *__doc_FeNN_Common_decodeRType = R"doc()doc";

static const char *__doc_FeNN_Common_decodeSType = R"doc()doc";

static const char *__doc_FeNN_Common_decodeUType = R"doc()doc";

static const char *__doc_FeNN_Common_from_index = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_nothrow_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_index_unchecked_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_nothrow_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_integral_unchecked_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_loop_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_loop_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nocase_nothrow_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_string_nothrow_16 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_2 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_3 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_4 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_5 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_6 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_7 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_8 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_9 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_10 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_11 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_12 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_13 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_14 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_15 = R"doc()doc";

static const char *__doc_FeNN_Common_from_value_loop_16 = R"doc()doc";

static const char *__doc_FeNN_Common_getBranchType = R"doc()doc";

static const char *__doc_FeNN_Common_getLoadType = R"doc()doc";

static const char *__doc_FeNN_Common_getOpImmType = R"doc()doc";

static const char *__doc_FeNN_Common_getOpType = R"doc()doc";

static const char *__doc_FeNN_Common_getStoreType = R"doc()doc";

static const char *__doc_FeNN_Common_getSystemType = R"doc()doc";

static const char *__doc_FeNN_Common_getVLoadType = R"doc()doc";

static const char *__doc_FeNN_Common_getVMovType = R"doc()doc";

static const char *__doc_FeNN_Common_getVOpImmType = R"doc()doc";

static const char *__doc_FeNN_Common_getVOpType = R"doc()doc";

static const char *__doc_FeNN_Common_getVSpcType = R"doc()doc";

static const char *__doc_FeNN_Common_getVStoreType = R"doc()doc";

static const char *__doc_FeNN_Common_getVTstType = R"doc()doc";

static const char *__doc_FeNN_Common_inBit = R"doc()doc";

static const char *__doc_FeNN_Common_inSBit = R"doc()doc";

static const char *__doc_FeNN_Common_initialize = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_2 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_3 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_4 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_5 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_6 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_7 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_8 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_9 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_10 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_11 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_12 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_13 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_14 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_15 = R"doc()doc";

static const char *__doc_FeNN_Common_initialize_16 = R"doc()doc";

static const char *__doc_FeNN_Common_isValidImm = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_2 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_3 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_4 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_5 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_6 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_7 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_8 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_9 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_10 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_11 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_12 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_13 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_14 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_15 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_16 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_17 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_18 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_19 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_20 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_21 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_22 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_23 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_24 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_25 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_26 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_27 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_28 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_29 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_30 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_31 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_32 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_2 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_3 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_4 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_5 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_6 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_7 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_8 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_9 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_10 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_11 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_12 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_13 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_14 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_15 = R"doc()doc";

static const char *__doc_FeNN_Common_is_valid_nocase_16 = R"doc()doc";

static const char *__doc_FeNN_Common_mask = R"doc()doc";

static const char *__doc_FeNN_Common_name = R"doc()doc";

static const char *__doc_FeNN_Common_name_2 = R"doc()doc";

static const char *__doc_FeNN_Common_name_3 = R"doc()doc";

static const char *__doc_FeNN_Common_name_4 = R"doc()doc";

static const char *__doc_FeNN_Common_name_5 = R"doc()doc";

static const char *__doc_FeNN_Common_name_6 = R"doc()doc";

static const char *__doc_FeNN_Common_name_7 = R"doc()doc";

static const char *__doc_FeNN_Common_name_8 = R"doc()doc";

static const char *__doc_FeNN_Common_name_9 = R"doc()doc";

static const char *__doc_FeNN_Common_name_10 = R"doc()doc";

static const char *__doc_FeNN_Common_name_11 = R"doc()doc";

static const char *__doc_FeNN_Common_name_12 = R"doc()doc";

static const char *__doc_FeNN_Common_name_13 = R"doc()doc";

static const char *__doc_FeNN_Common_name_14 = R"doc()doc";

static const char *__doc_FeNN_Common_name_15 = R"doc()doc";

static const char *__doc_FeNN_Common_name_16 = R"doc()doc";

static const char *__doc_FeNN_Common_names = R"doc()doc";

static const char *__doc_FeNN_Common_names_2 = R"doc()doc";

static const char *__doc_FeNN_Common_names_3 = R"doc()doc";

static const char *__doc_FeNN_Common_names_4 = R"doc()doc";

static const char *__doc_FeNN_Common_names_5 = R"doc()doc";

static const char *__doc_FeNN_Common_names_6 = R"doc()doc";

static const char *__doc_FeNN_Common_names_7 = R"doc()doc";

static const char *__doc_FeNN_Common_names_8 = R"doc()doc";

static const char *__doc_FeNN_Common_names_9 = R"doc()doc";

static const char *__doc_FeNN_Common_names_10 = R"doc()doc";

static const char *__doc_FeNN_Common_names_11 = R"doc()doc";

static const char *__doc_FeNN_Common_names_12 = R"doc()doc";

static const char *__doc_FeNN_Common_names_13 = R"doc()doc";

static const char *__doc_FeNN_Common_names_14 = R"doc()doc";

static const char *__doc_FeNN_Common_names_15 = R"doc()doc";

static const char *__doc_FeNN_Common_names_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_add_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_eq_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ge_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_gt_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_le_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lshift_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_lt_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_ne_16 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_2 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_3 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_4 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_5 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_6 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_7 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_8 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_9 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_10 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_11 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_12 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_13 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_14 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_15 = R"doc()doc";

static const char *__doc_FeNN_Common_operator_rshift_16 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_2 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_3 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_4 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_5 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_6 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_7 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_8 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_9 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_10 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_11 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_12 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_13 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_14 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_15 = R"doc()doc";

static const char *__doc_FeNN_Common_to_index_16 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_2 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_3 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_4 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_5 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_6 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_7 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_8 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_9 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_10 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_11 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_12 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_13 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_14 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_15 = R"doc()doc";

static const char *__doc_FeNN_Common_to_integral_16 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_2 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_3 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_4 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_5 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_6 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_7 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_8 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_9 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_10 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_11 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_12 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_13 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_14 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_15 = R"doc()doc";

static const char *__doc_FeNN_Common_to_string_16 = R"doc()doc";

static const char *__doc_FeNN_Common_values = R"doc()doc";

static const char *__doc_FeNN_Common_values_2 = R"doc()doc";

static const char *__doc_FeNN_Common_values_3 = R"doc()doc";

static const char *__doc_FeNN_Common_values_4 = R"doc()doc";

static const char *__doc_FeNN_Common_values_5 = R"doc()doc";

static const char *__doc_FeNN_Common_values_6 = R"doc()doc";

static const char *__doc_FeNN_Common_values_7 = R"doc()doc";

static const char *__doc_FeNN_Common_values_8 = R"doc()doc";

static const char *__doc_FeNN_Common_values_9 = R"doc()doc";

static const char *__doc_FeNN_Common_values_10 = R"doc()doc";

static const char *__doc_FeNN_Common_values_11 = R"doc()doc";

static const char *__doc_FeNN_Common_values_12 = R"doc()doc";

static const char *__doc_FeNN_Common_values_13 = R"doc()doc";

static const char *__doc_FeNN_Common_values_14 = R"doc()doc";

static const char *__doc_FeNN_Common_values_15 = R"doc()doc";

static const char *__doc_FeNN_Common_values_16 = R"doc()doc";

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
which emit them e.g. NeuronUpdateProcess)doc";

static const char *__doc_Frontend_EventSource = R"doc()doc";

static const char *__doc_Frontend_EventSource_2 = R"doc()doc";

static const char *__doc_Frontend_ModelComponent = R"doc()doc";

static const char *__doc_Frontend_Process = R"doc()doc";

static const char *__doc_Frontend_Process_2 = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_2 = R"doc()doc";

static const char *__doc_Frontend_State = R"doc()doc";

static const char *__doc_Frontend_State_2 = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_DMA_COHERENT = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_DMA_MASK = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_EXPORT_FD_FLAGS = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_IN_KERNEL_FUNCTIONS = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_IOCTL_VERSION = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_MMAP_MODE = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_SYNC_CMD = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_SYNC_DIR = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_SYNC_MODE = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_SYNC_OWNER = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_USE_OF_DMA_CONFIG = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_USE_OF_RESERVED_MEM = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_USE_QUIRK_MMAP = R"doc()doc";

static const char *__doc_GET_U_DMA_BUF_IOCTL_FLAGS_USE_QUIRK_MMAP_PAGE = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_DMA_COHERENT = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_DMA_MASK = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_EXPORT_FD_FLAGS = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_IN_KERNEL_FUNCTIONS = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_IOCTL_VERSION = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_MMAP_MODE = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_SYNC_CMD = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_SYNC_DIR = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_SYNC_MODE = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_SYNC_OWNER = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_USE_OF_DMA_CONFIG = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_USE_OF_RESERVED_MEM = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_USE_QUIRK_MMAP = R"doc()doc";

static const char *__doc_SET_U_DMA_BUF_IOCTL_FLAGS_USE_QUIRK_MMAP_PAGE = R"doc()doc";

static const char *__doc_U_DMA_BUF_IOCTL_FLAGS_SYNC_CMD_FOR_CPU = R"doc()doc";

static const char *__doc_U_DMA_BUF_IOCTL_FLAGS_SYNC_CMD_FOR_DEVICE = R"doc()doc";

static const char *__doc_addr = R"doc()doc";

static const char *__doc_addr_2 = R"doc()doc";

static const char *__doc_boost_uuids_detail_sha1 = R"doc()doc";

static const char *__doc_fd = R"doc()doc";

static const char *__doc_flags = R"doc()doc";

static const char *__doc_flags_2 = R"doc()doc";

static const char *__doc_flags_3 = R"doc()doc";

static const char *__doc_flags_4 = R"doc()doc";

static const char *__doc_offset = R"doc()doc";

static const char *__doc_offset_2 = R"doc()doc";

static const char *__doc_plog_IAppender = R"doc()doc";

static const char *__doc_size = R"doc()doc";

static const char *__doc_size_2 = R"doc()doc";

static const char *__doc_size_3 = R"doc()doc";

static const char *__doc_version = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

