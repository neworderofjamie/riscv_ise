# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS		+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX)

# Determine full path to test
LIF		:=$(BIN_DIRECTORY)/lif$(PREFIX)

.PHONY: all clean libcommon libise

all: $(LIF)

-include lif.d

$(LIF): lif.cc lif.d libcommon libise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) lif.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(LIF)
