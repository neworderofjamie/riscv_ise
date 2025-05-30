# Include common makefile
include ../../src/MakefileCommon

# Get tests path i.e. directory of this Makefile
TEST_PATH		:=$(RISCV_DIR)/tests/simulator
OBJECT_DIRECTORY	:=$(OBJECT_DIRECTORY)/tests/simulator

SOURCES			:=$(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc $(wildcard *.cc)
OBJECTS			:=$(SOURCES:%.cc=$(OBJECT_DIRECTORY)/%.o)
DEPS			:=$(OBJECTS:.o=.d)

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lriscv_arch_test$(PREFIX) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX) -lpthread
CXXFLAGS		+= -I "$(GTEST_DIR)" -isystem "$(GTEST_DIR)/include" -Wno-shift-negative-value

# Determine full path to generator and backend
TEST			:=$(TEST_PATH)/test$(PREFIX)

.PHONY: all clean libise libcommon libassembler libriscv_arch_test

all: $(TEST)

$(TEST): $(OBJECTS) libise libcommon libassembler libriscv_arch_test
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

-include $(DEPS)

$(OBJECT_DIRECTORY)/%.o: %.cc $(OBJECT_DIRECTORY)/%.d
	mkdir -p $(@D)
	$(CXX) -std=c++17 $(CXXFLAGS) -c -o $@ $<

%.d: ;

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

libriscv_arch_test:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/riscv_arch_test; fi;

clean:
	rm -f $(OBJECT_DIRECTORY)/*.o $(OBJECT_DIRECTORY)/*.d $(TEST)
