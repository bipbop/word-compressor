#ifndef WORD_COMPRESSION_CACHE_DEFINED
#define WORD_COMPRESSION_CACHE_DEFINED 1

#include "types.h"

unsigned long word_compression_cache(const char *path,
                                     WordCompressionNode **node,
                                     short index_value, short *error);

unsigned long word_compression_create_cache(const char *path,
                                            WordCompressionNode **node,
                                            short index_value, short *error);
void word_compression_cache_flush();

#endif