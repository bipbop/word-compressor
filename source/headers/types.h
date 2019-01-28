#ifndef WORD_COMPRESSION_TYPES_DEFINED
#define WORD_COMPRESSION_TYPES_DEFINED 1

#include <stddef.h>

typedef struct WordCompressionNodeStruct {
  char *index;
  char *value;
  unsigned long occurrences;
  struct WordCompressionNodeStruct *left;
  struct WordCompressionNodeStruct *right;
  struct WordCompressionNodeStruct *parent;
} WordCompressionNode;

typedef struct {
  char *str;
  unsigned long str_size;
  unsigned long dictionary_size;
  WordCompressionNode *dictionary;
  WordCompressionNode *local_dictionary;
} WordCompressionArguments;

#endif