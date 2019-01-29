#ifndef WORD_COMPRESSION_WORD_READER_REGISTER
#define WORD_COMPRESSION_WORD_READER_REGISTER 1

#include "./wrappers.h"
#include "./callbacks.h"

unsigned long word_compression_reader(char *, unsigned long,
                                      WordCompressionCallbackReader, void *,
                                      short *);

#endif