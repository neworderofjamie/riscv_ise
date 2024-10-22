# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lcommon$(PREFIX)

# Determine full path to test_mnist
DISASSEMBLER	:=$(BIN_DIRECTORY)/disassembler$(PREFIX)

.PHONY: all clean libcommon

all: $(DISASSEMBLER)

-include disassembler.d

$(DISASSEMBLER): disassembler.cc disassembler.d libcommon
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) disassembler.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(DISASSEMBLER)
