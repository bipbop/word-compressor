#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./headers/memory-management.h"

unsigned long used_bytes = 0;

void *word_compressor_realloc(void **ptr, unsigned long *old_size,
                              unsigned long new_bytes) {
  void *data = app_realloc(*ptr, *old_size + new_bytes);
  bzero(data + *old_size, new_bytes);

  if (data != NULL) {
    *ptr = data;
    *old_size += new_bytes;
    used_bytes += new_bytes;
  }
  return data;
}

void *word_compressor_malloc(unsigned long size) {
  void *data = app_malloc(size);
  bzero(data, size);
  used_bytes += size;
  return data;
}

void word_compressor_free(void **ptr, unsigned long size) {
  app_free(*ptr);
  *ptr = NULL;
  used_bytes -= size;
}

unsigned long word_compressor_allocated() { return used_bytes; }

void word_compressor_free_string(char **ptr) {
  if (*ptr == NULL)
    return;
  unsigned long size = strlen(*ptr) + 1;
  word_compressor_free((void **)ptr, size);
}