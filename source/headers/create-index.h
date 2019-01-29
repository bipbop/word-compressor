#ifndef WORD_COMPRESSION_CREATE_INDEX_DEFINED
#define WORD_COMPRESSION_CREATE_INDEX_DEFINED 1

#include "./types.h"

void word_compressor_free_array(DictionaryArray *);
short word_compression_array(DictionaryArray *, WordCompressionNode **,
                             unsigned long);
short int word_compression_create_index(WordCompressionNode **, unsigned long);

#endif