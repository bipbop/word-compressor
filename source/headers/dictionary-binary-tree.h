#ifndef word_compressor_dictionary_binary_tree_defined
#define word_compressor_dictionary_binary_tree_defined 1

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