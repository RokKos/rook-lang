
#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

#include "defines.h"

u64 KILOBYTES(u64 value) { return value * 1024; }
u64 MEGABYTES(u64 value) { return value * 1024 * 1024; }
u64 GIGABYTES(u64 value) { return value * 1024 * 1024 * 1024; }

typedef struct {
  u64 lexer_memory_size;
  void *lexer_memory_ptr;

} CompilerMemory;

#endif
