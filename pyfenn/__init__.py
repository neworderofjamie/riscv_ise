# python imports
import sys

# pyfenn interface
from ._fenn import (BackendFeNNSim, CopyProcess, EventContainer,
                    EventPropagationProcess, Model, ModelComponent,
                    NeuronUpdateProcess, NumericValue, Parameter, 
                    PlogSeverity, Process, ProcessGroup, RNGInitProcess, 
                    Runtime, Shape, State, UnresolvedType, Variable,
                    disassemble, init_logging)

from . import utils

__all__ = ["CopyProcess", "BackendFeNNSim", "EventContainer",
           "EventPropagationProcess", "Model", "ModelComponent",
           "NeuronUpdateProcess", "NumericValue", "Parameter", "PlogSeverity",
           "Process", "ProcessGroup", "RNGInitProcess", "Runtime", "Shape",
           "State", "UnresolvedType", "Variable", 
           "disassemble", "init_logging", "utils"]

if sys.version_info >= (3, 8):
    from importlib import metadata
else:
    import importlib_metadata as metadata

__version__ = metadata.version("pyfenn")
