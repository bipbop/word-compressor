#include "filter.h"
#include "codes.h"
#include "dictionary-binary-tree.h"
#include "dictionary.h"
#include "types.h"

short word_compression_filter(WordCompressionNode **node, void *min_occurrences,
                              short *error, short value_search) {
  if ((*node)->occurrences < *(unsigned long *)min_occurrences) {
    word_compression_tree_delete(node, 1, value_search);
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
