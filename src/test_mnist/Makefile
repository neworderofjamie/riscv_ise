# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX)

# Determine full path to test_mnist
TEST_MNIST		:=$(BIN_DIRECTORY)/test_mnist$(PREFIX)

.PHONY: all clean libise

all: $(TEST_MNIST)

-include test_mnist.d

$(TEST_MNIST): test_mnist.cc test_mnist.d libise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) test_mnist.cc -o $@ $(LDFLAGS)

test_mnist.cc: ;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f test_mnist.cc $(TEST_MNIST)
