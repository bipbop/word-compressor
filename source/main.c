#include <stdio.h>
#include <stdlib.h>

#include "./headers/callbacks.h"
#include "./headers/codes.h"
#include "./headers/create-index.h"
#include "./headers/dictionary-generator.h"
#include "./headers/dictionary-iterator.h"
#include "./headers/types.h"

short print(DictionaryNode **node, void *arguments, short *error) {
  printf("%s\t%s\t%ld\n", (*node)->value, (*node)->index, (*node)->occurrences);
  return 0;
}

int main() {
  DictionaryNode *current_node = NULL;
  short error = 0;
  unsigned long size =
      word_compressor_generate_dictionary_file(&current_node, stdin, &error);
  word_compressor_create_index(current_node, size);
  word_compressor_dictionary_iterator(&current_node, print, NULL, &error);

  if (word_compressor_last_error_code() != WORD_COMPRESSOR_SUCCESS) {
    fprintf(stderr, "%s\n", word_compressor_last_error());
    fflush(stderr);
    return abs(word_compressor_last_error_code());
  }
  return 0;
}
