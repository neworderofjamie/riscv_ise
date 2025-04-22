# Include common makefile
include src/MakefileCommon

.PHONY: all assembler common compiler disassembler ise backend

all: backend

assembler:
	$(MAKE) -C src/assembler

common:
	$(MAKE) -C src/common

compiler: common assembler
	$(MAKE) -C src/compiler

disassembler: common 
	$(MAKE) -C src/disassembler

ise: common
	$(MAKE) -C src/ise

backend: assembler common compiler disassembler ise
	$(MAKE) -C src/backend
