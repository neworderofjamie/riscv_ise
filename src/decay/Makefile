# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test_mnist
DECAY		:=$(BIN_DIRECTORY)/decay$(PREFIX)

.PHONY: all clean libise libassembler

all: $(DECAY)

-include decay.d

$(DECAY): decay.cc decay.d libise libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) decay.cc -o $@ $(LDFLAGS)

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(DECAY)
