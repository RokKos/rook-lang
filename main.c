#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sysexits.h>
#include <unistd.h>

typedef char unsigned u8;
typedef short unsigned u16;
typedef int unsigned u32;
typedef long long unsigned u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef float f32;
typedef double f64;

#define internal static
#define global static
#define local_persist static

const char K_COMMAND_RUN[4] = "run\0";
const char K_COMMAND_BUILD[6] = "build\0";
const char K_USAGE_INSTRUCTION[] =
    "Usage mmel <command> <file_path>\nCommands:\nrun - Builds and runs the "
    "program\nbuild - Builds the program\n";

i32 main(i32 argc, char **argv) {

  if (argc != 3) {
    perror(K_USAGE_INSTRUCTION);

    return EX_USAGE;
  }

  char *program_name = argv[0];
  char *program_command = argv[1];
  char *program_file = argv[2];

  i32 do_build = strcmp(program_command, K_COMMAND_BUILD);
  i32 do_run = strcmp(program_command, K_COMMAND_RUN);
  if (do_build != 0 && do_run != 0) {
    perror(K_USAGE_INSTRUCTION);

    return EX_USAGE;
  }

  printf("Running %s compiler to %s file %s \n", program_name, program_command,
         program_file);

  i32 file_descriptor = open(program_file, O_RDONLY);
  if (file_descriptor == -1) {
    perror("Error opening program file\n");
    return EX_NOINPUT;
  }

  struct stat file_stats;
  if (fstat(file_descriptor, &file_stats) == -1) {
    perror("Error getting file size");
    close(file_descriptor);
    return EX_DATAERR;
  }

  const i32 offset = 0;
  char *file_in_memory = mmap(NULL, file_stats.st_size, PROT_READ, MAP_PRIVATE,
                              file_descriptor, offset);
  if (file_in_memory == MAP_FAILED) {
    perror("Error mapping file");
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

  for (i64 index = 0; index < file_stats.st_size; ++index) {
    printf("%c", file_in_memory[index]);
  }

  // Don't forget to unmap and close the file descriptor
  munmap(file_in_memory, file_stats.st_size);
  close(file_descriptor);

  return EX_OK;
}
