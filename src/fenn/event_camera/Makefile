# Include common makefile
include ../../MakefileCommon

# Add compiler and linker flags to link libGeNN and pthreads
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lfenn_ise$(PREFIX) -lcommon$(PREFIX) -lfenn_common$(PREFIX) -lfenn_assembler$(PREFIX) -lpthread

# Determine full path to test_mnist
EVENT_CAMERA	:=$(BIN_DIRECTORY)/event_camera$(PREFIX)

.PHONY: all clean libcommon libfenn_common libfenn_assembler libfenn_ise

all: $(EVENT_CAMERA)

-include event_camera.d

$(EVENT_CAMERA): event_camera.cc event_camera.d libcommon libfenn_common libfenn_assembler libfenn_ise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) event_camera.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

libfenn_common:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/common; fi;

libfenn_assembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/assembler; fi;

libfenn_ise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/ise; fi;

%.d: ;

clean:
	rm -f $(COMPILER_TEST)