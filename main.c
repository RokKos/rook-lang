#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sysexits.h>
#include <unistd.h>

#include "defines.h"
#include "memory.h"

#define LOG_IMPLEMENTATION
#include "log.h"
#define LEXER_IMPLEMENTATION
#include "lexer.h"

const char K_COMMAND_RUN[4] = "run\0";
const char K_COMMAND_BUILD[6] = "build\0";
const char K_USAGE_INSTRUCTION[] =
    "Usage mmel <command> <file_path>\nCommands:\nrun - Builds and runs the "
    "program\nbuild - Builds the program\n";

i32 main(i32 argc, char **argv) {

  if (argc != 3) {
    log_error(K_USAGE_INSTRUCTION, __FILE__, __LINE__);

    return EX_USAGE;
  }

  char *program_name = argv[0];
  char *program_command = argv[1];
  char *program_file = argv[2];

  i32 do_build = strcmp(program_command, K_COMMAND_BUILD);
  i32 do_run = strcmp(program_command, K_COMMAND_RUN);
  if (do_build != 0 && do_run != 0) {
    log_error(K_USAGE_INSTRUCTION, __FILE__, __LINE__);

    return EX_USAGE;
  }

  printf("Running %s compiler to %s file %s \n", program_name, program_command,
         program_file);

  i32 file_descriptor = open(program_file, O_RDONLY);
  if (file_descriptor == -1) {
    log_error("Error opening program file", __FILE__, __LINE__);
    return EX_NOINPUT;
  }

  struct stat file_stats;
  if (fstat(file_descriptor, &file_stats) == -1) {
    log_error("Error getting file size", __FILE__, __LINE__);
    close(file_descriptor);
    return EX_DATAERR;
  }

  const i32 offset = 0;
  char *file_in_memory = mmap(NULL, file_stats.st_size, PROT_READ, MAP_PRIVATE,
                              file_descriptor, offset);
  if (file_in_memory == MAP_FAILED) {
    log_error("Error mapping file", __FILE__, __LINE__);
    close(file_descriptor);
    return EX_IOERR;
  }
  for (i64 i = 0; i < file_stats.st_size;) {
    i64 char_length = 1;

    // Determine the length of the UTF-8 character
    if ((file_in_memory[i] & 0x80) == 0x00) {
      // 1-byte character (ASCII)
      char_length = 1;
    } else if ((file_in_memory[i] & 0xE0) == 0xC0) {
      // 2-byte character
      char_length = 2;
    } else if ((file_in_memory[i] & 0xF0) == 0xE0) {
      // 3-byte character
      char_length = 3;
    } else if ((file_in_memory[i] & 0xF8) == 0xF0) {
      // 4-byte character
      char_length = 4;
    }

    fwrite(&file_in_memory[i], 1, char_length, stdout);

    i += char_length;
  }

  printf("\n\n\n");

  CompilerMemory memory;
  memory.lexer_memory_size = KILOBYTES(200);
  memory.lexer_memory_ptr = malloc(memory.lexer_memory_size * sizeof(u8));

  if (memory.lexer_memory_ptr == NULL) {
    log_error("Cannot allocate 200kb for lexer", __FILE__, __LINE__);

    munmap(file_in_memory, file_stats.st_size);
    close(file_descriptor);

    return EX_DATAERR;
  }

  lexer_input lexer_input = {.file_buffer_ptr = file_in_memory,
                             .file_buffer_lenght = file_stats.st_size,
                             .compiler_memory = memory};
  lexer_output lexer_output = lexer_stage(lexer_input);

  lexeme *lexer_memory = (lexeme *)memory.lexer_memory_ptr;
  u64 line = lexer_memory->line;
  for (u64 index = 0; index < lexer_output.lexemes_count; ++index) {
    printf("%s ", lexer_token_type_string(lexer_memory->type));

    if (lexer_memory->line != line) {
      line = lexer_memory->line;
      printf("\n");
    }
    // printf("Type:%d\nLine:%lld\nStart: %lld End: %lld\n\n",
    // lexer_memory->type,
    //        lexer_memory->line, lexer_memory->start, lexer_memory->end);
    lexer_memory += 1;
  }

  // Don't forget to unmap and close the file descriptor
  munmap(file_in_memory, file_stats.st_size);
  close(file_descriptor);

  return EX_OK;
}
