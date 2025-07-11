# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS		+= -L$(LIBRARY_DIRECTORY) -lcommon$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test
DMA_TEST_READ_CSR	:=$(BIN_DIRECTORY)/dma_test_read_csr$(PREFIX)

.PHONY: all clean libcommon libassembler

all: $(DMA_TEST_READ_CSR)

-include dma_test_read_csr.d

$(DMA_TEST_READ_CSR): dma_test_read_csr.cc dma_test_read_csr.d libcommon
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) dma_test_read_csr.cc -o $@ $(LDFLAGS)

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(DMA_TEST_READ_CSR)
