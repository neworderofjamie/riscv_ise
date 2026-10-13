# Include common makefile
include ../../MakefileCommon

# Add compiler and linker flags to link libGeNN and pthreads
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lfenn_ise$(PREFIX) -lcommon$(PREFIX) -lfenn_common$(PREFIX) -lfenn_assembler$(PREFIX)

# Determine full path to test_mnist
BLANK_JUMP	:=$(BIN_DIRECTORY)/blank_jump$(PREFIX)

.PHONY: all clean libcommon libfenn_common libfenn_assembler libfenn_ise

all: $(BLANK_JUMP)

-include blank_jump.d

$(BLANK_JUMP): blank_jump.cc blank_jump.d libcommon libfenn_common libfenn_assembler libfenn_ise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) blank_jump.cc -o $@ $(LDFLAGS)

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
	rm -f $(BLANK_JUMP)
