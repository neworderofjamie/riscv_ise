// Standard C++ includes
#include <iostream>

// Standard C includes
#include <cassert>

// Common include
#include "disassembler/disassembler.h"

int main(int argc, char *argv[])
{
    assert(argc == 2);

    try {
        disassemble(std::cout, std::stoul(argv[1], nullptr, 0));
        std::cout << std::endl;
        return 0;
    }
    catch(const std::exception &ex){
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}