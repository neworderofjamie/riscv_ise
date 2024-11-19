# FPGA-Enhanced Neural Networks assembly language toolchain
This is a first attempt at a toolchain to program our FeNN processor.

It features:
* An Instruction Set Simulator built on the carcass of [nedoPC-5](https://gitlab.com/nedopc/npc5/-/blob/master/emu-rv32i.c)
* A C++ Just-in-Time assembler built on the carcass of [xbyak_riscv](https://github.com/herumi/xbyak_riscv)
* A very primitive device interface which handles communicating with FeNN via ``/dev/mem``
* A handful of command line tools to help debug issues e.g. a command line disassembler
* Several example applications using the above functionality to e.g. perform Spiking Heidelberg Digits and MNIST classification

The instruction set for the processor is described in ``docs/instruction_set.pdf``.

## Usage
Examples can be build using Visual Studio (probably a minimum of 2019 for sufficient C++17 support) or GCC using GNU make. 
If using Visual Studio, just open the ``riscv_ise.sln`` in the root directory and build projects as required.
If using GNU make run ``make`` in a project directory e.g. ``src/mnist``. You can add ``DEBUG=1`` to generate debug code.
All executables will be built in the ``bin`` directory.
Most examples have a CLI with ``--help`` so, for example you can run the MNIST

## Data generation
Because the data required for some of the examples is rather too large to put on git, some examples come with python scripts to generate data:
* SHD, run ``bin/generate_shd.py`` to download and convert dataset (requires ``tonic`` package installing with pip)
* MNIST, run ``bin/generate_mnist.py`` to download and convert dataset (requires ``mnist`` package installing with pip)

## Creating new applications
There is an empty projec tin ``src/blank``. This is a good starting point for creating your own application you can just start writing instructions within the ``AssemblerUtils::generateStandardKernel`` block.
