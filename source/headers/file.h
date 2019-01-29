#ifndef WORD_COMPRESSION_FILE_DEFINED
#define WORD_COMPRESSION_FILE_DEFINED 1

#include "./callbacks.h"

#ifndef WORD_COMPRESSOR_BUFFER_SIZE
#define WORD_COMPRESSOR_BUFFER_SIZE 64
#endif

unsigned long word_compression_parse_dict(FILE *, WordCompressionNode **, short,
                                          short *);

unsigned long word_compression_file(FILE *, WordCompressionCallbackReader,
                                    void *, short *);

unsigned long word_compression_open(const char *, WordCompressionNode **, short,
                                    short *);

#endif