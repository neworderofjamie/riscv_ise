# python imports
import sys

# pyfenn interface
from ._fenn import (EventContainer, EventPropagationProcess, ModelComponent,
                    NeuronUpdateProcess, Process, ProcessGroup, Shape, State,
                    Variable)

__all__ = ["EventContainer", "EventPropagationProcess", "ModelComponent",
           "NeuronUpdateProcess", "Process", "ProcessGroup", "Shape",
           "State", "Variable"]

if sys.version_info >= (3, 8):
    from importlib import metadata
else:
    import importlib_metadata as metadata

__version__ = metadata.version("pyfenn")
