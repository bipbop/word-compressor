#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <utf8proc.h>

#include "./headers/codes.h"
#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary-generator.h"
#include "./headers/dictionary.h"
#include "./headers/file.h"
#include "./headers/memory-management.h"
#include "./headers/pinch-string.h"
#include "./headers/word-reader.h"

unsigned long dictionary(char *text, unsigned long start, unsigned long end,
                         void **callback_parameters, short alphanum,
                         short *error) {
  WordCompressionNode **mainNode = (WordCompressionNode **)callback_parameters;
  unsigned short is_new = 0;
  char *word = NULL;

  if (!alphanum)
    return 0;

  word = word_compression_pinch_string(text, start, end);

  if (word == NULL) {
    *error =
        word_compression_error(WORD_COMPRESSION_ERROR_ALLOC,
                               "not enought memory to allocate string value");
    return 0;
  }
  if (word_compression_tree_insert(word, mainNode, *mainNode,
                                                     &is_new) == NULL) {
    *error = word_compression_error(
        WORD_COMPRESSION_ERROR_ALLOC,
        "not enought memory to allocate a new dictionary node");
    return 0;
  }
  if (is_new) {
    return 1;
  }
  word_compression_free_string(&word);
  return 0;
}

unsigned long
word_compression_generate_dictionary_file(WordCompressionNode **current_node,
                                          FILE *fp, short *error) {
  return word_compression_file(fp, dictionary, (void **)current_node, error);
}

unsigned long
word_compression_generate_dictionary(WordCompressionNode **current_node,
                                     char *str, unsigned long length,
                                     short *error) {
  return word_compression_reader(str, length, dictionary, (void **)current_node,
                                 error);
}
