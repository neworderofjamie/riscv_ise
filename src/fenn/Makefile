FENN_DIR:=$(abspath $(dir $(lastword $(MAKEFILE_LIST))))

.PHONY: all common compiler_frontend frontend fenn_assembler fenn_backend fenn_common fenn_compiler fenn_disassembler fenn_ise

all: common compiler_frontend frontend fenn_assembler fenn_backend fenn_common fenn_compiler fenn_disassembler fenn_ise

common:
	$(MAKE) -C $(FENN_DIR)/../common

compiler_frontend:
	$(MAKE) -C $(FENN_DIR)/../compiler_frontend

frontend:
	$(MAKE) -C $(FENN_DIR)/../frontend

fenn_assembler: common
	$(MAKE) -C $(FENN_DIR)/assembler

fenn_backend: fenn_assembler fenn_common fenn_compiler fenn_ise
	$(MAKE) -C $(FENN_DIR)/backend

fenn_common: common
	$(MAKE) -C $(FENN_DIR)/common

fenn_compiler: common compiler_frontend fenn_common fenn_assembler
	$(MAKE) -C $(FENN_DIR)/compiler

fenn_disassembler: fenn_common
	$(MAKE) -C $(FENN_DIR)/disassembler

fenn_ise: common fenn_common 
	$(MAKE) -C $(FENN_DIR)/ise
