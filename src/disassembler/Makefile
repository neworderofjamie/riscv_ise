# Include common makefile
include ../MakefileCommon

LIBDISASSEMBLER:=$(LIBRARY_DIRECTORY)/libdisassembler$(PREFIX).$(LIBRARY_EXTENSION)

# Find source files
SOURCES	:=$(wildcard *.cc)

# Build objecs in sub-directory
OBJECT_DIRECTORY	:=$(OBJECT_DIRECTORY)/disassembler/

# Add object directory prefix
OBJECTS			:=$(SOURCES:%.cc=$(OBJECT_DIRECTORY)/%.o)
DEPS			:=$(OBJECTS:.o=.d)

.PHONY: all clean

all: $(LIBDISASSEMBLER)

ifdef DYNAMIC
ifeq ($(DARWIN),DARWIN)
$(LIBDISASSEMBLER): $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) -dynamiclib -undefined dynamic_lookup $(CXXFLAGS) -o $@ $(OBJECTS)
	install_name_tool -id "@loader_path/$(@F)" $@
else
$(LIBDISASSEMBLER): $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) -shared $(CXXFLAGS) -o $@ $(OBJECTS)
endif
else
$(LIBDISASSEMBLER): $(OBJECTS)
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
	@rm -f $(LIBDISASSEMBLER)
