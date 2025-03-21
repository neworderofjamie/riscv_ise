# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test_mnist
LANE_LOCAL_TEST		:=$(BIN_DIRECTORY)/lane_local_test$(PREFIX)

.PHONY: all clean libise libcommon libassembler

all: $(LANE_LOCAL_TEST)

-include lane_local_test.d

$(LANE_LOCAL_TEST): lane_local_test.cc lane_local_test.d libise libcommon libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) lane_local_test.cc -o $@ $(LDFLAGS)

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(LANE_LOCAL_TEST)
