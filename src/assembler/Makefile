# Include common makefile
include ../MakefileCommon

LIBASSEMBLER:=$(LIBRARY_DIRECTORY)/libassembler$(PREFIX).$(LIBRARY_EXTENSION)

# Find source files
SOURCES	:=$(wildcard *.cc)

# Build objecs in sub-directory
OBJECT_DIRECTORY	:=$(OBJECT_DIRECTORY)/assembler/

# Add object directory prefix
OBJECTS			:=$(SOURCES:%.cc=$(OBJECT_DIRECTORY)/%.o)
DEPS			:=$(OBJECTS:.o=.d)

.PHONY: all clean

all: $(LIBASSEMBLER)

ifdef DYNAMIC
ifeq ($(DARWIN),DARWIN)
$(LIBASSEMBLER): $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) -dynamiclib -undefined dynamic_lookup $(CXXFLAGS) -o $@ $(OBJECTS)
	install_name_tool -id "@loader_path/$(@F)" $@
else
$(LIBASSEMBLER): $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) -shared $(CXXFLAGS) -o $@ $(OBJECTS)
endif
else
$(LIBASSEMBLER): $(OBJECTS)
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
