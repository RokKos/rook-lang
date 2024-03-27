#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

extern void log_error(const char *message, const char *file,
                      unsigned long long line);

#ifdef __cplusplus
}
#endif

#ifdef LOG_IMPLEMENTATION
#include <errno.h>
#include <stdio.h>
#include <string.h>

void log_error(const char *message, const char *file, unsigned long long line) {
  char errbuf[512];
  if (errno) {
    strerror_r(errno, errbuf, sizeof(errbuf)); // Thread-safe strerror
  }

  fprintf(stderr, "ERROR:%s:%llu : %s\n", file, line, message);
  if (errno) {
    fprintf(stderr, "System ERROR: %s\n", errbuf);
  }
}

#ifdef LOG_TEST
#endif // LOG_TEST
#endif // LOG_IMPLEMENTATION
#endif // INCLUDE_LOG_H
