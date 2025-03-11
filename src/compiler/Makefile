# Include common makefile
include ../MakefileCommon

LIBCOMPILER:=$(LIBRARY_DIRECTORY)/libcompiler$(PREFIX).$(LIBRARY_EXTENSION)

# Find source files
SOURCES	:=$(wildcard *.cc)

# Build objecs in sub-directory
OBJECT_DIRECTORY	:=$(OBJECT_DIRECTORY)/compiler/

# Add object directory prefix
OBJECTS			:=$(SOURCES:%.cc=$(OBJECT_DIRECTORY)/%.o)
DEPS			:=$(OBJECTS:.o=.d)

# Add GeNN include directories
CXXFLAGS +=-I$(GENN_PATH)/include/genn/genn -I$(GENN_PATH)/include/genn/third_party $(shell pkg-config libffi --cflags)

.PHONY: all clean

all: $(LIBCOMPILER)

ifdef DYNAMIC
$(LIBCOMPILER): $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) -shared $(CXXFLAGS) -o $@ $(OBJECTS)
else
$(LIBCOMPILER): $(OBJECTS)
	mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)
endif

-include $(DEPS)

$(OBJECT_DIRECTORY)/%.o: %.cc $(OBJECT_DIRECTORY)/%.d
	mkdir -p $(@D)
	$(CXX) -std=c++17 $(CXXFLAGS) -c -o $@ $<

%.d: ;

clean:
	@find $(OBJECT_DIRECTORY) -type f -name "*.o" -delete
	@find $(OBJECT_DIRECTORY) -type f -name "*.d" -delete
	@rm -f $(LIBCOMMON)
