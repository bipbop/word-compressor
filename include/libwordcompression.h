#ifndef LIBWORDCOMPRESSION_DEFINED
#define LIBWORDCOMPRESSION_DEFINED 1

#include "wrappers.h"
#include "liberrors.h"
#include "version.h"
#include "cache.h"

short word_compressor_create_dictionary(WC_FILE *, WC_FILE *, unsigned long);
char *word_compressor_file(char *, WC_FILE *, short *);
char *word_decompressor_file(char *, WC_FILE *, short *);
char *word_compressor_string(char *, char *, short *);
char *word_decompressor_string(char *, char *, short *);

#endif