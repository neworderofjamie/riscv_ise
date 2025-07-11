# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS		+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test
DMA_TEST_FENN		:=$(BIN_DIRECTORY)/dma_test_fenn$(PREFIX)

.PHONY: all clean libcommon libise libassembler

all: $(DMA_TEST_FENN)

-include dma_test_fenn.d

$(DMA_TEST_FENN): dma_test_fenn.cc dma_test_fenn.d libcommon libise libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) dma_test_fenn.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(DMA_TEST_FENN)
