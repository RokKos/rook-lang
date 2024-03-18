#ifndef INCLUDE_LEXER_H
#define INCLUDE_LEXER_H

#include "defines.h"
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const char *const file_buffer_ptr;
  CompilerMemory compiler_memory;
  i64 file_buffer_lenght;
} lexer_input;

typedef enum {
  // Single-character tokens.
  TOKEN_TYPE_LEFT_PARENTHESIS,
  TOKEN_TYPE_RIGHT_PARENTHESIS,
  TOKEN_TYPE_LEFT_BRACE,
  TOKEN_TYPE_RIGHT_BRACE,
  TOKEN_TYPE_COMMA,
  TOKEN_TYPE_DOT,
  TOKEN_TYPE_MINUS,
  TOKEN_TYPE_PLUS,
  TOKEN_TYPE_SLASH,
  TOKEN_TYPE_STAR,
  TOKEN_TYPE_HASH,
  TOKEN_TYPE_MODULO,

  // One or two character tokens.
  TOKEN_TYPE_NOT,
  TOKEN_TYPE_NOT_EQUAL,
  TOKEN_TYPE_ASSIGN,
  TOKEN_TYPE_EQUALITY,
  TOKEN_TYPE_GREATER,
  TOKEN_TYPE_GREATER_EQUAL,
  TOKEN_TYPE_LESS,
  TOKEN_TYPE_LESS_EQUAL,
  TOKEN_TYPE_ARROW,

  // Literals.
  TOKEN_TYPE_IDENTIFIER,
  TOKEN_TYPE_STRING,
  TOKEN_TYPE_INTEGERS,
  TOKEN_TYPE_FLOATS,

  // Keywords.
  TOKEN_TYPE_AND,
  TOKEN_TYPE_OR,
  TOKEN_TYPE_XOR,
  TOKEN_TYPE_BIT_AND,
  TOKEN_TYPE_BIT_OR,
  TOKEN_TYPE_BIT_XOR,
  TOKEN_TYPE_FN,
  TOKEN_TYPE_DEFER,
  TOKEN_TYPE_RETURN,
  TOKEN_TYPE_IF,
  TOKEN_TYPE_ELSE,
  TOKEN_TYPE_WHILE,
  TOKEN_TYPE_SWITCH,
  TOKEN_TYPE_PARTIAL_SWITCH,

  TOKEN_TYPE_I32,
  TOKEN_TYPE_U32,
  TOKEN_TYPE_I64,
  TOKEN_TYPE_U64,
  TOKEN_TYPE_ENUM,
  TOKEN_TYPE_STRUCT,
  TOKEN_TYPE_MEM_OF,
  TOKEN_TYPE_MUT,
  TOKEN_TYPE_INTERNAL,
  TOKEN_TYPE_LOCAL_PERSIST,

  TOKEN_TYPE_EOF
} TokenType;

typedef struct {
  TokenType type;
  i64 start;
  i64 end;
  i64 line;
} lexeme;

typedef struct {
  u64 lexemes_count;
} lexer_output;

extern lexer_output lexer_stage(lexer_input input);

#ifdef __cplusplus
}
#endif

#define LEXER_IMPLEMENTATION
#ifdef LEXER_IMPLEMENTATION

#include <stdio.h>

extern lexer_output lexer_stage(lexer_input input) {
  printf("---STARTING LEXER STAGE---\n\n");

  u64 lexeme_count = 0;
  lexeme *lexer_memory = (lexeme *)input.compiler_memory.lexer_memory_ptr;
  for (i64 pos = 0; pos < input.file_buffer_lenght; pos += 1) {
    char character = input.file_buffer_ptr[pos];
    switch (character) {
    case '(': {

      lexer_memory->type = TOKEN_TYPE_LEFT_PARENTHESIS;
      lexer_memory->start = pos;
      lexer_memory->end = pos + 1;
      lexer_memory->line = 0;

      // printf("type: %d, start: %lld, end: %lld, line: %lld, memory: %lld "
      //        "sizeof: %lld\n",
      //        lexer_memory->type, lexer_memory->start, lexer_memory->end,
      //        lexer_memory->line, lexer_memory, sizeof(lexeme));
      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    }
  }

  return (lexer_output){.lexemes_count = lexeme_count};
}

#ifdef LEXER_TEST
#endif // LEXER_TEST
#endif // LEXER_IMPLEMENTATION
#endif // INCLUDE_LEXER_H
