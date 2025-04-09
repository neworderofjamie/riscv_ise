# python imports
import sys

# pyfenn interface
from ._fenn import (EventContainer, ModelComponent, Process, Shape, State)

__all__ = ["EventContainer", "ModelComponent", "Process", "Shape", "State"]

if sys.version_info >= (3, 8):
    from importlib import metadata
else:
    import importlib_metadata as metadata

__version__ = metadata.version("pyfenn")
