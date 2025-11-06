# FPGA-Enhanced Neural Networks toolchain
This is a first attempt at a toolchain to program our FeNN processor.

It features:
* An Instruction Set Simulator built on the carcass of [nedoPC-5](https://gitlab.com/nedopc/npc5/-/blob/master/emu-rv32i.c)
* A C++ Just-in-Time assembler built on the carcass of [xbyak_riscv](https://github.com/herumi/xbyak_riscv)
* A very primitive device interface which handles communicating with FeNN via ``/dev/mem``
* A Python interface for define models and interacting with both real FeNN hardware and the Instruction Set Simulator

The instruction set for the processor is described in ``docs/instruction_set.pdf``.

## Installation
The following installation instructions are required both to use the Instruction Set Simulator on a desktop computer and to

1. Clone the repository, ensuring the submodules are fetched ``git clone https://github.com/neworderofjamie/riscv_ise.git --recurse-submodules``
2. Install dependencies ``pip install numpy psutil pybind11``
3. Build PyFeNN ``python setup.py develop``

## Device installation 
To run examples on a Kria KV260, some other steps are required:

1. Build the udmabuf driver by navigating into the ``udmabuf``, run ``make`` and then load the module with ``sudo insmod u-dma-buf.ko udmabuf0=8000000``
2. Download the bitstream associated with a software release and unzip into ``/lib/firmware/xilinx``
3. Unload the default bitstream with ``sudo dfx-mgr -remove 0`` and load the newly downloaded one with ``sudo dfx-mgr -load fenn_0.01``

# Tutorials
Some basic tutorials are available to use on Google Collab:
1. [Simulate a population of ALIF neurons](https://colab.research.google.com/github/neworderofjamie/riscv_ise/blob/master/tutorials/alif.ipynb)
2. [Generate poisson noise using FeNN's hardware RNG](https://colab.research.google.com/github/neworderofjamie/riscv_ise/blob/master/tutorials/poisson.ipynb)
3. [Classify MNIST](https://colab.research.google.com/github/neworderofjamie/riscv_ise/blob/master/tutorials/poisson.ipynb)
  
