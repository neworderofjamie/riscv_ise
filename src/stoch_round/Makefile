# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lcommon$(PREFIX) -lise$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test_mnist
STOCH_ROUND		:=$(BIN_DIRECTORY)/stoch_round$(PREFIX)

.PHONY: all clean libcommon libise libassembler

all: $(STOCH_ROUND)

-include stoch_round.d

$(STOCH_ROUND): stoch_round.cc stoch_round.d libcommon libise libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) stoch_round.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(STOCH_ROUND)
