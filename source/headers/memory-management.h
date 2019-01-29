#ifndef WORD_COMPRESSION_MEMORY_MANAGEMENT_REGISTER
#define WORD_COMPRESSION_MEMORY_MANAGEMENT_REGISTER 1

/* Use your malloc and free functions */

#include "./wrappers.h"

void get_memory_table();
void *word_compression_malloc(unsigned long);
void *word_compression_realloc(void **, unsigned long *, unsigned long);
void word_compression_free(void **, unsigned long);
void word_compression_free_string(char **);
unsigned long word_compression_allocated();

#endif