#include <stdio.h>
#include <string.h>

#include "./headers/callbacks.h"
#include "./headers/codes.h"
#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary.h"
#include "./headers/file.h"
#include "./headers/memory-management.h"
#include "./headers/types.h"
#include "./headers/word-reader.h"

unsigned read_file(FILE *fp, WordCompressionNode **node, short indexValue,
                   short *error) {
  char buffer[1024] = {0};
  char *name = NULL;
  char *index = NULL;
  char *ocurrences = NULL;
  char *token = "\t";
  char *temp = NULL;
  unsigned long words = 0;

  WordCompressionNode *current_node = NULL;
  WordCompressionNode *next_node = NULL;

  while ((fgets(buffer, sizeof(buffer), fp)) != NULL) {
    /* get the first token */
    temp = strtok(buffer, token);
    name = word_compression_malloc((strlen(temp) + 1) * sizeof(char));
    if (name == NULL) {
      *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
      return words;
    }
    strcpy(name, temp);

    temp = strtok(NULL, token);
    index = word_compression_malloc((strlen(temp) + 1) * sizeof(char));
    if (name == NULL) {
      word_compression_free_string(&name);
      *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
      return 0;
    }
    strcpy(index, temp);

    ocurrences = strtok(NULL, token);

    next_node =
        word_compression_dictionary(name, index, NULL, atoi(ocurrences));
    if (next_node == NULL) {
      word_compression_free_string(&name);
      word_compression_free_string(&index);
      *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
      return words;
    }

    word_compression_dictionary_binary_tree_push(next_node, &current_node,
                                                 current_node, indexValue);
    name = NULL;
    index = NULL;
    next_node = NULL;
    words++;
  }

  if (ferror(fp)) {
    word_compression_dictionary_free(&current_node, 1, 1);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    return words;
  }

  *node = current_node;
  return words;
}

unsigned long word_compression_open(const char *path, WordCompressionNode **node,
                               short indexValue, short *error) {
  unsigned words;

  if (path == NULL) {
    *error = word_compression_error(
        WORD_COMPRESSION_ERROR_NULL_POINTER,
        "null pointer received at word-compression-open");
    return 0;
  }

  FILE *fp = fopen(path, "r");
  if (fp == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    return 0;
  }

  words = read_file(fp, node, indexValue, error);
  fclose(fp);

  return words;
}

unsigned long word_compression_file(FILE *fp, WordCompressionCallbackReader callback,
                               void *callback_parameters, short *error) {
  unsigned long words = 0;
  char *fullstring = NULL;
  char buffer[1024] = {0};
  unsigned long current_size = 0;
  unsigned numbytes = 0;
  unsigned bytes = 0;

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    if (!current_size) {
      fullstring = word_compression_malloc(sizeof(buffer));
      if (fullstring == NULL) {
        return word_compression_error(WORD_COMPRESSION_ERROR_ALLOC,
                                      "can't allocate %ld bytes",
                                      sizeof(buffer));
      }
      current_size += sizeof(buffer);
    } else if (word_compression_realloc((void **)&fullstring, &current_size,
                                        sizeof(buffer)) == NULL) {
      word_compression_free((void **)&fullstring, current_size);
      return word_compression_error(WORD_COMPRESSION_ERROR_ALLOC,
                                    "can't reallocate %ld bytes as using %ld",
                                    current_size, sizeof(buffer));
    }

    bytes = strlen(buffer);
    if ((numbytes + bytes) > current_size) {
      return word_compression_error(
          WORD_COMPRESSION_ERROR_LOGICAL,
          "number of bytes to write are greather than allocated memory");
    }

    memcpy(fullstring + numbytes, buffer, bytes);
    numbytes += bytes;

    if (buffer[bytes - 1] == '\n') {
      words += word_compression_reader(fullstring, numbytes, callback,
                                       callback_parameters, error);
      word_compression_free((void **)&fullstring, current_size);
      if (*error != WORD_COMPRESSION_SUCCESS) {
        return words;
      }
      current_size = 0;
      numbytes = 0;
    }

    bzero(buffer, sizeof(buffer));
  }

  if (ferror(fp)) {
    word_compression_free((void **)&fullstring, current_size);
  }

  if (numbytes) {
    words += word_compression_reader(fullstring, numbytes, callback,
                                     callback_parameters, error);
    word_compression_free((void **)&fullstring, current_size);
  }

  return words;
}