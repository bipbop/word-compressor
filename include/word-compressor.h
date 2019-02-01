#ifndef WORD_COMPRESSION_DEFINED
#define WORD_COMPRESSION_DEFINED 1

#include "types.h"
#include "wrappers.h"

#define WORD_COMPRESSION_FORMAT "%s\t%s\t%ld\n"

char *word_compressor_file(char *, WC_FILE *, short *);
char *word_decompressor_file(char *, WC_FILE *, short *);
void word_compression_free_arguments(WordCompressionArguments *);
short word_compressor_create_dictionary(WC_FILE *, WC_FILE *, unsigned long);
short word_compressor_append_string(char *, unsigned long, unsigned long,
                                    WordCompressionArguments *);

#endif