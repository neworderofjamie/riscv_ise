# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX)

# Determine full path to test
TEST_INIT_VRAM	:=$(BIN_DIRECTORY)/test_init_vram$(PREFIX)

.PHONY: all clean libise

all: $(TEST_INIT_VRAM)

-include test_init_vram.d

$(TEST_INIT_VRAM): test_init_vram.cc test_init_vram.d libise
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) test_init_vram.cc -o $@ $(LDFLAGS)

test_init_vram.cc: ;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(TEST_INIT_VRAM)
