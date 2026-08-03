#! /bin/bash
python mkdoc.py -o frontend_doc_strings.h -std=c++17 -I ../../include/ ../../include/common/*.h ../../include/compiler_frontend/*.h ../../include/frontend/*.h
