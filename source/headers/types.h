#ifndef word_compressor_types_defined
#define word_compressor_types_defined 1

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