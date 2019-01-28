#ifndef WORD_COMPRESSION_DICTIONARY_GENERATOR_REGISTER
#define WORD_COMPRESSION_DICTIONARY_GENERATOR_REGISTER 1

#include "./types.h"
#include <stdio.h>

unsigned long word_compression_generate_dictionary_file(WordCompressionNode **,
                                                        FILE *, short *);

unsigned long word_compression_generate_dictionary(WordCompressionNode **,
                                                   char *, unsigned long,
                                                   short *);

#endif