#ifndef WORD_COMPRESSOR_TYPES_DEFINED
#define WORD_COMPRESSOR_TYPES_DEFINED 1

#include <stddef.h>

typedef struct DictionaryNodeStruct {
  char *index;
  char *value;
  unsigned long occurrences;
  struct DictionaryNodeStruct *left;
  struct DictionaryNodeStruct *right;
  struct DictionaryNodeStruct *parent;
} DictionaryNode;

#endif