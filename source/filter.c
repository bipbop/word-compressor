#include "./headers/filter.h"
#include "./headers/codes.h"
#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary.h"
#include "./headers/types.h"

short word_compression_filter(WordCompressionNode **node, void *min_occurrences,
                              short *error) {
  if ((*node)->occurrences < *(unsigned *)min_occurrences) {
    word_compression_dictionary_binary_tree_delete(node, 1);
  }
  return WORD_COMPRESSION_SUCCESS;
}
