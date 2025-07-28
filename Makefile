# Include common makefile
include src/MakefileCommon

.PHONY: all assembler common compiler disassembler ise backend assembly_examples

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

assembly_examples:
	$(MAKE) -C src/alif
	$(MAKE) -C src/dma_test
	$(MAKE) -C src/dma_test_fenn
	$(MAKE) -C src/dma_test_read_csr
	$(MAKE) -C src/lif
	$(MAKE) -C src/mnist
	$(MAKE) -C src/poisson
	$(MAKE) -C src/shd
	$(MAKE) -C src/sparse_spike
	$(MAKE) -C src/spike_dma
	$(MAKE) -C src/special_function_test
	$(MAKE) -C src/spike
	$(MAKE) -C src/dma
	$(MAKE) -C src/va_benchmark
