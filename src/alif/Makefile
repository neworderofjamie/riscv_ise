# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS		+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test
ALIF		:=$(BIN_DIRECTORY)/lif$(PREFIX)

.PHONY: all clean libcommon libise libassembler

all: $(ALIF)

-include alif.d

$(ALIF): alif.cc alif.d libcommon libise libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) alif.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(ALIF)
