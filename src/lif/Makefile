# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX)

# Determine full path to test
TEST		:=$(BIN_DIRECTORY)/test$(PREFIX)

.PHONY: all clean libise

all: $(TEST)

-include test.d

$(TEST): test.cc test.d libise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) test.cc -o $@ $(LDFLAGS)

test.cc: ;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(TEST)
