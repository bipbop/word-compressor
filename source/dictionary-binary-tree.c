#include <string.h>

#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary.h"
#include "./headers/types.h"

WordCompressionNode *word_compression_dictionary_binary_tree_insert(
    char *key, WordCompressionNode **leaf, WordCompressionNode *parent,
    unsigned short *new) {
  WordCompressionNode *node = *leaf;
  int res = 0;
  if (node == NULL) {
    *leaf = word_compression_dictionary(key, NULL, parent, 1);
    if (*leaf == NULL)
      return NULL;
    *new = 1;
    return *leaf;
  }

  res = strcmp(key, node->index);
  if (res < 0) {
    return word_compression_dictionary_binary_tree_insert(key, &node->left,
                                                          node, new);
  } else if (res > 0) {
    return word_compression_dictionary_binary_tree_insert(key, &node->right,
                                                          node, new);
  }

  *new = 0;
  node->occurrences += 1;
  return node;
}

WordCompressionNode *word_compression_dictionary_binary_tree_search(
    char *key, WordCompressionNode *node_leaf, short value_search) {
  int res = 0;

  if (node_leaf == NULL)
    return NULL;

  res = value_search ? strcmp(key, node_leaf->value)
                     : strcmp(key, node_leaf->index);
  if (res < 0) {
    return word_compression_dictionary_binary_tree_search(key, node_leaf->left,
                                                          value_search);
  } else if (res > 0) {
    return word_compression_dictionary_binary_tree_search(key, node_leaf->right,
                                                          value_search);
  }

  return node_leaf;
}

WordCompressionNode *word_compression_dictionary_binary_tree_push(
    WordCompressionNode *node, WordCompressionNode **leaf,
    WordCompressionNode *parent, short value_search) {
  int res = 0;
  WordCompressionNode *node_leaf = *leaf;

  if (node_leaf == NULL) {
    node->parent = parent;
    *leaf = node;
    return node;
  }

  res = value_search ? strcmp(node->value, node_leaf->value)
                     : strcmp(node->index, node_leaf->index);
  if (res < 0) {
    return word_compression_dictionary_binary_tree_push(
        node, &node_leaf->left, node_leaf, value_search);
  } else if (res > 0) {
    return word_compression_dictionary_binary_tree_push(
        node, &node_leaf->right, node_leaf, value_search);
  }

  return node_leaf;
}

void word_compression_dictionary_binary_tree_delete(WordCompressionNode **leaf,
                                                    unsigned short delete_words,
                                                    short value_search) {
  int res = 0;

  WordCompressionNode *node = *leaf;
  WordCompressionNode *left = node->left;
  WordCompressionNode *right = node->right;
  WordCompressionNode *parent = node->parent;

  *leaf = NULL;

  if (parent == NULL) {
    if (right != NULL && left != NULL) {
      *leaf = left;
      left->parent = NULL;
      word_compression_dictionary_binary_tree_push(right, leaf, left,
                                                   value_search);
    } else if (right) {
      right->parent = NULL;
      *leaf = right;
    } else if (left) {
      left->parent = NULL;
      *leaf = left;
    }
  } else {
    *leaf = parent;

    res = value_search ? strcmp(node->value, parent->value)
                       : strcmp(node->index, parent->index);
    if (res < 0) {
      parent->left = NULL;
    } else {
      parent->right = NULL;
    }

    if (left != NULL)
      word_compression_dictionary_binary_tree_push(left, leaf, parent,
                                                   value_search);
    if (right != NULL)
      word_compression_dictionary_binary_tree_push(right, leaf, parent,
                                                   value_search);
  }

  word_compression_free_dictionary(&node, 1, 0);
}

WordCompressionNode *
word_compression_dictionary_binary_tree_base(WordCompressionNode *node) {
  if (node == NULL)
    return NULL;
  while (node->parent != NULL) {
    node = node->parent;
  }
  return node;
}

void word_compression_dictionary_binary_tree_free(WordCompressionNode **node,
                                                  unsigned short delete_words) {
  if ((*node) == NULL)
    return;
  WordCompressionNode *father =
      word_compression_dictionary_binary_tree_base(*node);
  word_compression_free_dictionary(&father, delete_words, 1);
  *node = NULL;
}