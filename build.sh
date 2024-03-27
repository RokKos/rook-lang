#!/bin/sh

SOURCE_FILE="main.c"
OUTPUT_FILE="mmel"


WARNINGS=" -Wall -Wextra -Wpedantic -Werror -Wfloat-equal -Wundef -Wint-to-pointer-cast -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wcast-qual -Wmissing-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion --pedantic-errors"
CFLAGS="-std=c99 -O3 -fstack-protector-strong -fsanitize=address -fsanitize=undefined"

clang $WARNINGS $CFLAGS -o $OUTPUT_FILE $SOURCE_FILE -v 

