#ifndef WORD_COMPRESSION_MEMORY_MANAGEMENT_REGISTER
#define WORD_COMPRESSION_MEMORY_MANAGEMENT_REGISTER 1

/* Use your malloc and free functions */

#include <stdlib.h>

#ifndef app_malloc
#define app_malloc malloc
#endif

#ifndef app_free
#define app_free free
#endif

#ifndef app_realloc
#define app_realloc realloc
#endif

#ifndef MEMORY_TABLE_LENGTH
#define MEMORY_TABLE_LENGTH 300000
#endif

void get_memory_table();
void *word_compression_malloc(unsigned long);
void *word_compression_realloc(void **, unsigned long *, unsigned long);
void word_compression_free(void **, unsigned long);
void word_compression_free_string(char **);
unsigned long word_compression_allocated();

#endif