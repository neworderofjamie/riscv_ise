# Add GeNN include directories
CXXFLAGS +=-I$(RISCV_DIR)/genn/include/genn/genn -I$(RISCV_DIR)/genn/include/genn/third_party $(shell pkg-config libffi --cflags)

LIBRARY_NAME:=fenn_compiler

# Include common makefile
include ../../MakefileCommonLibrary
