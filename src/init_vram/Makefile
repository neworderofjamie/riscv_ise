# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test
INIT_VRAM	:=$(BIN_DIRECTORY)/init_vram$(PREFIX)

.PHONY: all clean libise libcommon libassembler

all: $(INIT_VRAM)

-include init_vram.d

$(INIT_VRAM): init_vram.cc init_vram.d libise libcommon libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) init_vram.cc -o $@ $(LDFLAGS)

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(INIT_VRAM)
