#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "./headers/word-compression.h"

#define d(idx, dfl) ((argc > idx) ? argv[idx] : dfl)

int main(int argc, char **argv) {
  short error = word_compression_last_error_code();
  char *argument = d(1, "dictionary");
  char *data = NULL;

  if (strcmp("dictionary", argument) == 0) {
    error = word_compressor_create_dictionary(
        stdin, stdout, atol(d(2, WORD_COMPRESSION_FILTER)));
  } else if (strcmp("compress", argument) == 0) {
    data = word_compressor_file(d(2, NULL), stdin, &error);
  } else if (strcmp("decompress", argument) == 0) {
    data = word_decompressor_file(d(2, NULL), stdin, &error);
  } else {
    error = word_compression_error(WORD_COMPRESSION_ERROR_MISSING_ARGUMENTS,
                                   "Usage: %s [dictionary|decompress|compress]",
                                   d(0, WORD_COMPRESSION_APP_NAME));
  }

  if (error != WORD_COMPRESSION_SUCCESS) {
    fprintf(stderr, "%d: %s\n", abs(error), word_compression_last_error());
    fflush(stderr);
  }

  if (data != NULL) {
    fprintf(stdout, "%s", data);
    word_compression_free_string(&data);
    fflush(stdout);
  }

#ifdef DEBUG
#if DEBUG == 1
  get_memory_table();
  fprintf(stderr, "memory used: %ld\n", word_compression_allocated());
  fflush(stderr);
#endif
#endif

  return error == WORD_COMPRESSION_SUCCESS ? EXIT_SUCCESS : abs(error);
}