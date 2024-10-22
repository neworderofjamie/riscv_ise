# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS		+= -L$(LIBRARY_DIRECTORY) -lcommon$(PREFIX)

# Determine full path to test
DMA_TEST	:=$(BIN_DIRECTORY)/dma_test$(PREFIX)

.PHONY: all clean libcommon

all: $(DMA_TEST)

-include dma_test.d

$(DMA_TEST): dma_test.cc dma_test.d libcommon
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) dma_test.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(DMA_TEST)
