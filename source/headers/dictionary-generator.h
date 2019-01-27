#ifndef word_compressor_dictionary_register
#define word_compressor_dictionary_register 1

#include "./types.h"

unsigned long word_compressor_generate_dictionary_file(DictionaryNode **,
                                                       FILE *, short *);

unsigned long word_compressor_generate_dictionary(DictionaryNode **, char *,
                                                  unsigned long, short *);

#endif