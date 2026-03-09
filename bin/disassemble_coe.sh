#!/bin/bash

ADDRESS=0
tail -n +3 $1 | while read LINE
do
    LINE_NO_COMMA="${LINE%,}"
    LINE_NO_SUFFIX="${LINE_NO_COMMA%;}"
    printf "%x " $ADDRESS
    ./disassembler_cli 0x$LINE_NO_SUFFIX
    ADDRESS=$[ADDRESS + 4]
   # do something with $line here
done
