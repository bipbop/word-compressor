#include <stdlib.h>
#include <string.h>

#include "./headers/codes.h"
#include "./headers/create-index.h"
#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary-compare.h"
#include "./headers/dictionary-iterator.h"
#include "./headers/memory-management.h"
#include "./headers/position-index.h"
#include "./headers/types.h"

typedef struct {
  DictionaryNode **list;
  unsigned long position;
  unsigned long size;
} DictionaryArray;

short int copy_list(DictionaryNode **node, void *parameter, short *error) {
  DictionaryArray *dictionary_array = (DictionaryArray *)parameter;
  if (dictionary_array->position >= dictionary_array->size) {
    *error = WORD_COMPRESSOR_ERROR_SIZE;
    return word_compressor_error(
        WORD_COMPRESSOR_ERROR_SIZE, "(wordcompression) can't copy %s (%p) "
                                    "dictionary array, list size is too small, "
                                    "position: %ld and size: %ld",
        (*node)->value, *node, dictionary_array->position,
        dictionary_array->size);
  }
  dictionary_array->list[dictionary_array->position++] = *node;
  return WORD_COMPRESSOR_SUCCESS;
}

short word_compressor_create_index(DictionaryNode **current_node,
                                   unsigned long tree_size) {
  DictionaryArray dictionary_array = {
      .list = NULL, .size = tree_size, .position = 0};

  DictionaryNode *position = NULL;
  DictionaryNode *new_list = NULL;
  short response = 0;
  short error = 0;
  unsigned long index_size = 0;
  unsigned long ordered_list_size = sizeof(DictionaryNode **) * tree_size;

  if (tree_size == 0)
    return WORD_COMPRESSOR_ERROR_EMPTY_POSITION;

  dictionary_array.list =
      (DictionaryNode **)word_compressor_malloc(ordered_list_size);
  if (dictionary_array.list == NULL) {
    return word_compressor_error(WORD_COMPRESSOR_ERROR_ALLOC,
                                 "no memory is available for index creation, "
                                 "required %ld bytes for %ld words",
                                 ordered_list_size, tree_size);
  }

  bzero(dictionary_array.list, ordered_list_size);

  response = word_compressor_dictionary_iterator(current_node, &copy_list,
                                                 &dictionary_array, &error);
  if (response < 0) {
    word_compressor_free((void **)dictionary_array.list, ordered_list_size);
    return response;
  }

  if (error < 0) {
    word_compressor_free((void **)dictionary_array.list, ordered_list_size);
    return error;
  }

  qsort(dictionary_array.list, dictionary_array.size, sizeof(DictionaryNode *),
        word_compressor_dictionary_compare_ptr);

  for (dictionary_array.position = 0;
       dictionary_array.position < dictionary_array.size;
       dictionary_array.position++) {

    position = dictionary_array.list[dictionary_array.position];

    if (position == NULL) {
      word_compressor_free((void **)&dictionary_array.list, ordered_list_size);
      return word_compressor_error(WORD_COMPRESSOR_ERROR_EMPTY_POSITION, NULL);
    }

    position->parent = NULL;
    position->left = NULL;
    position->right = NULL;

    if (position->index != NULL && position->index != position->value) {
      word_compressor_free_string(&position->index);
    }

    index_size =
        word_compressor_position_index(NULL, 0, dictionary_array.position);

    position->index = (char *)word_compressor_malloc(index_size * sizeof(char));
    if (position->index == NULL) {
      word_compressor_free((void **)&dictionary_array.list, ordered_list_size);
      return word_compressor_error(WORD_COMPRESSOR_ERROR_ALLOC, NULL);
    }

    bzero(position->index, index_size * sizeof(char));

    if (position->index == NULL) {
      word_compressor_free((void **)&dictionary_array.list, ordered_list_size);
      return word_compressor_error(WORD_COMPRESSOR_ERROR_ALLOC,
                                   "no memory is available for index creation, "
                                   "required %ld bytes to name %ld position",
                                   index_size, dictionary_array.position);
    }

    index_size -= word_compressor_position_index(position->index, index_size,
                                                 dictionary_array.position);
    if (index_size) {
      word_compressor_free((void **)&dictionary_array.list, ordered_list_size);
      return word_compressor_error(WORD_COMPRESSOR_ERROR_CORRUPTION,
                                   "the program did not write as many bytes as "
                                   "needed, remaining %ld for %ld position",
                                   index_size, dictionary_array.position);
    }

    word_compressor_dictionary_binary_tree_push(position, &new_list, new_list);
    new_list = word_compressor_dictionary_binary_tree_base(new_list);
  }

  *current_node = new_list;
  word_compressor_free((void **)&dictionary_array.list, ordered_list_size);
  return WORD_COMPRESSOR_SUCCESS;
}