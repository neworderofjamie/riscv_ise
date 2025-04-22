# python imports
import sys

# pyfenn interface
from ._fenn import (BackendFeNNSim, EventContainer, EventPropagationProcess,
                    Model, ModelComponent, NeuronUpdateProcess, NumericValue,
                    Parameter, Process, ProcessGroup, RNGInitProcess, Runtime,
                    Shape, State, UnresolvedType, Variable,
                    disassemble, init_logging)

__all__ = ["BackendFeNNSim", "EventContainer", "EventPropagationProcess",
           "Model", "ModelComponent", "NeuronUpdateProcess", "NumericValue",
           "Parameter", "Process", "ProcessGroup", "RNGInitProcess",
           "Runtime", "Shape", "State", "UnresolvedType", "Variable",
           disassemble, init_logging]

if sys.version_info >= (3, 8):
    from importlib import metadata
else:
    import importlib_metadata as metadata

__version__ = metadata.version("pyfenn")
