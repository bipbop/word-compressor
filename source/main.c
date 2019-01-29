#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "./headers/word-compression.h"

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
  FILE *fp = (FILE *)arguments;

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

short word_compressor_create_dictionary(FILE *input, FILE *output,
                                        unsigned long filter_ocurrences) {
  WordCompressionNode *current_node = NULL;
  short error = WORD_COMPRESSION_SUCCESS;
  unsigned long size = 0;

  size =
      word_compression_generate_dictionary_file(&current_node, input, &error);
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

  word_compression_dictionary_iterator(&current_node, print_format, output,
                                       &error);
  word_compression_free_dictionary(&current_node, 1, 1);
  return error;
}

unsigned long word_decompressor(char *text, unsigned long start,
                                unsigned long end, void **callback_parameters,
                                short alphanum, short *error) {
  char *word = NULL;
  WordCompressionArguments *compression_argument = (WordCompressionArguments *)*callback_parameters;
  WordCompressionNode *node = NULL;
  if (!alphanum) {
    *error = word_compressor_append_string(text, start, end, compression_argument);
    return 0;
  }

  word = word_compression_pinch_string(text, start, end);

  node = word_compression_tree_search(word, compression_argument->local_dictionary, 0);
  if (!node) {
    node = word_compression_tree_search(word, compression_argument->dictionary, 0);
  }

  if (!node) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_CORRUPTION, "dictionary is corrupted, can't find %s key", word);
  } else {
    word_compression_realloc((void **)&compression_argument->str, &compression_argument->size, strlen(node->value));
    strcat(compression_argument->str, node->value);
  }

  word_compression_free_string(&word);
  return 0;
}

char *decompress(char *dictionary, FILE *input, short *error) {
  char *string = NULL;
  WordCompressionArguments compression_argument = {0};
  WordCompressionArguments *compression_argument_ptr = &compression_argument;

  if (dictionary == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_MISSING_ARGUMENTS,
                                    "please define the directory");
    return NULL;
  }

  if (dictionary == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_MISSING_ARGUMENTS,
                                    "invalid file pointer");
    return NULL;
  }

  unsigned long words = 0;
  words += word_compression_open(file_extension(dictionary),
                                 &compression_argument.dictionary, 0, error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_arguments(&compression_argument);
    return NULL;
  }

  words += word_compression_parse_dict(
      input, &compression_argument.local_dictionary, 0, error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_arguments(&compression_argument);
    return NULL;
  }

  compression_argument.dictionary_length = words;
  compression_argument.size = 1;

  word_compression_file(input, word_decompressor,
                        (void **)&compression_argument_ptr, error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_arguments(&compression_argument);
    return NULL;
  }

  string = compression_argument.str;
  compression_argument.str = NULL;
  word_compression_free_arguments(&compression_argument);
  return string;
}

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
    data = decompress(d(2, NULL), stdin, &error);
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