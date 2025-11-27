[![DOI](https://zenodo.org/badge/824061946.svg)](https://doi.org/10.5281/zenodo.17739997)
# FPGA-Enhanced Neural Networks toolchain
This is a first attempt at a toolchain to program our FeNN processor. It currently supports 

It features:
* An Instruction Set Simulator built on the carcass of [nedoPC-5](https://gitlab.com/nedopc/npc5/-/blob/master/emu-rv32i.c)
* A C++ Just-in-Time assembler built on the carcass of [xbyak_riscv](https://github.com/herumi/xbyak_riscv)
* A device interface which communicates with FeNN using [UIO](https://www.kernel.org/doc/html/v5.0/driver-api/uio-howto.html)
* A Python interface for define models and interacting with both real FeNN hardware and the Instruction Set Simulator

The instruction set for the processor is described in ``docs/instruction_set.pdf``.

## Pre-installation
Currently, the PyFeNN toolchain needs to be built from source. To prepare for this you need to do the following:

1.  Install a C++ compiler on the machine, if not already present.
    For Windows, Visual Studio 2019 or above is required. The Microsoft Visual Studio 
    Community Edition can be downloaded from
    https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx.
    When installing Visual Studio, one should select the 'Desktop 
    development with C++' configuration. On Linux, the GNU Compiler 
    Collection (GCC) 7.5 or above is required. This can be obtained from your
    Linux distribution repository, for example on Ubuntu by running ``sudo apt-get install g++``, 
    or alternatively from https://gcc.gnu.org/index.html.
2. On Linux, install the development version of libffi. For example, on Ubuntu you can do this by running ``sudo apt-get install libffi-dev``.

## Installation
The following installation instructions are required both to use the Instruction Set Simulator on a desktop computer and to use PyFeNN on a Kria KV260 running the [AMD Ubuntu image](https://ubuntu.com/download/amd#kria-k26):

1. Clone the repository, ensuring the submodules are fetched e.g. with ``git clone https://github.com/neworderofjamie/riscv_ise.git --recurse-submodules``
2. Build PyFeNN using ``pip install -e .``. If you wish to also install the additional dependencies required to run the examples, you can do this automatically with
``pip install -e .[examples]``.

## Kria KV260 installation 
To run examples on a Kria KV260, some additional steps are required:

1. Build the udmabuf kernel module by navigating into the ``udmabuf`` folder and running ``make`` 
2. Load the udmabuf kernel module with ``sudo insmod u-dma-buf.ko udmabuf0=8000000`` where 8000000 specifies an 128MByte buffer (in hexadecimal). It is possible to create larger buffers but, it tends to require killing all running applications to get enough memory. 
3. Download the bitstream associated with a software release and unzip into ``/lib/firmware/xilinx``
4. Install the UDEV rules which allow FeNN to be used without sudo access by copying ``tools/90-fenn-uio.rules`` into ``/etc/udev/rules.d/``.
5. Unload the default bitstream with ``sudo dfx-mgr -remove 0`` and load the newly downloaded one with ``sudo dfx-mgr -load fenn_single_core``

Steps 2 and 5 need re-running if you reboot your Kria!

# Tutorials
Some basic tutorials are available to use on Google Collab:
1. [Simulate a population of ALIF neurons](https://colab.research.google.com/github/neworderofjamie/riscv_ise/blob/master/tutorials/alif.ipynb)
2. [Generate poisson noise using FeNN's hardware RNG](https://colab.research.google.com/github/neworderofjamie/riscv_ise/blob/master/tutorials/poisson.ipynb)
3. [Classify MNIST](https://colab.research.google.com/github/neworderofjamie/riscv_ise/blob/master/tutorials/mnist_classifier.ipynb)
  
# Examples
The ``examples`` folder contains several examples, implemented in PyFeNN, including those presented in our paper(s). These all default to running on the simulator but, if you have installed the software on a Kria KV260 as described above, you can run them using the ``--device`` command line argument e.g. with ``python shd_lif_classifier.py --device``.
