# python imports
from importlib import metadata

# pyfenn interface
from . import fenn_backend, utils

__all__ = ["fenn_backend", "utils"]

__version__ = metadata.version("pyfenn")
