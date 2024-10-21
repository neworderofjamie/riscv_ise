# Get directory of this makefile (means make can be invoked else where)
ARCH_TEST_CONVERTER_DIR:=$(dir $(lastword $(MAKEFILE_LIST)))
RISCV_DIR:=$(abspath $(ARCH_TEST_CONVERTER_DIR)../../)

# Directories of different ISAs
TEST_SUITE_DIR?=$(HOME)/riscv-arch-test/riscv-test-suite/rv32e_m/

TEST_LIBRARY_INCLUDE_DIR=$(RISCV_DIR)/include/riscv_arch_test
TEST_LIBRARY_SRC_DIR=$(RISCV_DIR)/src/riscv_arch_test
DEVICE_TESTS_DIR=$(RISCV_DIR)/tests/device
SIMULATOR_TESTS_DIR=$(RISCV_DIR)/tests/simulator

# Find source files
E_TESTS:=add-01 andi-01 bgeu-01 bne-01 slli-01 sltiu-01 srai-01 sub-01 addi-01 beq-01 blt-01 lui-01 or-01 slt-01 sltu-01 srl-01 xor-01 and-01 bge-01 bltu-01 lw-align-01 ori-01 sll-01 slti-01 sra-01 srli-01 xori-01
B_TESTS:=clz-01 cpop-01 ctz-01

ASSEMBLER_SOURCES:=$(E_TESTS:%=$(TEST_SUITE_DIR)/E/src/%.S)
ASSEMBLER_SOURCES+=$(B_TESTS:%=$(TEST_SUITE_DIR)/B/src/%.S)

.PHONY: all, test_library, device_google_test, simulator_google_test

all: test_library, device_google_test, simulator_google_test


test_library:
	python $(ARCH_TEST_CONVERTER_DIR)/generate_tests.py -t test_library.cc.jinja -o $(TEST_LIBRARY_SRC_DIR) $(ASSEMBLER_SOURCES)
	python $(ARCH_TEST_CONVERTER_DIR)/generate_tests.py -t test_library.h.jinja -o $(TEST_LIBRARY_INCLUDE_DIR) $(ASSEMBLER_SOURCES)

simulator_google_test: test_library
	python $(ARCH_TEST_CONVERTER_DIR)/generate_tests.py -t simulator_google_test.cc.jinja -o $(SIMULATOR_TESTS_DIR) $(ASSEMBLER_SOURCES)

device_google_test: test_library
	python $(ARCH_TEST_CONVERTER_DIR)/generate_tests.py -t device_google_test.cc.jinja -o $(DEVICE_TESTS_DIR) $(ASSEMBLER_SOURCES)
