#include <stdio.h>
#include <string.h>
#include <time.h>

#include "wrappers.h"
#include "filter.h"
#include "codes.h"
#include "word-compressor.h"
#include "create-index.h"
#include "file.h"
#include "memory-management.h"
#include "dictionary-generator.h"
#include "dictionary-iterator.h"
#include "dictionary.h"
#include "libwordcompression.h"

short word_compressor_create_dictionary(WC_FILE *input, WC_FILE *output,
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


char *word_compressor_file(char *dictionary, WC_FILE *fp_target, short *error) {
  char *local_dictionary = NULL;
  char *string = NULL;
  unsigned long local_dictionary_size = 1;
  unsigned long string_size = 0;

  WordCompressionArguments compression_argument = {0};
  WordCompressionArguments *compression_argument_ptr = &compression_argument;

  DictionaryArray dictionary_array = {0};

  unsigned long bag_of_words = 0;

  compression_argument.size = 1;

  if (dictionary == NULL || fp_target == NULL) {
    *error =
        word_compression_error(WORD_COMPRESSION_ERROR_MISSING_ARGUMENTS, NULL);
    return NULL;
  }

  compression_argument.dictionary_length = word_compression_open(
      dictionary, &compression_argument.dictionary, 1, error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_arguments(&compression_argument);
    return NULL;
  }

  word_compression_open(dictionary, &compression_argument.index_dictionary, 0,
                        error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_arguments(&compression_argument);
    return NULL;
  }

  bag_of_words = word_compression_file(
      fp_target, word_compressor, (void **)&compression_argument_ptr, error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_arguments(&compression_argument);
    return NULL;
  }

  if (bag_of_words) {
    *error = word_compression_array(&dictionary_array,
                                    &compression_argument.local_dictionary,
                                    bag_of_words);

    if (*error != WORD_COMPRESSION_SUCCESS) {
      word_compression_free_arguments(&compression_argument);
      word_compressor_free_array(&dictionary_array);
      return NULL;
    }

    for (dictionary_array.position = 0;
         dictionary_array.position < dictionary_array.length;
         dictionary_array.position++) {
      *error = word_compressor_dict_line(&dictionary_array, &local_dictionary,
                                         &local_dictionary_size);
      if (*error != WORD_COMPRESSION_SUCCESS) {
        word_compression_free_string(&local_dictionary);
        word_compressor_free_array(&dictionary_array);
        word_compression_free_arguments(&compression_argument);
        return NULL;
      }
    }

    word_compressor_free_array(&dictionary_array);
  }

  if (local_dictionary == NULL) {
    string_size = snprintf(NULL, 0, "\n\n%s", compression_argument.str);
  } else {
    string_size = snprintf(NULL, 0, "%s\n\n%s", local_dictionary,
                           compression_argument.str);
  }
  if (string_size < 0) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    word_compression_free_string(&local_dictionary);
    word_compression_free_arguments(&compression_argument);
    return NULL;
  }

  string = (char *)word_compression_malloc(string_size + 1);
  if (string == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    word_compression_free_string(&local_dictionary);
    word_compression_free_arguments(&compression_argument);
    return NULL;
  }

  if (local_dictionary == NULL) {
    string_size -=
        snprintf(string, string_size + 1, "\n\n%s", compression_argument.str);
  } else {
    string_size -= snprintf(string, string_size + 1, "%s\n\n%s",
                            local_dictionary, compression_argument.str);
  }
  if (string_size != 0) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    word_compression_free_string(&string);
  }

  word_compression_free_arguments(&compression_argument);
  word_compression_free_string(&local_dictionary);

  return string;
}

char *word_decompressor_file(char *dictionary, WC_FILE *input, short *error) {
  char *string = NULL;
  WordCompressionArguments compression_argument = {0};
  WordCompressionArguments *compression_argument_ptr = &compression_argument;

  if (dictionary == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_MISSING_ARGUMENTS,
                                    "please define the directory");
    return NULL;
  }

  if (input == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_MISSING_ARGUMENTS,
                                    "invalid file pointer");
    return NULL;
  }

  unsigned long words = 0;
  words += word_compression_open(dictionary, &compression_argument.dictionary,
                                 0, error);
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