#! /bin/bash
python mkdoc.py -o frontend_doc_strings.h -std=c++17 -I ../../include/ -I ../../include/third_party ../../include/common/*.h ../../include/compiler_frontend/*.h ../../include/frontend/*.h
python mkdoc.py -o fenn_backend_doc_strings.h -std=c++17 -I ../../include/ -I ../../include/third_party ../../include/fenn/backend/*.h ../../include/fenn/disassembler/*.h
