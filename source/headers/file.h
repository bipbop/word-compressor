#ifndef WORD_COMPRESSION_FILE_DEFINED
#define WORD_COMPRESSION_FILE_DEFINED 1

#include "./callbacks.h"

unsigned long word_compression_file(FILE *, WordCompressionCallbackReader, void *,
                               short *);
unsigned long word_compression_open(const char *, WordCompressionNode **, short,
                               short *);

#endif