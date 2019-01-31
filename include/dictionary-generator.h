#ifndef WORD_COMPRESSION_DICTIONARY_GENERATOR_REGISTER
#define WORD_COMPRESSION_DICTIONARY_GENERATOR_REGISTER 1

#include <stdio.h>

#include "types.h"
#include "wrappers.h"

unsigned long word_compression_generate_dictionary_file(WordCompressionNode **,
                                                        WC_FILE *, short *);

unsigned long word_compression_generate_dictionary(WordCompressionNode **,
                                                   char *, unsigned long,
                                                   short *);

#endif