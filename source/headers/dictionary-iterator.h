#ifndef WORD_COMPRESSION_ITERATOR_REGISTER
#define WORD_COMPRESSION_ITERATOR_REGISTER 1

#include "./callbacks.h"
#include "./types.h"

short word_compression_dictionary_iterator(WordCompressionNode **,
                                           WordCompressionCallback, void *,
                                           short *);

#endif