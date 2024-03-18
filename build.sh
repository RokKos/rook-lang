#!/bin/sh

SOURCE_FILE="main.c"
OUTPUT_FILE="mmel"


WARNINGS=" -Wall -Wextra -Wpedantic"
CFLAGS="-std=c99 -O3"

clang $WARNINGS $CFLAGS -o $OUTPUT_FILE $SOURCE_FILE -v 

