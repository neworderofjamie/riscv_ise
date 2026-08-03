# Add FFI include directories
CXXFLAGS += $(shell pkg-config libffi --cflags)
LDFLAGS	+=-lcommon$(PREFIX)
LIBRARY_NAME:=compiler_frontend

# Include common makefile
include ../MakefileCommonLibrary
