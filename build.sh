#!/bin/bash
set -e

# Build with AFL++ instrumentation
export CC=afl-clang-fast
export CXX=afl-clang-fast
export AFL_USE_ASAN=1
export AFL_USE_UBSAN=1
export ASAN_OPTIONS=detect_leaks=1:abort_on_error=1
export UBSAN_OPTIONS=halt_on_error=1
export AFL_FUZZING=1

# ./autogen.sh
# ./configure
# make clean

# Compile the fuzzer with AFL++'s driver
$CC -g -O2 -fsanitize=address,undefined \
    -I. -Isrc -I/AFLplusplus/include \
    -DBUILTIN -DDATADIR=\".\" \
    -x c \
    /AFLplusplus/utils/aflpp_driver/aflpp_driver.c \
    src/fuzz_advent.c src/database.c src/english.c src/itverb.c src/turn.c src/verb.c \
    -o fuzz_advent -lrt
