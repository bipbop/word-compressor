#include <stdlib.h>
#include <string.h>

#include "codes.h"
#include "create-index.h"
#include "dictionary-binary-tree.h"
#include "dictionary-compare.h"
#include "dictionary-iterator.h"
#include "memory-management.h"
#include "position-index.h"
#include "types.h"

short copy_list(WordCompressionNode **node, void *parameter, short *error) {
  DictionaryArray *dictionary_array = (DictionaryArray *)parameter;
  if (dictionary_array->position >= dictionary_array->length) {
    *error = WORD_COMPRESSION_ERROR_SIZE;
    return word_compression_error(WORD_COMPRESSION_ERROR_SIZE,
                                  "(wordcompression) can't copy %s (%p) "
                                  "dictionary array, list size is too small, "
                                  "position: %ld and size: %ld",
                                  (*node)->value, *node,
                                  dictionary_array->position,
                                  dictionary_array->length);
  }
  dictionary_array->list[dictionary_array->position++] = *node;
  return WORD_COMPRESSION_SUCCESS;
}

short word_compression_array(DictionaryArray *dictionary_array,
                             WordCompressionNode **current_node,
                             unsigned long tree_size) {
  short response = WORD_COMPRESSION_SUCCESS;
  short error = WORD_COMPRESSION_SUCCESS;

  if (dictionary_array == NULL || tree_size == 0)
    return WORD_COMPRESSION_ERROR_EMPTY_POSITION;

  dictionary_array->length = tree_size;
  dictionary_array->size = sizeof(WordCompressionNode **) * tree_size;

  dictionary_array->list =
      (WordCompressionNode **)word_compression_malloc(dictionary_array->size);
  if (dictionary_array->list == NULL) {
    return word_compression_error(WORD_COMPRESSION_ERROR_ALLOC,
                                  "no memory is available for index creation, "
                                  "required %ld bytes for %ld words",
                                  dictionary_array->size, tree_size);
  }

  response = word_compression_dictionary_iterator(current_node, copy_list,
                                                  dictionary_array, &error);
  if (response < 0) {
    word_compressor_free_array(dictionary_array);
    return response;
  }

  if (error < 0) {
    word_compressor_free_array(dictionary_array);
    return error;
  }

  return WORD_COMPRESSION_SUCCESS;
}

void word_compressor_free_array(DictionaryArray *dictionary_array) {
  if (dictionary_array == NULL)
    return;
  word_compression_free((void **)&dictionary_array->list,
                        dictionary_array->size);
  dictionary_array->size = 0;
  dictionary_array->position = 0;
  dictionary_array->length = 0;
}

short word_compression_create_index(WordCompressionNode **current_node,
                                    unsigned long tree_size) {
  DictionaryArray dictionary_array = {0};
  WordCompressionNode *new_list = NULL;
  WordCompressionNode *position = NULL;

  short response = WORD_COMPRESSION_SUCCESS;
  unsigned long index_size = 0;

  response = word_compression_array(&dictionary_array, current_node, tree_size);
  if (response != WORD_COMPRESSION_SUCCESS) {
    return word_compression_error(response, NULL);
  }

  qsort(dictionary_array.list, dictionary_array.length,
        sizeof(WordCompressionNode *), word_compression_dictionary_compare_ptr);

  for (dictionary_array.position = 0;
       dictionary_array.position < dictionary_array.length;
       dictionary_array.position++) {

    position = dictionary_array.list[dictionary_array.position];

    if (position == NULL) {
      word_compression_free((void **)&dictionary_array.list,
                            dictionary_array.size);
      return word_compression_error(WORD_COMPRESSION_ERROR_EMPTY_POSITION,
                                    NULL);
    }

    position->parent = NULL;
    position->left = NULL;
    position->right = NULL;

    if (position->index != NULL && position->index != position->value) {
      word_compression_free_string(&position->index);
    }

    index_size =
        word_compression_position_index(NULL, 0, dictionary_array.position);

    position->index =
        (char *)word_compression_malloc(index_size * sizeof(char));
    if (position->index == NULL) {
      word_compression_free((void **)&dictionary_array.list,
                            dictionary_array.size);
      return word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    }

    if (position->index == NULL) {
      word_compression_free((void **)&dictionary_array.list,
                            dictionary_array.size);
      return word_compression_error(
          WORD_COMPRESSION_ERROR_ALLOC,
          "no memory is available for index creation, "
          "required %ld bytes to name %ld position",
          index_size, dictionary_array.position);
    }

    index_size -= word_compression_position_index(position->index, index_size,
                                                  dictionary_array.position);
    if (index_size) {
      word_compression_free((void **)&dictionary_array.list,
                            dictionary_array.size);
      return word_compression_error(
          WORD_COMPRESSION_ERROR_CORRUPTION,
          "the program did not write as many bytes as "
          "needed, remaining %ld for %ld position",
          index_size, dictionary_array.position);
    }

    word_compression_tree_push(position, &new_list, new_list, 0);
  }

  *current_node = new_list;
  word_compressor_free_array(&dictionary_array);
  return WORD_COMPRESSION_SUCCESS;
}