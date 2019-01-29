#include <stdio.h>
#include <string.h>

#include "./headers/callbacks.h"
#include "./headers/codes.h"
#include "./headers/types.h"
#include "./headers/word-compression.h"
#include "./headers/word-compressor.h"

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
  word_compression_free_dictionary(&compression_argument->dictionary, 1, 1);
  bzero(compression_argument, sizeof(WordCompressionArguments));
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
    *error = word_compression_error(
        WORD_COMPRESSION_ERROR_CORRUPTION,
        "dictionary is corrupted, can't find %s key", word);
  } else {
    word_compression_realloc((void **)&compression_argument->str,
                             &compression_argument->size, strlen(node->value));
    strcat(compression_argument->str, node->value);
  }

  word_compression_free_string(&word);
  return 0;
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
