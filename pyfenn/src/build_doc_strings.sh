#! /bin/bash
python mkdoc.py -o docStrings.h -std=c++17 -I ../../include/ -I ../../genn/include/genn/genn -I ../../genn/include/genn/third_party ../../include/backend/*.h ../../include/compiler/*.h ../../genn/include/genn/genn/type.h

# Note that some mac users might require
# python mkdoc.py -o docStrings.h -std=c++17 -I /usr/local/opt/libffi/include -I ../../include/ -I ../../genn/include/genn/genn -I ../../genn/include/genn/third_party ../../include/backend/*.h ../../include/compiler/*.h ../../genn/include/genn/genn/type.h
