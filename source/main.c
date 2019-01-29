#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "./headers/word-compression.h"

FILE *fp = NULL;

char file_path_buffer[FILENAME_MAX + 1] = {0};
char extension_buffer[FILENAME_MAX + 1] = {0};

const char *file_extension(const char *path) {
  if (path == NULL)
    return NULL;
  int length_path = strlen(path);
  if (length_path > 4 &&
      strcmp(path + (length_path - 4), WORD_COMPRESSION_EXTENSION) == 0)
    return path;
  snprintf(extension_buffer, FILENAME_MAX * sizeof(char), "%s%s", path,
           WORD_COMPRESSION_EXTENSION);
  if (realpath(extension_buffer, file_path_buffer) == NULL)
    return extension_buffer;
  return file_path_buffer;
}

short print_format(WordCompressionNode **node, void *arguments, short *error) {
  int bytes = fprintf(fp, WORD_COMPRESSION_FORMAT, (*node)->value,
                      (*node)->index, (*node)->occurrences);

  if (bytes < 0 || ferror(fp)) {
    return word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
  }

  if (fflush(fp) != 0) {
    return word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
  }

  return WORD_COMPRESSION_SUCCESS;
}

short word_compressor_create_dictionary(FILE *fp, const char *file,
                                        unsigned filter_ocurrences) {
  WordCompressionNode *current_node = NULL;
  short error = WORD_COMPRESSION_SUCCESS;
  unsigned long size = 0;

  file = file_extension(file);
  size =
      word_compression_generate_dictionary_file(&current_node, fp, &error);
  if (error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_dictionary(&current_node, 1, 1);
    return error;
  }

  error = word_compression_create_index(&current_node, size);
  if (error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_dictionary(&current_node, 1, 1);
    return error;
  }

  word_compression_dictionary_iterator(
      &current_node, word_compression_filter_index, &filter_ocurrences, &error);
  if (error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_dictionary(&current_node, 1, 1);
    return error;
  }

  error = word_compressor_write_in(file, &fp);
  if (error) {
    word_compression_free_dictionary(&current_node, 1, 1);
    return error;
  }

  word_compression_dictionary_iterator(&current_node, print_format, NULL,
                                       &error);
  fclose(fp);
  word_compression_free_dictionary(&current_node, 1, 1);
  return error;
}

char *decompress(char *dictionary, char *target) {
  short error = WORD_COMPRESSION_SUCCESS;

  if (dictionary == NULL || target == NULL) {
    return word_compression_error(WORD_COMPRESSION_ERROR_MISSING_ARGUMENTS,
                                  "usage: %s decompress [dict%s] [target]",
                                  WORD_COMPRESSION_APP_NAME,
                                  WORD_COMPRESSION_EXTENSION);
  }

  error = word_compressor_write_in(NULL, &fp);
  if (error != WORD_COMPRESSION_SUCCESS)
    return error;

  WordCompressionNode *dictionary_node = NULL;
  word_compression_open(file_extension(dictionary), &dictionary_node, 0,
                        &error);
  if (error != WORD_COMPRESSION_SUCCESS) {
    word_compression_dictionary_iterator(&dictionary_node, print_format, NULL,
                                         &error);
  }

  word_compression_free_dictionary(&dictionary_node, 1, 1);

  return error;
}

#define d(idx, dfl) ((argc > idx) ? argv[idx] : dfl)

int main(int argc, char **argv) {
  short error = word_compression_last_error_code();
  char *argument = d(1, "dictionary");
  char *data = NULL;

  if (strcmp("dictionary", argument) == 0) {
    error = word_compressor_create_dictionary(stdin, d(2, NULL), atoi(d(3, WORD_COMPRESSION_FILTER)));
  } else if (strcmp("compress", argument) == 0) {
    data = word_compressor_file(d(2, NULL), d(3, NULL), &error);
  } else if (strcmp("decompress", argument) == 0) {
    data = decompress(d(2, NULL), d(3, NULL));
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
  printf("memory used: %ld\n", word_compression_allocated());
#endif
#endif

  return error == WORD_COMPRESSION_SUCCESS ? EXIT_SUCCESS : abs(error);
}