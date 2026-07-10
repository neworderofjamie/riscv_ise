# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS		+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX) -ldisassembler$(PREFIX)

# Determine full path to test
LIF_WITH_FUSI		:=$(BIN_DIRECTORY)/lif_with_fusi_stdp$(PREFIX)

.PHONY: all clean libcommon libise libassembler libdisassembler

all: $(LIF_WITH_FUSI)

-include lif_with_fusi_stdp.d

$(LIF_WITH_FUSI): lif_with_fusi_stdp.cc lif_with_fusi_stdp.d libcommon libise libassembler libdisassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) lif_with_fusi_stdp.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libdisassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/disassembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(LIF_WITH_FUSI)
