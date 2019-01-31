#ifndef WORD_COMPRESSION_TREE_DEFINED
#define WORD_COMPRESSION_TREE_DEFINED 1

#include "types.h"
#include "wrappers.h"

void word_compression_tree_free(WordCompressionNode **, unsigned short);

void word_compression_tree_delete(WordCompressionNode **, unsigned short,
                                  short);
WordCompressionNode *word_compression_tree_base(WordCompressionNode *);
WordCompressionNode *word_compression_tree_insert(char *key,
                                                  WordCompressionNode **,
                                                  WordCompressionNode *,
                                                  unsigned short *);
WordCompressionNode *word_compression_tree_push(WordCompressionNode *,
                                                WordCompressionNode **,
                                                WordCompressionNode *, short);

WordCompressionNode *word_compression_tree_search(char *, WordCompressionNode *,
                                                  short);

#endif