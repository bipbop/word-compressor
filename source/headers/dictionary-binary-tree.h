#ifndef WORD_COMPRESSOR_DICTIONARY_BINARY_TREE_DEFINED
#define WORD_COMPRESSOR_DICTIONARY_BINARY_TREE_DEFINED 1

#include "./types.h"

void word_compressor_dictionary_binary_tree_free(DictionaryNode **,
                                                 unsigned short);
void word_compressor_dictionary_binary_tree_delete(DictionaryNode **,
                                                   unsigned short);
DictionaryNode *word_compressor_dictionary_binary_tree_base(DictionaryNode *);
DictionaryNode *word_compressor_dictionary_binary_tree_insert(char *key,
                                                              DictionaryNode **,
                                                              DictionaryNode *,
                                                              unsigned short *);
DictionaryNode *word_compressor_dictionary_binary_tree_push(DictionaryNode *,
                                                            DictionaryNode **,
                                                            DictionaryNode *);

DictionaryNode *
word_compressor_dictionary_binary_tree_search(char *key, DictionaryNode *leaf);

#endif