# Include common makefile
include ../../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lfenn_disassembler$(PREFIX) -lfenn_common$(PREFIX)

# Determine full path to test
DISASSEMBLER_CLI	:=$(BIN_DIRECTORY)/disassembler_cli$(PREFIX)

.PHONY: all clean libfenn_common libfenn_disassembler

all: $(DISASSEMBLER_CLI)

-include disassembler_cli.d

$(DISASSEMBLER_CLI): disassembler_cli.cc libfenn_common libfenn_disassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) disassembler_cli.cc -o $@ $(LDFLAGS)

libfenn_disassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/disassembler; fi;

libfenn_common:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/fenn/common; fi;

%.d: ;

clean:
	rm -f $(DISASSEMBLER_CLI)
