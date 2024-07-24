# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX)

# Determine full path to test_mnist
TEST_ALIF_POISSON	:=$(BIN_DIRECTORY)/test_alif_poisson$(PREFIX)

.PHONY: all clean libise

all: $(TEST_ALIF_POISSON)

-include test_alif_poisson.d

$(TEST_ALIF_POISSON): test_alif_poisson.cc test_alif_poisson.d libise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) test_alif_poisson.cc -o $@ $(LDFLAGS)


libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(TEST_ALIF_POISSON)
