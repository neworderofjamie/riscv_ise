# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lcommon$(PREFIX) -li2c

# Determine full path to test_mnist
GENX320 :=$(BIN_DIRECTORY)/genx320$(PREFIX)

.PHONY: all clean libcommon

all: $(GENX320)

-include genx320.d

$(GENX320): genx320.cc genx320.d libcommon
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) genx320.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(GENX320)
