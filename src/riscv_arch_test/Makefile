# Include common makefile
include ../MakefileCommon

LIBRISCV_ARCH_TEST:=$(LIBRARY_DIRECTORY)/libriscv_arch_test$(PREFIX).$(LIBRARY_EXTENSION)

# Find source files
SOURCES	:=$(wildcard *.cc)

# Build objecs in sub-directory
OBJECT_DIRECTORY	:=$(OBJECT_DIRECTORY)/riscv_arch_test/

# Add object directory prefix
OBJECTS			:=$(SOURCES:%.cc=$(OBJECT_DIRECTORY)/%.o)
DEPS			:=$(OBJECTS:.o=.d)

# Add compiler linker flags
CXXFLAGS		+= -I$(RISCV_DIR)/include/riscv_arch_test -Wno-shift-negative-value

.PHONY: all clean

all: $(LIBRISCV_ARCH_TEST)

ifdef DYNAMIC
$(LIBRISCV_ARCH_TEST): $(OBJECTS)
	mkdir -p $(@D)
	$(CXX) -shared $(CXXFLAGS) -o $@ $(OBJECTS)
else
$(LIBRISCV_ARCH_TEST): $(OBJECTS)
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
	@rm -f $(LIBRISCV_ARCH_TEST)
