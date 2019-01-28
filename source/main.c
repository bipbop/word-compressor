#include <stdio.h>
#include <stdlib.h>

#include "./headers/callbacks.h"
#include "./headers/codes.h"
#include "./headers/create-index.h"
#include "./headers/dictionary-generator.h"
#include "./headers/dictionary-iterator.h"
#include "./headers/dictionary.h"
#include "./headers/filter.h"
#include "./headers/memory-management.h"
#include "./headers/types.h"

#define default_filter 3

short print(DictionaryNode **node, void *arguments, short *error) {
  printf("%s\t%s\t%ld\n", (*node)->value, (*node)->index, (*node)->occurrences);
  return 0;
}

int main(int argc, char **argv) {
  DictionaryNode *current_node = NULL;
  short error = 0;
  unsigned int filter_ocurrences =
      argc > 2 ? abs(atoi(argv[1])) : default_filter;
  unsigned long size =
      word_compressor_generate_dictionary_file(&current_node, stdin, &error);

  printf("%ld\n", word_compressor_allocated());
  word_compressor_create_index(&current_node, size);
  printf("%ld\n", word_compressor_allocated());
  word_compressor_dictionary_iterator(&current_node, word_compressor_filter,
                                      &filter_ocurrences, &error);
  printf("%ld\n", word_compressor_allocated());
  word_compressor_dictionary_iterator(&current_node, print, NULL, &error);
  word_compressor_dictionary_free(&current_node, 1, 1);
  printf("%ld\n", word_compressor_allocated());

  get_memory_table();

  if (word_compressor_last_error_code() != WORD_COMPRESSOR_SUCCESS) {
    fprintf(stderr, "%s\n", word_compressor_last_error());
    fflush(stderr);
    return abs(word_compressor_last_error_code());
  }
  return 0;
}
