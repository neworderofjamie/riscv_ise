# Include common makefile
include src/MakefileCommon

.PHONY: all assembler common compiler ise backend

all: backend

assembler:
	$(MAKE) -C src/assembler

common:
	$(MAKE) -C src/common

compiler: common assembler
	$(MAKE) -C src/compiler

ise: common
	$(MAKE) -C src/ise

backend: assembler common compiler ise
	$(MAKE) -C src/backend
