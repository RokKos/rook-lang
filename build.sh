#!/bin/sh

SOURCE_FILE="main.c"
OUTPUT_FILE="mmel"


WARNINGS=" -Wall -Wextra -Wpedantic -Werror"
CFLAGS="-std=c99 -O3 -fstack-protector-strong -fsanitize=address -fsanitize=undefined"

clang $WARNINGS $CFLAGS -o $OUTPUT_FILE $SOURCE_FILE -v 

