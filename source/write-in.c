#include <stdio.h>

#include "./headers/codes.h"

short word_compressor_write_in(const char *path, FILE **fp) {
  if (path == NULL) {
    if (stdout == NULL)
      return word_compression_error(WORD_COMPRESSION_ERROR_STDIO,
                                    "closed stdout");
    *fp = stdout;
    return WORD_COMPRESSION_SUCCESS;
  }

  *fp = fopen(path, "w");
  fprintf(stdout, "%s\n", path);
  fflush(stdout);

  if (*fp == NULL)
    return word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
  return WORD_COMPRESSION_SUCCESS;
}