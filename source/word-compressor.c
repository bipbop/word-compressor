#include <stdio.h>
#include <string.h>

#include "./headers/codes.h"
#include "./headers/types.h"
#include "./headers/callbacks.h"
#include "./headers/word-compressor.h"
#include "./headers/word-compression.h"

short word_compressor_append_string(
    char *text, unsigned long start, unsigned long end,
    WordCompressionArguments *compression_argument) {

  word_compression_realloc((void **)&compression_argument->str,
                           &compression_argument->str_size, end - start + 1);
  if (compression_argument->str == NULL) {
    return word_compression_error(
        WORD_COMPRESSION_ERROR_ALLOC, "cannot allocate %d bytes",
        compression_argument->str_size + end - start + 1);
  }

  if (snprintf(compression_argument->str + strlen(compression_argument->str),
               end - start + 1, "%.*s", (int)(end - start), text + start) < 0) {
    return word_compression_error(WORD_COMPRESSION_ERROR_STDIO,
                                  "failed to append string at snprintf");
  }

  return WORD_COMPRESSION_SUCCESS;
}


unsigned long word_compressor(char *text, unsigned long start, unsigned long end,
                     void **callback_parameters, short alphanum, short *error) {
  WordCompressionArguments *compression_argument =
      (WordCompressionArguments *)*callback_parameters;
  char *word = NULL;
  WordCompressionNode *result = NULL;

  if (!alphanum) {
    *error =
        word_compressor_append_string(text, start, end, compression_argument);
    return 0;
  }

  word = word_compression_pinch_string(text, start, end);
  result = word_compression_dictionary_binary_tree_search(
      word, compression_argument->dictionary, 1);

  if (result != NULL) {
    if (word_compression_realloc((void **)&compression_argument->str,
                                 &compression_argument->str_size,
                                 strlen(word) + 1) == NULL) {
      word_compression_free_string(&word);
      *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
      return 0;
    }
    word_compression_free_string(&word);
    strcat(compression_argument->str, result->index);
    return 0;
  }

  return 1;
}

char * word_compressor_dict(char *dictionary, char *target, short *error) {
  if (dictionary == NULL || target == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_MISSING_ARGUMENTS, NULL);
    return NULL;
  }


  WordCompressionNode *dictionary_node = NULL;
  word_compression_open(dictionary, &dictionary_node, 1, error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    word_compression_dictionary_free(&dictionary_node, 1, 1);
    return NULL;
  }

  FILE *fp_target = fopen(target, "r");
  WordCompressionArguments compression_argument = {0};
  compression_argument.dictionary = dictionary_node;

  WordCompressionArguments *compression_argumentPtr = &compression_argument;
  compression_argument.dictionary_size = word_compression_file(fp_target, word_compressor,
                        (void **)&compression_argumentPtr, error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    word_compression_dictionary_free(&dictionary_node, 1, 1);
    return NULL;
  }

  if (stdout == NULL) {
    word_compression_dictionary_free(&dictionary_node, 1, 1);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    return NULL;
  }
 
  word_compression_dictionary_free(&dictionary_node, 1, 1);
  return compression_argument.str;
}
