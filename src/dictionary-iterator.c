#include "dictionary-iterator.h"
#include "callbacks.h"
#include "codes.h"
#include "types.h"

#define RETONERROR                                                             \
  if (res != WORD_COMPRESSION_SUCCESS) {                                       \
    *non_fatal_error = res;                                                    \
    return res;                                                                \
  }

short word_compression_dictionary_iterator(
    WordCompressionNode **root, WordCompressionCallback callback_node,
    void *callback_node_parameter, short *non_fatal_error) {
  int res = WORD_COMPRESSION_SUCCESS;
  if ((*root) == NULL)
    return res;
  res = word_compression_dictionary_iterator(
      &(*root)->left, callback_node, callback_node_parameter, non_fatal_error);
  RETONERROR;
  res = word_compression_dictionary_iterator(
      &(*root)->right, callback_node, callback_node_parameter, non_fatal_error);
  RETONERROR;
  return callback_node(root, callback_node_parameter, non_fatal_error);
}
