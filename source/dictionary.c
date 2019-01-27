#include <string.h>

#include "./headers/dictionary.h"
#include "./headers/memory-management.h"
#include "./headers/types.h"

DictionaryNode *word_compressor_dictionary(char *word, DictionaryNode *parent,
                                           unsigned long occurrences) {
  DictionaryNode *dictionaryNode =
      (DictionaryNode *)word_compressor_malloc(sizeof(DictionaryNode));
  if (dictionaryNode == NULL)
    return NULL;
  dictionaryNode->value = word;
  dictionaryNode->index = word;
  dictionaryNode->parent = parent;
  dictionaryNode->occurrences = occurrences;
  dictionaryNode->left = NULL;
  dictionaryNode->right = NULL;
  return dictionaryNode;
}

void word_compressor_dictionary_free(DictionaryNode **node,
                                     unsigned short int free_word,
                                     unsigned short int free_children) {
  if (free_word && (*node)->value != NULL) {
    if ((*node)->value == (*node)->index) {
      word_compressor_free_string(&(*node)->value);
      (*node)->index = NULL;
    } else if ((*node)->index != NULL) {
      word_compressor_free_string(&(*node)->value);
      word_compressor_free_string(&(*node)->index);
    }
  } else if (free_word && (*node)->index != NULL) {
    word_compressor_free_string(&(*node)->index);
  }

  if (free_children && (*node)->left != NULL)
    word_compressor_dictionary_free(&(*node)->left, free_word, 1);
  if (free_children && (*node)->right != NULL)
    word_compressor_dictionary_free(&(*node)->right, free_word, 1);
  word_compressor_free((void **)node, sizeof(DictionaryNode));
}