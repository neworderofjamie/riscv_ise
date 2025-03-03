# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX)

# Determine full path to test_mnist
VA_BENCHMARK		:=$(BIN_DIRECTORY)/va_benchmark$(PREFIX)

.PHONY: all clean libise libcommon libassembler

all: $(VA_BENCHMARK)

-include va_benchmark.d

$(VA_BENCHMARK): va_benchmark.cc va_benchmark.d libise libcommon libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) va_benchmark.cc -o $@ $(LDFLAGS)

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(VA_BENCHMARK)
