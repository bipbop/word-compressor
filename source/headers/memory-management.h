#ifndef word_compressor_memory_management_register
#define word_compressor_memory_management_register 1

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
#define MEMORY_TABLE_LENGTH 3000
#endif

void get_memory_table();
void *word_compressor_malloc(unsigned long);
void *word_compressor_realloc(void **, unsigned long *, unsigned long);
void word_compressor_free(void **, unsigned long);
void word_compressor_free_string(char **);
unsigned long word_compressor_allocated();

#endif