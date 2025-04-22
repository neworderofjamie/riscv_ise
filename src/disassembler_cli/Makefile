# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -ldisassembler$(PREFIX) -lcommon$(PREFIX)

# Determine full path to test
DISASSEMBLER_CLI	:=$(BIN_DIRECTORY)/disassembler_cli$(PREFIX)

.PHONY: all clean libcommon libdisassembler

all: $(DISASSEMBLER_CLI)

-include disassembler_cli.d

$(DISASSEMBLER_CLI): disassembler_cli.cc libcommon libdisassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) disassembler_cli.cc -o $@ $(LDFLAGS)

libdisassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/disassembler; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(DISASSEMBLER_CLI)
