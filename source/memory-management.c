#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./headers/memory-management.h"

unsigned long used_bytes = 0;
void *memory_table[MEMORY_TABLE_LENGTH] = {0};
unsigned long memory_table_index = 0;

void get_memory_table() {
#ifdef DEBUG
#if DEBUG == 1
  for (int i = 0; i < MEMORY_TABLE_LENGTH; i++) {
    if (memory_table[i] == NULL)
      continue;
    printf("%p\n", memory_table[i]);
  }
#endif
#endif
}

unsigned long word_compression_allocated() { return used_bytes; }

void *word_compression_realloc(void **ptr, unsigned long *old_size,
                               unsigned long new_bytes) {
  void *data = *ptr == NULL ? app_malloc(new_bytes)
                            : app_realloc(*ptr, *old_size + new_bytes);

  if (data != NULL) {

#ifdef DEBUG
#if DEBUG == 1
    for (int i = 0; i < MEMORY_TABLE_LENGTH; i++) {
      if (memory_table[i] == *ptr) {
        memory_table[i] = data;
        break;
      }
    }
#endif
#endif

    *ptr = data;
    bzero(data + *old_size, new_bytes);
    *old_size += new_bytes;
    used_bytes += new_bytes;
  }

  return data;
}

void *word_compression_malloc(unsigned long size) {
  void *data = app_malloc(size);
  bzero(data, size);
  used_bytes += size;
#ifdef DEBUG
#if DEBUG == 1
  memory_table[memory_table_index++ % MEMORY_TABLE_LENGTH] = data;
#endif
#endif

  return data;
}

void word_compression_free(void **ptr, unsigned long size) {
  if (*ptr == NULL)
    return;
  app_free(*ptr);

#ifdef DEBUG
#if DEBUG == 1
  for (int i = 0; i < MEMORY_TABLE_LENGTH; i++) {
    if (memory_table[i] == *ptr) {
      memory_table[i] = NULL;
      break;
    }
  }
#endif
#endif

  *ptr = NULL;
  used_bytes -= size;
}

void word_compression_free_string(char **ptr) {
  if (*ptr == NULL)
    return;
  unsigned long size = strlen(*ptr) + 1;
  word_compression_free((void **)ptr, size);
}