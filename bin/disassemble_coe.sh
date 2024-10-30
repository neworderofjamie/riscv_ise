#!/bin/bash

tail -n +3 $1 | while read LINE
do
    LINE_NO_COMMA="${LINE%,}"
    LINE_NO_SUFFIX="${LINE_NO_COMMA%;}"
    ./disassembler 0x$LINE_NO_SUFFIX
   # do something with $line here
done
