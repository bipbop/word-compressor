#include "./headers/dictionary-iterator.h"
#include "./headers/callbacks.h"
#include "./headers/codes.h"
#include "./headers/types.h"

#define RETONERROR                                                             \
  if (res != WORD_COMPRESSOR_SUCCESS)                                          \
  return res

short word_compressor_dictionary_iterator(DictionaryNode **root,
                                          CallbackNode callback_node,
                                          void *callback_node_parameter,
                                          short *non_fatal_error) {
  int res = WORD_COMPRESSOR_SUCCESS;
  if ((*root) == NULL)
    return res;
  res = word_compressor_dictionary_iterator(
      &(*root)->left, callback_node, callback_node_parameter, non_fatal_error);
  RETONERROR;
  res = word_compressor_dictionary_iterator(
      &(*root)->right, callback_node, callback_node_parameter, non_fatal_error);
  RETONERROR;
  return callback_node(root, callback_node_parameter, non_fatal_error);
}
