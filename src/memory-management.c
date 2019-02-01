#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory-management.h"

unsigned long used_bytes = 0;
void *memory_table[MEMORY_TABLE_LENGTH] = {0};
unsigned long memory_table_size[MEMORY_TABLE_LENGTH] = {0};
unsigned long memory_table_index = 0;

void get_memory_table() {
#ifdef DEBUG
#if DEBUG == 1
  for (int i = 0; i < MEMORY_TABLE_LENGTH; i++) {
    if (memory_table[i] == NULL)
      continue;
    fprintf(stderr, "%p\n", memory_table[i]);
    fflush(stderr);
  }
#endif
#endif
}

unsigned long word_compression_allocated() { return used_bytes; }

void *word_compression_realloc(void **ptr, unsigned long *old_size,
                               unsigned long new_bytes) {
  void *data = NULL;
  if (*ptr == NULL) {
    *old_size = (*old_size) + new_bytes;
    *ptr = word_compression_malloc(*old_size);
    return *ptr;
  }

  data = WC_REALLOC(*ptr, (*old_size) + new_bytes);

  if (data != NULL) {

#ifdef DEBUG
#if DEBUG == 1
    for (int i = 0; i < MEMORY_TABLE_LENGTH; i++) {
      if (memory_table[i] == *ptr) {
        memory_table[i] = data;
        memory_table_size[i] += new_bytes;
        break;
      }
    }
#endif
#endif

    *ptr = data;
    bzero(data + (*old_size), new_bytes);
    *old_size += new_bytes;
    used_bytes += new_bytes;
  }

  return data;
}

void *word_compression_malloc(unsigned long size) {
  void *data = WC_MALLOC(size);
  bzero(data, size);
  used_bytes += size;
#ifdef DEBUG
#if DEBUG == 1
  unsigned long i = memory_table_index++ % MEMORY_TABLE_LENGTH;
  memory_table[i] = data;
  memory_table_size[i] = size;
#endif
#endif
  return data;
}

void word_compression_free(void **ptr, unsigned long size) {
  if (*ptr == NULL)
    return;

#ifdef DEBUG
#if DEBUG == 1
  for (int i = 0; i < MEMORY_TABLE_LENGTH; i++) {
    if (memory_table[i] == *ptr) {
      memory_table_size[i] -= size;
      if (memory_table_size[i]) {
        printf("\nIncorrect freeing, rest is %ld\n",
               (unsigned long)memory_table_size[i]);
        return;
      }
      memory_table[i] = NULL;

      break;
    }
  }
#endif
#endif

  WC_FREE(*ptr);
  *ptr = NULL;
  used_bytes -= size;
}

void word_compression_free_string(char **ptr) {
  if (*ptr == NULL)
    return;
  unsigned long size = strlen(*ptr) + 1;
  word_compression_free((void **)ptr, size);
}