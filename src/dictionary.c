#include <string.h>

#include "dictionary.h"
#include "memory-management.h"
#include "types.h"

WordCompressionNode *word_compression_dictionary(char *word, char *index,
                                                 WordCompressionNode *parent,
                                                 unsigned long occurrences) {
  WordCompressionNode *dictionaryNode =
      (WordCompressionNode *)word_compression_malloc(
          sizeof(WordCompressionNode));
  if (dictionaryNode == NULL)
    return NULL;
  dictionaryNode->value = word;
  dictionaryNode->index = (index == NULL ? word : index);
  dictionaryNode->parent = parent;
  dictionaryNode->occurrences = occurrences;
  dictionaryNode->left = NULL;
  dictionaryNode->right = NULL;
  return dictionaryNode;
}

void word_compression_free_dictionary(WordCompressionNode **node,
                                      unsigned short int free_word,
                                      unsigned short int free_children) {
  if (*node == NULL)
    return;
  if (free_word && (*node)->value != NULL) {
    if ((*node)->value == (*node)->index) {
      word_compression_free_string(&(*node)->value);
      (*node)->index = NULL;
    } else if ((*node)->index != NULL) {
      word_compression_free_string(&(*node)->value);
      word_compression_free_string(&(*node)->index);
    }
  } else if (free_word && (*node)->index != NULL) {
    word_compression_free_string(&(*node)->index);
  }

  if (free_children && (*node)->left != NULL)
    word_compression_free_dictionary(&(*node)->left, free_word, 1);
  if (free_children && (*node)->right != NULL)
    word_compression_free_dictionary(&(*node)->right, free_word, 1);
  word_compression_free((void **)node, sizeof(WordCompressionNode));
}