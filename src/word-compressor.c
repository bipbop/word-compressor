#include <stdio.h>
#include <string.h>

#include "callbacks.h"
#include "codes.h"
#include "types.h"
#include "word-compression.h"
#include "word-compressor.h"

short word_compressor_append_string(
    char *text, unsigned long start, unsigned long end,
    WordCompressionArguments *compression_argument) {

  char *word = word_compression_pinch_string(text, start, end);
  if (!word) {
    return word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
  }

  if (word_compression_realloc((void **)&compression_argument->str,
                               &compression_argument->size,
                               strlen(word)) == NULL) {
    word_compression_free_string(&word);
    return word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
  }

  strcat(compression_argument->str, word);
  word_compression_free_string(&word);

  return WORD_COMPRESSION_SUCCESS;
}

unsigned long word_compressor(char *text, unsigned long start,
                              unsigned long end, void **callback_parameters,
                              short alphanum, short *error) {
  WordCompressionArguments *compression_argument =
      (WordCompressionArguments *)*callback_parameters;

  char *word = NULL;
  char *index = NULL;
  WordCompressionNode *result = NULL;
  unsigned long index_size = 0;

  if (!alphanum) {
    *error =
        word_compressor_append_string(text, start, end, compression_argument);
    return 0;
  }

  word = word_compression_pinch_string(text, start, end);
  result =
      word_compression_tree_search(word, compression_argument->dictionary, 1);

  if (result != NULL) {
    if (word_compression_realloc((void **)&compression_argument->str,
                                 &compression_argument->size,
                                 strlen(result->index)) == NULL) {
      word_compression_free_string(&word);
      *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
      return 0;
    }
    word_compression_free_string(&word);
    strcat(compression_argument->str, result->index);
    return 0;
  }

  result = word_compression_tree_search(
      word, compression_argument->local_dictionary, 1);
  if (result != NULL) {
    result->occurrences++;
    if (word_compression_realloc((void **)&compression_argument->str,
                                 &compression_argument->size,
                                 strlen(result->index)) == NULL) {
      word_compression_free_string(&word);
      *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
      return 0;
    }
    word_compression_free_string(&word);
    strcat(compression_argument->str, result->index);
    return 0;
  }

  if (!word_compression_tree_search(
          word, compression_argument->local_dictionary, 0)) {
    word_compression_free_string(&word);
    *error =
        word_compressor_append_string(text, start, end, compression_argument);
    return 0;
  }

  index_size = word_compression_position_index(
      NULL, 0, ++compression_argument->dictionary_length);
  index = (char *)word_compression_malloc(index_size * sizeof(char));
  if (index == NULL) {
    word_compression_free_string(&word);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    return 0;
  }

  index_size -= word_compression_position_index(
      index, index_size, compression_argument->dictionary_length);

  result = word_compression_dictionary(word, index, NULL, 1);
  if (result == NULL) {
    word_compression_free_string(&word);
    word_compression_free_string(&index);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    return 0;
  }

  if (word_compression_realloc((void **)&compression_argument->str,
                               &compression_argument->size,
                               strlen(result->index)) == NULL) {
    word_compression_free_string(&word);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    return 0;
  }

  word_compression_tree_push(result, &compression_argument->local_dictionary,
                             compression_argument->local_dictionary, 1);

  strcat(compression_argument->str, result->index);

  return 1;
}

short word_compressor_dict_line(DictionaryArray *dictionary_array, char **ptr,
                                unsigned long *ptr_size) {
#define ARGS                                                                   \
  dictionary_array->list[dictionary_array->position]->value,                   \
      dictionary_array->list[dictionary_array->position]->index,               \
      dictionary_array->list[dictionary_array->position]->occurrences
  char *buffer = NULL;
  unsigned long size = snprintf(NULL, 0, WORD_COMPRESSION_FORMAT, ARGS);
  if (size < 0)
    return word_compression_error(
        WORD_COMPRESSION_ERROR_STDIO,
        "error formation string (" WORD_COMPRESSION_FORMAT ")");

  if (word_compression_realloc((void **)ptr, ptr_size, size) == NULL)
    return word_compression_error(WORD_COMPRESSION_ERROR_ALLOC,
                                  "could not allocate %ld bytes to dict line",
                                  size);

  buffer = (char *)word_compression_malloc(size + 1);
  if (buffer == NULL)
    return word_compression_error(
        WORD_COMPRESSION_ERROR_ALLOC,
        "could not allocate %ld bytes to dict line buffer", size);

  size -= snprintf(buffer, size + 1, WORD_COMPRESSION_FORMAT, ARGS);
  if (size != 0) {
    word_compression_free_string(&buffer);
    return word_compression_error(WORD_COMPRESSION_ERROR_LOGICAL,
                                  "%ld bytes aren't registered at %p", size,
                                  *ptr);
  }

  strcat(*ptr, buffer);
  word_compression_free_string(&buffer);

  return WORD_COMPRESSION_SUCCESS;
}

void word_compression_free_arguments(
    WordCompressionArguments *compression_argument) {
  if (compression_argument == NULL)
    return;
  word_compression_free_string(&compression_argument->str);
  word_compression_free_dictionary(&compression_argument->local_dictionary, 1,
                                   1);
  word_compression_free_dictionary(&compression_argument->index_dictionary, 1,
                                   1);
  word_compression_free_dictionary(&compression_argument->dictionary, 1, 1);
  bzero(compression_argument, sizeof(WordCompressionArguments));
}

unsigned long word_decompressor(char *text, unsigned long start,
                                unsigned long end, void **callback_parameters,
                                short alphanum, short *error) {
  char *word = NULL;
  WordCompressionArguments *compression_argument =
      (WordCompressionArguments *)*callback_parameters;
  WordCompressionNode *node = NULL;
  if (!alphanum) {
    *error =
        word_compressor_append_string(text, start, end, compression_argument);
    return 0;
  }

  word = word_compression_pinch_string(text, start, end);

  node = word_compression_tree_search(
      word, compression_argument->local_dictionary, 0);
  if (!node) {
    node =
        word_compression_tree_search(word, compression_argument->dictionary, 0);
  }

  if (!node) {
    *error =
        word_compressor_append_string(text, start, end, compression_argument);
  } else {
    word_compression_realloc((void **)&compression_argument->str,
                             &compression_argument->size, strlen(node->value));
    strcat(compression_argument->str, node->value);
  }

  word_compression_free_string(&word);
  return 0;
}



short print_format(WordCompressionNode **node, void *arguments, short *error) {
  WC_FILE *fp = (WC_FILE *)arguments;

  int bytes = snprintf(NULL, 0, WORD_COMPRESSION_FORMAT, (*node)->value,
                       (*node)->index, (*node)->occurrences);

  if (bytes < 0) {
    return word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
  }

  char *string = (char *)word_compression_malloc(bytes + 1);
  if (bytes != snprintf(string, bytes + 1, WORD_COMPRESSION_FORMAT,
                        (*node)->value, (*node)->index, (*node)->occurrences)) {
    word_compression_free_string(&string);
    return word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
  }

  if (!WC_FWRITE(string, bytes * sizeof(char), fp)) {
    word_compression_free_string(&string);
    return word_compression_error(WORD_COMPRESSION_ERROR_CORRUPTION,
                                  "can't write in the output file");
  }

  word_compression_free_string(&string);

  if (WC_FFLUSH(fp) != 0) {
    return word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
  }

  return WORD_COMPRESSION_SUCCESS;
}