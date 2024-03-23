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
  const char *const file_name;
  CompilerMemory compiler_memory;
  u64 file_buffer_lenght;
} lexer_input;

typedef struct {
  u64 lexemes_count;
} lexer_output;

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
  TOKEN_TYPE_COLON,

  // One or two character tokens.
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
  TOKEN_TYPE_NOT,
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

#include "log.h"
#include <stdio.h>
#include <string.h>

internal void lexer_add_token(TokenType token_type, lexeme **lexer_memory,
                              u64 *const lexeme_count, u64 start, u64 end,
                              u64 line);

internal i32 lexer_is_number(char character);
internal i32 lexer_is_alphabet(char character);
enum { K_KEYWORDS_COUNT = 25 };
internal const char keywords_strings[K_KEYWORDS_COUNT][32];

extern lexer_output lexer_stage(lexer_input input) {
  printf("---STARTING LEXER STAGE---\n\n");

  u64 lexeme_count = 0;
  u64 line = 0;
  lexeme *lexer_memory = (lexeme *)input.compiler_memory.lexer_memory_ptr;

  u64 pos = 0;
  u64 start = 0;
  while (pos < input.file_buffer_lenght) {
    char character = input.file_buffer_ptr[pos];
    start = pos;
    pos += 1;

    switch (character) {
    case ' ':
    case '\r':
    case '\t':
      break;

    case '\n': {
      line += 1;
    } break;
    case '(': {
      lexer_add_token(TOKEN_TYPE_LEFT_PARENTHESIS, &lexer_memory, &lexeme_count,
                      start, pos, line);

    } break;
    case ')': {
      lexer_add_token(TOKEN_TYPE_RIGHT_PARENTHESIS, &lexer_memory,
                      &lexeme_count, start, pos, line);

    } break;
    case '{': {
      lexer_add_token(TOKEN_TYPE_LEFT_BRACE, &lexer_memory, &lexeme_count,
                      start, pos, line);

    } break;
    case '}': {
      lexer_add_token(TOKEN_TYPE_RIGHT_BRACE, &lexer_memory, &lexeme_count,
                      start, pos, line);

    } break;
    case '[': {
      lexer_add_token(TOKEN_TYPE_LEFT_BRACKET, &lexer_memory, &lexeme_count,
                      start, pos, line);

    } break;
    case ']': {
      lexer_add_token(TOKEN_TYPE_RIGHT_BRACKET, &lexer_memory, &lexeme_count,
                      start, pos, line);

    } break;
    case ',': {
      lexer_add_token(TOKEN_TYPE_RIGHT_BRACKET, &lexer_memory, &lexeme_count,
                      start, pos, line);

    } break;
    case '.': {
      lexer_add_token(TOKEN_TYPE_DOT, &lexer_memory, &lexeme_count, start, pos,
                      line);

    } break;
    case '+': {
      lexer_add_token(TOKEN_TYPE_PLUS, &lexer_memory, &lexeme_count, start, pos,
                      line);

    } break;
    case '/': {
      lexer_add_token(TOKEN_TYPE_SLASH, &lexer_memory, &lexeme_count, start,
                      pos, line);

    } break;
    case '*': {
      lexer_add_token(TOKEN_TYPE_STAR, &lexer_memory, &lexeme_count, start, pos,
                      line);

    } break;
    case '#': {
      char next_character = input.file_buffer_ptr[pos];
      while (!(next_character == '#' || next_character == '\n') &&
             pos < input.file_buffer_lenght) {
        next_character = input.file_buffer_ptr[pos];
        pos += 1;
      }
      if (next_character == '\n') {
        line += 1;
      }

    } break;
    case '%': {
      lexer_add_token(TOKEN_TYPE_MODULO, &lexer_memory, &lexeme_count, start,
                      pos, line);

    } break;
    case ':': {
      lexer_add_token(TOKEN_TYPE_COLON, &lexer_memory, &lexeme_count, start,
                      pos, line);

    } break;
    case '-': {
      char next_character = input.file_buffer_ptr[pos];
      if (next_character == '>') {
        pos += 1;
        lexer_add_token(TOKEN_TYPE_ARROW, &lexer_memory, &lexeme_count, start,
                        pos, line);
      } else {

        lexer_add_token(TOKEN_TYPE_MINUS, &lexer_memory, &lexeme_count, start,
                        pos, line);
      }

    } break;
    case '=': {
      lexer_add_token(TOKEN_TYPE_EQUALITY, &lexer_memory, &lexeme_count, start,
                      pos, line);
    } break;
    case '<': {
      char next_character = input.file_buffer_ptr[pos];
      if (next_character == '-') {
        pos += 1;
        lexer_add_token(TOKEN_TYPE_ASSIGN, &lexer_memory, &lexeme_count, start,
                        pos, line);
      } else if (next_character == '=') {
        pos += 1;
        lexer_add_token(TOKEN_TYPE_LESS_EQUAL, &lexer_memory, &lexeme_count,
                        start, pos, line);
      } else {
        lexer_add_token(TOKEN_TYPE_LESS, &lexer_memory, &lexeme_count, start,
                        pos, line);
      }

    } break;
    case '>': {
      char next_character = input.file_buffer_ptr[pos];
      if (next_character == '=') {
        pos += 1;
        lexer_add_token(TOKEN_TYPE_GREATER_EQUAL, &lexer_memory, &lexeme_count,
                        start, pos, line);
      } else {
        lexer_add_token(TOKEN_TYPE_GREATER, &lexer_memory, &lexeme_count, start,
                        pos, line);
      }

    } break;
    case '"': {
      char next_character = input.file_buffer_ptr[pos];
      while (!(next_character == '"') && !(pos >= input.file_buffer_lenght)) {
        next_character = input.file_buffer_ptr[pos];
        pos += 1;
        if (next_character == '\n') {
          line += 1;
        }
      }
      if (pos == input.file_buffer_lenght) {
        log_error("LEXER ERROR: Unterminated string.", input.file_name, line);
      }

      lexer_add_token(TOKEN_TYPE_STRING, &lexer_memory, &lexeme_count, start,
                      pos, line);
    } break;
    default: {

      if (lexer_is_number(character)) {
        char next_character = input.file_buffer_ptr[pos];
        while ((lexer_is_number(next_character) || next_character == '_') &&
               pos < input.file_buffer_lenght) {
          pos += 1;

          next_character = input.file_buffer_ptr[pos];
        }

        if (next_character == '.') {

          while ((lexer_is_number(next_character) || next_character == '_') &&
                 pos < input.file_buffer_lenght) {
            pos += 1;

            next_character = input.file_buffer_ptr[pos];
          }

          lexer_add_token(TOKEN_TYPE_FLOATS, &lexer_memory, &lexeme_count,
                          start, pos, line);

        } else {
          lexer_add_token(TOKEN_TYPE_INTEGERS, &lexer_memory, &lexeme_count,
                          start, pos, line);
        }
      } else if (lexer_is_alphabet(character)) {
        char next_character = input.file_buffer_ptr[pos];
        while ((lexer_is_alphabet(next_character) ||
                lexer_is_number(next_character)) &&
               pos < input.file_buffer_lenght) {
          pos += 1;

          next_character = input.file_buffer_ptr[pos];
        }

        i32 length = pos - start;
        char identifier[length + 1];
        strncpy(identifier, input.file_buffer_ptr + start, length);
        identifier[length] = '\0';

        i32 found_keyword = 0;
        for (i32 i = 0; i < 24; i += 1) {
          if (strcmp(identifier, keywords_strings[i]) == 0) {

            lexer_add_token(
                (TokenType)(TOKEN_TYPE_COUNT - K_KEYWORDS_COUNT + i),
                &lexer_memory, &lexeme_count, start, pos, line);
            found_keyword = 1;
            break;
          }
        }
        if (found_keyword == 1) {

        } else {
          lexer_add_token(TOKEN_TYPE_IDENTIFIER, &lexer_memory, &lexeme_count,
                          start, pos, line);
        }
      }
    }
    }
  }

  lexer_add_token(TOKEN_TYPE_EOF, &lexer_memory, &lexeme_count, start, pos,
                  line);

  return (lexer_output){.lexemes_count = lexeme_count};
}

internal const char keywords_strings[K_KEYWORDS_COUNT][32] = {
    // clang-format off
    "not",
    "and",
    "or",
    "xor",
    "bit_and",
    "bit_or",
    "bit_xor",
    "fn",
    "defer",
    "return",
    "if",
    "else",
    "while",
    "switch",
    "partial_switch",

    "i32",
    "u32",
    "i64",
    "u64",
    "enum",
    "struct",
    "mem_of",
    "mut",
    "internal",
    "local_persist",
    // clang-format on
};

internal const char token_type_strings[TOKEN_TYPE_COUNT][32] = {
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
    "COLON",

    // One or two character tokens.
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
    "NOT",
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

// NOTE:(Rok Kos): We need to pass lexer_memory by pointer to pointer, because C
// passes everything by value so if we would do only lexeme *lexer_memory it
// would create a copy of a pointer and it wouldn't increment the address of a
// pointer
internal void lexer_add_token(TokenType token_type, lexeme **lexer_memory,
                              u64 *const lexeme_count, u64 start, u64 end,
                              u64 line) {
  lexeme *lexeme = *lexer_memory;
  lexeme->type = token_type;
  lexeme->start = start;
  lexeme->end = end;
  lexeme->line = line;

  *lexer_memory = *lexer_memory + 1;
  *lexeme_count = *lexeme_count + 1;
}

internal i32 lexer_is_number(char character) {
  return character >= '0' && character <= '9';
}

// TODO: (Rok Kos): Think if you want to allow any other special characters here
internal i32 lexer_is_alphabet(char character) {
  return (character >= 'a' && character <= 'z') ||
         (character >= 'a' && character <= 'Z') || (character == '_');
}

extern const char *lexer_token_type_string(TokenType token_type) {
  assert(token_type < TOKEN_TYPE_COUNT);
  assert(token_type_strings[token_type] != 0);

  return token_type_strings[token_type];
}

#ifdef LEXER_TEST
#endif // LEXER_TEST
#endif // LEXER_IMPLEMENTATION
#endif // INCLUDE_LEXER_H
