#ifndef INCLUDE_LEXER_H
#define INCLUDE_LEXER_H

#include "defines.h"
#include "memory.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const char *const file_buffer_ptr;
  CompilerMemory compiler_memory;
  u64 file_buffer_lenght;
} lexer_input;

typedef struct {
  u64 lexemes_count;
} lexer_output;

struct lexer_output;
typedef struct lexer_output lexer_output;

// MAIN ENTRY FOR THIS STAGE
extern lexer_output lexer_stage(lexer_input input);

typedef enum __attribute__((__packed__)) {
  // Single-character tokens.
  TOKEN_TYPE_LEFT_PARENTHESIS = 0,
  TOKEN_TYPE_RIGHT_PARENTHESIS,
  TOKEN_TYPE_LEFT_BRACE,
  TOKEN_TYPE_RIGHT_BRACE,
  TOKEN_TYPE_LEFT_BRACKET,
  TOKEN_TYPE_RIGHT_BRACKET,
  TOKEN_TYPE_COMMA,
  TOKEN_TYPE_DOT,
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
  TOKEN_TYPE_MINUS,

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

  TOKEN_TYPE_EOF,
  TOKEN_TYPE_COUNT
} TokenType;

COMPILE_TIME_ASSERT(sizeof(TokenType) == sizeof(u8));

extern const char *lexer_token_type_string(TokenType token_type);

typedef struct {
  TokenType type;
  u64 start;
  u64 end;
  u64 line;
} lexeme;

#ifdef __cplusplus
}
#endif

#define LEXER_IMPLEMENTATION
#ifdef LEXER_IMPLEMENTATION

#include <stdio.h>

extern lexer_output lexer_stage(lexer_input input) {
  printf("---STARTING LEXER STAGE---\n\n");

  u64 lexeme_count = 0;
  u64 line = 0;
  lexeme *lexer_memory = (lexeme *)input.compiler_memory.lexer_memory_ptr;

  u64 pos = 0;
  while (pos < input.file_buffer_lenght) {
    char character = input.file_buffer_ptr[pos];
    u64 start = pos;
    pos += 1;

    switch (character) {
    case '\n': {
      line += 1;
    } break;
    case '(': {
      lexer_memory->type = TOKEN_TYPE_LEFT_PARENTHESIS;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case ')': {
      lexer_memory->type = TOKEN_TYPE_RIGHT_PARENTHESIS;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '{': {
      lexer_memory->type = TOKEN_TYPE_LEFT_BRACE;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '}': {
      lexer_memory->type = TOKEN_TYPE_RIGHT_BRACE;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '[': {
      lexer_memory->type = TOKEN_TYPE_LEFT_BRACKET;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case ']': {
      lexer_memory->type = TOKEN_TYPE_RIGHT_BRACKET;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case ',': {
      lexer_memory->type = TOKEN_TYPE_COMMA;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '.': {
      lexer_memory->type = TOKEN_TYPE_DOT;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '+': {
      lexer_memory->type = TOKEN_TYPE_PLUS;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '/': {
      lexer_memory->type = TOKEN_TYPE_SLASH;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '*': {
      lexer_memory->type = TOKEN_TYPE_STAR;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '#': {
      char next_character = input.file_buffer_ptr[pos];
      while (!(next_character == '#' || next_character == '\n')) {
        next_character = input.file_buffer_ptr[pos];
        pos += 1;
      }
      if (next_character == '\n') {
        line += 1;
      }

    } break;
    case '%': {
      lexer_memory->type = TOKEN_TYPE_MODULO;
      lexer_memory->start = start;
      lexer_memory->end = pos;
      lexer_memory->line = line;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    case '-': {
      char next_character = input.file_buffer_ptr[pos];
      if (next_character == '>') {
        lexer_memory->type = TOKEN_TYPE_ARROW;
        pos += 1;
      } else {
        lexer_memory->type = TOKEN_TYPE_MINUS;
      }

      lexer_memory->line = line;
      lexer_memory->start = start;
      lexer_memory->end = pos;

      lexer_memory += 1;
      lexeme_count += 1;

    } break;
    }
  }

  return (lexer_output){.lexemes_count = lexeme_count};
}

internal const char *token_type_strings[TOKEN_TYPE_COUNT] = {
    // clang-format off
  
    // Single-character tokens.
    "LEFT_PARENTHESIS",
    "RIGHT_PARENTHESIS",
    "LEFT_BRACE",
    "RIGHT_BRACE",
    "LEFT_BRACKET",
    "RIGHT_BRACKET",
    "COMMA",
    "DOT",
    "PLUS",
    "SLASH",
    "STAR",
    "HASH",
    "MODULO",

    // One or two character tokens.
    "NOT",
    "NOT_EQUAL",
    "ASSIGN",
    "EQUALITY",
    "GREATER",
    "GREATER_EQUAL",
    "LESS",
    "LESS_EQUAL",
    "ARROW",
    "MINUS",

    // Literals.
    "IDENTIFIER",
    "STRING",
    "INTEGERS",
    "FLOATS",

    // Keywords.
    "AND",
    "OR",
    "XOR",
    "BIT_AND",
    "BIT_OR",
    "BIT_XOR",
    "FN",
    "DEFER",
    "RETURN",
    "IF",
    "ELSE",
    "WHILE",
    "SWITCH",
    "PARTIAL_SWITCH",

    "I32",
    "U32",
    "I64",
    "U64",
    "ENUM",
    "STRUCT",
    "MEM_OF",
    "MUT",
    "INTERNAL",
    "LOCAL_PERSIST",

    "EOF"

// clang-format on 
};
extern const char *lexer_token_type_string(TokenType token_type) {
  assert(token_type < TOKEN_TYPE_COUNT);

  return token_type_strings[token_type];

}

#ifdef LEXER_TEST
#endif // LEXER_TEST
#endif // LEXER_IMPLEMENTATION
#endif // INCLUDE_LEXER_H
