#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <utf8proc.h>

#include "./headers/codes.h"
#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary.h"
#include "./headers/file.h"
#include "./headers/memory-management.h"
#include "./headers/pinch-string.h"
#include "./headers/word-reader.h"

long dictionary(char *text, unsigned long start, unsigned long end,
                void **callback_parameters, short alphanum, short *error) {
  DictionaryNode **mainNode = (DictionaryNode **)callback_parameters;
  unsigned short is_new = 0;
  char *word = word_compressor_pinch_string(text, start, end);

  if (!alphanum)
    return 0;

  if (word == NULL) {
    *error =
        word_compressor_error(WORD_COMPRESSOR_ERROR_ALLOC,
                              "not enought memory to allocate string value");
    return 0;
  }
  if (word_compressor_dictionary_binary_tree_insert(word, mainNode, *mainNode,
                                                    &is_new) == NULL) {
    *error = word_compressor_error(
        WORD_COMPRESSOR_ERROR_ALLOC,
        "not enought memory to allocate a new dictionary node");
    return 0;
  }
  if (is_new) {
    return 1;
  }
  word_compressor_free_string(&word);
  return 0;
}

unsigned long
word_compressor_generate_dictionary_file(DictionaryNode **current_node,
                                         FILE *fp, short *error) {
  return word_compressor_file(fp, dictionary, (void **)current_node, error);
}

unsigned long word_compressor_generate_dictionary(DictionaryNode **current_node,
                                                  char *str,
                                                  unsigned long length,
                                                  short *error) {
  return word_compressor_reader(str, length, dictionary, (void **)current_node,
                                error);
}
