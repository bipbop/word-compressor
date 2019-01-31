#ifndef WORD_COMPRESSION_DICTIONARY_REGISTER
#define WORD_COMPRESSION_DICTIONARY_REGISTER 1

#include "types.h"
#include "wrappers.h"

WordCompressionNode *word_compression_dictionary(char *, char *,
                                                 WordCompressionNode *,
                                                 unsigned long);
void word_compression_free_dictionary(WordCompressionNode **, unsigned short,
                                      unsigned short);

#endif