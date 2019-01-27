#include "./headers/filter.h"
#include "./headers/codes.h"
#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary.h"
#include "./headers/types.h"

short word_compressor_filter(DictionaryNode **node, void *min_occurrences,
                             short *error) {
  if ((*node)->occurrences < *(unsigned int *)min_occurrences) {
    word_compressor_dictionary_binary_tree_delete(node, 1);
  }
  return WORD_COMPRESSOR_SUCCESS;
}
