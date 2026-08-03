# Add GeNN include directories
CXXFLAGS +=-I$(RISCV_DIR)/include/third_party $(shell pkg-config libffi --cflags)
LDFLAGS	+=-lcommon$(PREFIX) -lfenn_common$(PREFIX) $(shell pkg-config libffi --libs)
LIBRARY_NAME:=fenn_compiler

# Include common makefile
include ../../MakefileCommonLibrary
