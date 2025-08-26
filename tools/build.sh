#!/bin/bash

g++ -std=c++17 -O0 -g -isystem $GTEST_DIR -isystem $GTEST_DIR/include dma_event_loop_test.cc $GTEST_DIR/src/gtest-all.cc $GTEST_DIR/src/gtest_main.cc -lpthread -o dma_event_loop_test

