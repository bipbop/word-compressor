
#include <stdio.h>
#include <string.h>

#include "codes.h"
#include "word-compressor.h"

char *word_compressor_string(char *dict, char *buffer, short *error) {
  FILE *stream;
  char *response;
  if ((stream = fmemopen(buffer, strlen(buffer), "r")) == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    return NULL;
  }
  response = word_compressor_file(dict, stream, error);
  fclose(stream);
  return response;
}

char *word_decompressor_string(char *dict, char *buffer, short *error) {
  FILE *stream;
  char *response;
  if ((stream = fmemopen(buffer, strlen(buffer), "r")) == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    return NULL;
  }
  response = word_decompressor_file(dict, stream, error);
  fclose(stream);
  return response;
}