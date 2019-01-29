#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./headers/callbacks.h"
#include "./headers/codes.h"
#include "./headers/decoder.h"
#include "./headers/dictionary-binary-tree.h"
#include "./headers/dictionary.h"
#include "./headers/file.h"
#include "./headers/memory-management.h"
#include "./headers/types.h"
#include "./headers/word-reader.h"

const char *token = "\t";

unsigned long parse_buffer(char *buffer, short *error,
                           WordCompressionNode **current_node,
                           short index_value) {
  char *temp = NULL;
  char *name = NULL;
  char *index = NULL;
  char *ocurrences = NULL;
  WordCompressionNode *next_node = NULL;
  unsigned long words = 0;

  /* get the first token */
  temp = strtok(buffer, token);
  name = word_compression_malloc((strlen(temp) + 1) * sizeof(char));
  if (name == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_CORRUPTION, NULL);
    return words;
  }

  strcpy(name, temp);

  temp = strtok(NULL, token);

  index = word_compression_malloc((strlen(temp) + 1) * sizeof(char));
  if (name == NULL) {
    word_compression_free_string(&name);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_CORRUPTION, NULL);
    return 0;
  }
  strcpy(index, temp);
  ocurrences = strtok(NULL, token);

  if (ocurrences == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_CORRUPTION, NULL);
    return 0;
  }

  next_node = word_compression_dictionary(name, index, NULL, atol(ocurrences));

  if (next_node == NULL) {
    word_compression_free_string(&name);
    word_compression_free_string(&index);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    return 0;
  }

  word_compression_tree_push(next_node, current_node,
                                               *current_node, index_value);
  return 1;
}

unsigned long word_compression_parse_dict(WC_FILE *fp, WordCompressionNode **node,
                                          short index_value, short *error) {
  char buffer[WORD_COMPRESSOR_BUFFER_SIZE] = {0};
  char *ptr_buffer = NULL;
  char *line_buffer = NULL;
  unsigned long words = 0;
  unsigned long readed_bytes = 0;
  unsigned long line_buffer_bytes = 1;
  unsigned long i = 0;
  short block = 0;

  WordCompressionNode *current_node = NULL;

  while (!WC_FEOF(fp) && block < 2 /* \n1\n2\n */) {
    bzero(buffer, sizeof(buffer));
    ptr_buffer = buffer;
    WC_FREAD(buffer, sizeof(char) * (WORD_COMPRESSOR_BUFFER_SIZE - 1), fp);
    readed_bytes = strlen(buffer);
    if (!readed_bytes)
      break;

    for (i = 0;
         i < (WORD_COMPRESSOR_BUFFER_SIZE - 1) && block < 2 /* \n1\n2\n */;
         i++) {
      if (buffer[i] != '\n')
        continue;
      *(buffer + (i * sizeof(char))) = '\0';
      readed_bytes = strlen(ptr_buffer);

      if (!readed_bytes && line_buffer == NULL) {
        ptr_buffer = buffer + ((i + 1) * sizeof(char));
        block++;
        continue;
      }

      block = 0;
      word_compression_realloc((void **)&line_buffer, &line_buffer_bytes,
                               readed_bytes);
      strcat(line_buffer, ptr_buffer);

      words += parse_buffer(line_buffer, error, &current_node, index_value);
      word_compression_free((void **)&line_buffer, line_buffer_bytes);
      line_buffer_bytes = 1;
      ptr_buffer = buffer + ((i + 1) * sizeof(char));
      if ((*error) != WORD_COMPRESSION_SUCCESS) {
        word_compression_free_string(&line_buffer);
        word_compression_free_dictionary(&current_node, 1, 1);
        return 0;
      }
    }

    readed_bytes = strlen(ptr_buffer);
    if (!readed_bytes)
      continue;

    word_compression_realloc((void **)&line_buffer, &line_buffer_bytes,
                             readed_bytes);
    strcat(line_buffer, ptr_buffer);
  }

  if ((*error) != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_dictionary(&current_node, 1, 1);
    return 0;
  }

  if (line_buffer != NULL) {
    if (block >= 2 /* \n1\n2\n */) {
      if (WC_FSEEK(fp, (line_buffer_bytes * -1) + 1, SEEK_CUR) != 0) {
        *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
        word_compression_free_dictionary(&current_node, 1, 1);
        word_compression_free_string(&line_buffer);
        line_buffer_bytes = 1;
        return 0;
      }
    } else {
      words += parse_buffer(line_buffer, error, &current_node, index_value);
    }
  }

  word_compression_free_string(&line_buffer);
  line_buffer_bytes = 1;

  if (ferror(fp)) {
    word_compression_free_dictionary(&current_node, 1, 1);
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    return 0;
  }

  if ((*error) != WORD_COMPRESSION_SUCCESS) {
    word_compression_free_dictionary(&current_node, 1, 1);
    return 0;
  }

  *node = current_node;
  return words;
}

unsigned long word_compression_open(const char *path,
                                    WordCompressionNode **node,
                                    short index_value, short *error) {
  unsigned long words = 0;

  if (path == NULL) {
    *error = word_compression_error(
        WORD_COMPRESSION_ERROR_NULL_POINTER,
        "null pointer received at word-compression-open");
    return 0;
  }

  WC_FILE *fp = WC_FOPEN(path, "r");
  if (fp == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_STDIO, NULL);
    return 0;
  }

  words = word_compression_parse_dict(fp, node, index_value, error);
  WC_FCLOSE(fp);

  return words;
}

unsigned long word_compression_file(WC_FILE *fp,
                                    WordCompressionCallbackReader callback,
                                    void *callback_parameters, short *error) {
  unsigned long words = 0;

  char *linebuffer = NULL;
  char buffer[WORD_COMPRESSOR_BUFFER_SIZE] = {0};

  unsigned long current_size = 1;
  unsigned long buffer_length = 0;
  unsigned long bytes = 0;

  while (!WC_FEOF(fp)) {
    WC_FREAD((void *)buffer, sizeof(char) * (WORD_COMPRESSOR_BUFFER_SIZE - 1), fp);
    bytes = strlen(buffer);
    if (bytes == 0)
      break;

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
    if (word_compression_utf8alnum(buffer + ((buffer_length - 1) * sizeof(char))) == 0) {
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
    return words;
  }

  if (current_size) {
    words += word_compression_reader(linebuffer, current_size, callback,
                                     callback_parameters, error);
    word_compression_free((void **)&linebuffer, current_size);
    current_size = 1;
  }

  return words;
}