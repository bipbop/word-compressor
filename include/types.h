#ifndef WORD_COMPRESSION_TYPES_DEFINED
#define WORD_COMPRESSION_TYPES_DEFINED 1

#include "wrappers.h"

typedef struct WordCompressionNodeStruct {
  char *index;
  char *value;
  unsigned long occurrences;
  struct WordCompressionNodeStruct *left;
  struct WordCompressionNodeStruct *right;
  struct WordCompressionNodeStruct *parent;
} WordCompressionNode;

typedef struct {
  WordCompressionNode **list;
  unsigned long size;
  unsigned long position;
  unsigned long length;
} DictionaryArray;

typedef struct {
  char *str;
  unsigned long size;
  unsigned long dictionary_length;
  WordCompressionNode *dictionary;
  WordCompressionNode *index_dictionary;
  WordCompressionNode *local_dictionary;
} WordCompressionArguments;

#endif