#ifndef WORD_COMPRESSION_DEFINED
#define WORD_COMPRESSION_DEFINED 1

#include "types.h"
#include "wrappers.h"

#define WORD_COMPRESSION_FORMAT "%s\t%s\t%ld\n"
short print_format(WordCompressionNode **, void *, short *);
short word_compressor_dict_line(DictionaryArray *, char **,
                                unsigned long *);
void word_compression_free_arguments(WordCompressionArguments *);
short word_compressor_append_string(char *, unsigned long, unsigned long,
                                    WordCompressionArguments *);
unsigned long word_compressor(char *, unsigned long, unsigned long, void **,
                              short, short *);
unsigned long word_decompressor(char *, unsigned long, unsigned long, void **,
                                short, short *);
#endif