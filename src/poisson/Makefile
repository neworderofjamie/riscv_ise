# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX)

# Determine full path to test_mnist
POISSON		:=$(BIN_DIRECTORY)/poisson$(PREFIX)

.PHONY: all clean libise

all: $(POISSON)

-include poisson.d

$(POISSON): poisson.cc poisson.d libise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) poisson.cc -o $@ $(LDFLAGS)


libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(POISSON)
