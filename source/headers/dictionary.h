#ifndef WORD_COMPRESSOR_DICTIONARY_REGISTER
#define WORD_COMPRESSOR_DICTIONARY_REGISTER 1

#include "./types.h"

DictionaryNode *word_compressor_dictionary(char *, DictionaryNode *,
                                           unsigned long);
void word_compressor_dictionary_free(DictionaryNode **, unsigned short,
                                     unsigned short);

#endif