# Include common makefile
include ../MakefileCommon

# Add compiler and linker flags to link libGeNN and correct backend; and to configure generator to use backend
LDFLAGS			+= -L$(LIBRARY_DIRECTORY) -lise$(PREFIX)

# Determine full path to test
DUMP_COE	:=$(BIN_DIRECTORY)/dump_coe$(PREFIX)

.PHONY: all clean

all: $(DUMP_COE)

-include dump_coe.d

$(DUMP_COE): dump_coe.cc dump_coe.d
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) dump_coe.cc -o $@ $(LDFLAGS)

%.d: ;

clean:
	rm -f $(DUMP_COE)
