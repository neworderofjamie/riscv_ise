from ._fenn_backend import (BroadcastProcess, DenseEventPropagationProcess, 
                            EventChannel, EventSinkBuffer, EventSourceBuffer, 
                            MemsetProcess, NeuronUpdateProcess, 
                            RNGInitProcess, RoundingMode, RuntimeHW, 
                            RuntimeSim, SimpleKernel, SimulationLoopKernel,
                            SparseEventPropagationProcess, Variable, 
                            disassemble)
                            
from ._frontend import (NumericValue, PlogSeverity, ProcessGroup, 
                        ResolvedType, Shape, SlicedEventSink, 
                        SlicedEventSource, SlicedVariable, UnresolvedType)

def init_logging(appender, level=PlogSeverity.INFO, 
                 frontend_level=None, compiler_frontend_level=None,
                 fenn_common_level=None, fenn_assembler_level=None,
                 fenn_backend_level=None, fenn_compiler_level=None,
                 fenn_ise_level=None):

    from ._frontend import init_logging as init_frontend_logging
    from  ._fenn_backend import init_logging as init_fenn_logging
    
    # Initialise frontend logging
    init_frontend_logging(frontend_level or level,
                          compiler_frontend_level or level,
                          appender, appender)
    
    # Initialise backend logging
    init_fenn_logging(fenn_common_level or level,
                      fenn_assembler_level or level,
                      fenn_backend_level or level,
                      fenn_compiler_level or level,
                      fenn_ise_level or level,
                      appender, appender, appender, 
                      appender, appender)