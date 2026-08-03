# Add GeNN include directories
CXXFLAGS +=$(shell pkg-config libffi --cflags)
LDFLAGS	+=-lcommon$(PREFIX) -lfrontend$(PREFIX) -lfenn_assembler$(PREFIX) -lfenn_common$(PREFIX) -lfenn_compiler$(PREFIX) -lfenn_ise$(PREFIX) $(shell pkg-config libffi --libs)
LIBRARY_NAME:=fenn_backend

# Include common makefile
include ../../MakefileCommonLibrary
