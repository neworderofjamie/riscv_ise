# Include common makefile
include ../../MakefileCommon

# Add compiler and linker flags to link libGeNN and pthreads
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lfenn_backend$(PREFIX) -lfenn_ise$(PREFIX) -lfrontend$(PREFIX) -lcommon$(PREFIX) -lcompiler_frontend$(PREFIX) -lfenn_common$(PREFIX) -lfenn_assembler$(PREFIX) -lfenn_compiler$(PREFIX)  -lfenn_disassembler$(PREFIX) -lpthread -lffi

# Determine full path to test_mnist
COMPILER_TEST	:=$(BIN_DIRECTORY)/compiler_test$(PREFIX)

.PHONY: all clean libcommon libcompiler_frontend libfrontend libfenn_common libfenn_assembler libfenn_backend libfenn_compiler libfenn_disassembler libfenn_ise

all: $(COMPILER_TEST)

-include compiler_test.d

$(COMPILER_TEST): compiler_test.cc compiler_test.d libcommon libcompiler_frontend libfrontend libfenn_common libfenn_assembler libfenn_backend libfenn_compiler libfenn_disassembler libfenn_ise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) compiler_test.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;
	
libcompiler_frontend:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/compiler_frontend; fi;

libfrontend:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/frontend; fi;

libfenn_common:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/common; fi;

libfenn_assembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/assembler; fi;

libfenn_backend:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/backend; fi;

libfenn_compiler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/compiler; fi;

libfenn_disassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/disassembler; fi;

libfenn_ise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/ise; fi;

%.d: ;

clean:
	rm -f $(MNIST)
