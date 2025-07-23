# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test_mnist
SPIKE_DMA :=$(BIN_DIRECTORY)/spike_dma$(PREFIX)

.PHONY: all clean libise libcommon libassembler

all: $(SPIKE_DMA)

-include spike_dma.d

$(SPIKE_DMA): spike_dma.cc spike_dma.d libise libcommon libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) spike_dma.cc -o $@ $(LDFLAGS)

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(SPIKE_DMA)
