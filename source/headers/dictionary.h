#ifndef word_compressor_dictionary_register
#define word_compressor_dictionary_register 1

#include "./types.h"

DictionaryNode *word_compressor_dictionary(char *, DictionaryNode *,
                                           unsigned long);
void word_compressor_dictionary_free(DictionaryNode **, unsigned short,
                                     unsigned short);

#endif