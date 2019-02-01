#ifndef LIBWORDCOMPRESSION_DEFINED
#define LIBWORDCOMPRESSION_DEFINED 1

#include <stdio.h>
#include "wrappers.h"

short word_compressor_create_dictionary(WC_FILE *, WC_FILE *, unsigned long);
char *word_compressor_file(char *, WC_FILE *, short *);
char *word_decompressor_file(char *, WC_FILE *, short *);
char *word_compressor_string(char *, char *, short *);
char *word_decompressor_string(char *, char *, short *);

#endif