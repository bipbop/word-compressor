#include <string.h>

#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary.h"
#include "./headers/types.h"

DictionaryNode *
word_compressor_dictionary_binary_tree_insert(char *key, DictionaryNode **leaf,
                                              DictionaryNode *parent,
                                              unsigned short *new) {
  DictionaryNode *node = *leaf;
  int res = 0;
  if (node == NULL) {
    *leaf = word_compressor_dictionary(key, parent, 1);
    if (*leaf == NULL)
      return NULL;
    *new = 1;
    return *leaf;
  }

  res = strcmp(key, node->index);
  if (res < 0) {
    return word_compressor_dictionary_binary_tree_insert(key, &node->left, node,
                                                         new);
  } else if (res > 0) {
    return word_compressor_dictionary_binary_tree_insert(key, &node->right,
                                                         node, new);
  }

  *new = 0;
  node->occurrences += 1;
  return node;
}

DictionaryNode *
word_compressor_dictionary_binary_tree_search(char *key,
                                              DictionaryNode *node_leaf) {
  int res = 0;

  if (node_leaf == NULL)
    return NULL;

  res = strcmp(key, node_leaf->index);
  if (res < 0) {
    return word_compressor_dictionary_binary_tree_search(key, node_leaf->left);
  } else if (res > 0) {
    return word_compressor_dictionary_binary_tree_search(key, node_leaf->right);
  }

  return node_leaf;
}

DictionaryNode *word_compressor_dictionary_binary_tree_push(
    DictionaryNode *node, DictionaryNode **leaf, DictionaryNode *parent) {
  int res = 0;
  DictionaryNode *node_leaf = *leaf;

  if (node_leaf == NULL) {
    node->parent = parent;
    *leaf = node;
    return node;
  }

  res = strcmp(node->index, node_leaf->index);
  if (res < 0) {
    return word_compressor_dictionary_binary_tree_push(node, &node_leaf->left,
                                                       node_leaf);
  } else if (res > 0) {
    return word_compressor_dictionary_binary_tree_push(node, &node_leaf->right,
                                                       node_leaf);
  }

  return node_leaf;
}

void word_compressor_dictionary_binary_tree_delete(
    DictionaryNode **leaf, unsigned short delete_words) {
  int res = 0;

  DictionaryNode *node = *leaf;
  DictionaryNode *left = node->right;
  DictionaryNode *right = node->left;
  DictionaryNode *parent = node->parent;

  *leaf = NULL;

  if (parent == NULL) {
    if (right != NULL && left != NULL) {
      *leaf = left;
      left->parent = NULL;
      word_compressor_dictionary_binary_tree_push(right, leaf, left);
    } else if (right) {
      right->parent = NULL;
      *leaf = right;
    } else if (left) {
      left->parent = NULL;
      *leaf = left;
    }
  } else {
    *leaf = parent;

    res = strcmp(node->index, parent->index);

    if (res < 0) {
      parent->left = NULL;
    } else {
      parent->right = NULL;
    }

    if (left != NULL)
      word_compressor_dictionary_binary_tree_push(left, leaf, parent);
    if (right != NULL)
      word_compressor_dictionary_binary_tree_push(right, leaf, parent);
  }

  word_compressor_dictionary_free(&node, 1, 0);
}

DictionaryNode *
word_compressor_dictionary_binary_tree_base(DictionaryNode *node) {
  if (node == NULL)
    return NULL;
  while (node->parent != NULL) {
    node = node->parent;
  }
  return node;
}

void word_compressor_dictionary_binary_tree_free(DictionaryNode **node,
                                                 unsigned short delete_words) {
  if ((*node) == NULL)
    return;
  DictionaryNode *father = word_compressor_dictionary_binary_tree_base(*node);
  word_compressor_dictionary_free(&father, delete_words, 1);
  *node = NULL;
}