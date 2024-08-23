# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX)

# Determine full path to test_mnist
MNIST		:=$(BIN_DIRECTORY)/mnist$(PREFIX)

.PHONY: all clean libise libcommon

all: $(MNIST)

-include mnist.d

$(MNIST): mnist.cc mnist.d libise libcommon
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) mnist.cc -o $@ $(LDFLAGS)

mnist.cc: ;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

%.d: ;

clean:
	rm -f $(MNIST)
