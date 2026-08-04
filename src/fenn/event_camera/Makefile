# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS		+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX) -lcommon$(PREFIX) -lassembler$(PREFIX) -pthread

# Determine full path to test
EVENT_CAMERA		:=$(BIN_DIRECTORY)/event_camera$(PREFIX)

.PHONY: all clean libcommon libise libassembler

all: $(EVENT_CAMERA)

-include event_camera.d

$(EVENT_CAMERA): event_camera.cc event_camera.d libcommon libise libassembler
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) event_camera.cc -o $@ $(LDFLAGS)

libcommon:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/common; fi;

libassembler:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/assembler; fi;

libise:
	if [ -w $(LIBRARY_DIRECTORY) ]; then $(MAKE) -C $(RISCV_DIR)/src/ise; fi;

%.d: ;

clean:
	rm -f $(EVENT_CAMERA)
