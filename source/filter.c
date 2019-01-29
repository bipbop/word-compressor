#include "./headers/filter.h"
#include "./headers/codes.h"
#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary.h"
#include "./headers/types.h"

short word_compression_filter(WordCompressionNode **node, void *min_occurrences,
                              short *error, short value_search) {
  if ((*node)->occurrences < *(unsigned *)min_occurrences) {
    word_compression_dictionary_binary_tree_delete(node, 1, value_search);
  }
  return WORD_COMPRESSION_SUCCESS;
}

short word_compression_filter_index(WordCompressionNode **node,
                                    void *min_occurrences, short *error) {
  return word_compression_filter(node, min_occurrences, error, 0);
}

short word_compression_filter_value(WordCompressionNode **node,
                                    void *min_occurrences, short *error) {
  return word_compression_filter(node, min_occurrences, error, 1);
}
