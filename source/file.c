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
#include "./headers/decoder.h"

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
    word_compression_free_dictionary(&current_node, 1, 1);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    return words;
  }

  *node = current_node;
  return words;
}

unsigned long word_compression_open(const char *path,
                                    WordCompressionNode **node,
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

unsigned long word_compression_file(FILE *fp,
                                    WordCompressionCallbackReader callback,
                                    void *callback_parameters, short *error) {
  unsigned long words = 0;

  char *linebuffer = NULL;
  char buffer[WORD_COMPRESSOR_BUFFER_SIZE] = {0};

  unsigned long current_size = 1;
  unsigned long buffer_length = 0;
  unsigned long bytes = 0;

  while (!feof(fp)) {
    bytes = fread((void *)buffer, sizeof(char), WORD_COMPRESSOR_BUFFER_SIZE - 1, fp);
    if (bytes == 0) break;
    
    buffer_length = strlen(buffer);
    if (word_compression_realloc((void **)&linebuffer, &current_size,
                                 buffer_length) == NULL) {
      word_compression_free((void **)&linebuffer, current_size);
      current_size = 0;
      return word_compression_error(
          WORD_COMPRESSION_ERROR_ALLOC,
          "can't reallocate %ld buffer_length as using %ld", current_size,
          sizeof(buffer));
    }

    strcat(linebuffer, buffer);
    if (!word_compression_utf8alnum(buffer + ((buffer_length - 1) * sizeof(char)))) {
      words += word_compression_reader(linebuffer, current_size, callback,
                                       callback_parameters, error);
      word_compression_free((void **)&linebuffer, current_size);
      current_size = 1;

      if (*error != WORD_COMPRESSION_SUCCESS) {
        return words;
      }
    }

    bzero(buffer, sizeof(buffer));
  }

  if (ferror(fp)) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    word_compression_free((void **)&linebuffer, current_size);
    current_size = 1;
  }

  if (current_size) {
    words += word_compression_reader(linebuffer, current_size, callback,
                                     callback_parameters, error);
    word_compression_free((void **)&linebuffer, current_size);
    current_size = 1;
  }

  return words;
}