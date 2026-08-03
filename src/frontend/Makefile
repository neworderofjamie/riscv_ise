# Add GeNN include directories
CXXFLAGS +=$(shell pkg-config libffi --cflags)
LDFLAGS	+=-lcommon$(PREFIX) -lcompiler_frontend$(PREFIX) $(shell pkg-config libffi --libs)
LIBRARY_NAME:=frontend

# Include common makefile
include ../MakefileCommonLibrary
