# Include common makefile
include ../../src/MakefileCommon

# Get SOC test path i.e. directory of this Makefile
SOC_TEST_PATH		:=$(RISCV_DIR)/tests/soc
OBJECT_DIRECTORY	:=$(OBJECT_DIRECTORY)/tests/soc

SOURCES			:=$(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc $(wildcard *.cc)
OBJECTS			:=$(SOURCES:%.cc=$(OBJECT_DIRECTORY)/%.o)
DEPS			:=$(OBJECTS:.o=.d)

# Add compiler and linker flags to link libGeNN and pthreads
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX) -lpthread
CXXFLAGS		+= -I "$(GTEST_DIR)" -isystem "$(GTEST_DIR)/include" 

# Determine full path to generator and backend
TEST			:=$(SOC_TEST_PATH)/test$(PREFIX)

.PHONY: all clean libise libcommon libassembler

all: $(TEST)

$(TEST): $(OBJECTS) libise libcommon libassembler
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


clean:
	rm -f $(OBJECT_DIRECTORY)/*.o $(OBJECT_DIRECTORY)/*.d $(TEST)
