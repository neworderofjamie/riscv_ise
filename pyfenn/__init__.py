# python imports
import sys

# pyfenn interface
from ._fenn import (BackendFeNNHW, BackendFeNNSim, BroadcastProcess,  
                    EventContainer, EventPropagationProcess, MemsetProcess,
                    Model, ModelComponent, NeuronUpdateProcess, NumericValue,
                    Parameter, PerformanceCounter, PlogSeverity, Process,
                    ProcessGroup, RNGInitProcess, RoundingMode, Runtime, 
                    Shape, State, UnresolvedType, Variable,
                    disassemble, init_logging)

from . import models, nir_import, utils

__all__ = ["BackendFeNNHW", "BackendFeNNSim", "BroadcastProcess",
           "EventContainer", "EventPropagationProcess", "MemsetProcess",
           "Model", "ModelComponent", "NeuronUpdateProcess", "NumericValue",
           "Parameter", "PerformanceCounter", "PlogSeverity", "Process",
           "ProcessGroup", "RNGInitProcess", "RoundingMode", "Runtime",
           "Shape", "State", "UnresolvedType", "Variable",
           "disassemble", "init_logging", "models", "nir_import", "utils"]

if sys.version_info >= (3, 8):
    from importlib import metadata
else:
    import importlib_metadata as metadata

__version__ = metadata.version("pyfenn")
