# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
CXXFLAGS 		+=-I$(GENN_PATH)/include/genn/genn -I$(GENN_PATH)/include/genn/third_party $(shell pkg-config libffi --cflags)
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -L$(GENN_PATH)/lib -lbackend$(PREFIX) -lcompiler$(PREFIX) -lise$(PREFIX) -lcommon$(PREFIX) -ldisassembler$(PREFIX) -lassembler$(PREFIX) -lgenn$(PREFIX) -ldl $(shell pkg-config libffi --libs)

# Determine full path to test_mnist
COMPILER_TEST	:=$(BIN_DIRECTORY)/compiler_test$(PREFIX)

.PHONY: all clean libise libbackend libcommon libassembler libdisassembler libcompiler

all: $(COMPILER_TEST)

-include compiler_test.d

$(COMPILER_TEST): compiler_test.cc compiler_test.d libise libcommon libbackend libassembler libdisassembler libcompiler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) compiler_test.cc -o $@ $(LDFLAGS)

libbackend:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/backend; fi;

libcompiler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/compiler; fi;

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libdisassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/disassembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;


%.d: ;

clean:
	rm -f $(MNIST)
