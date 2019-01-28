#ifndef WORD_COMPRESSOR_DICTIONARY_GENERATOR_REGISTER
#define WORD_COMPRESSOR_DICTIONARY_GENERATOR_REGISTER 1

#include "./types.h"

unsigned long word_compressor_generate_dictionary_file(DictionaryNode **,
                                                       FILE *, short *);

unsigned long word_compressor_generate_dictionary(DictionaryNode **, char *,
                                                  unsigned long, short *);

#endif