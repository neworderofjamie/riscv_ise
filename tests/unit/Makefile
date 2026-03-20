# Include common makefile
include ../../src/MakefileCommon

# Get SOC test path i.e. directory of this Makefile
TEST_PATH		:=$(RISCV_DIR)/tests/unit
OBJECT_DIRECTORY	:=$(OBJECT_DIRECTORY)/tests/unit

SOURCES			:=$(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc $(shell find -type f -name '*.cc')
OBJECTS			:=$(SOURCES:%.cc=$(OBJECT_DIRECTORY)/%.o)
DEPS			:=$(OBJECTS:.o=.d)

# Add compiler and linker flags to link libGeNN and pthreads
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lcommon$(PREFIX) -lcompiler_frontend$(PREFIX) -lfrontend$(PREFIX) -lpthread -lffi
CXXFLAGS		+= -I "$(GTEST_DIR)" -isystem "$(GTEST_DIR)/include" 

# Determine full path to generator and backend
TEST			:=$(TEST_PATH)/test$(PREFIX)

.PHONY: all clean libcommon libcompiler_frontend libfrontend

all: $(TEST)

$(TEST): $(OBJECTS) libcommon libcompiler_frontend libfrontend
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

-include $(DEPS)

$(OBJECT_DIRECTORY)/%.o: %.cc $(OBJECT_DIRECTORY)/%.d
	mkdir -p $(@D)
	$(CXX) -std=c++17 $(CXXFLAGS) -c -o $@ $<

%.d: ;


libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;
	
libcompiler_frontend:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/compiler_frontend; fi;

libfrontend:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/frontend; fi;

clean:
	rm -f $(OBJECT_DIRECTORY)/*.o $(OBJECT_DIRECTORY)/*.d $(TEST)
