#ifndef word_compressor_codes_defined
#define word_compressor_codes_defined 1

#define WORD_COMPRESSOR_ERROR_LOGICAL -6
#define WORD_COMPRESSOR_ERROR_CORRUPTION -5
#define WORD_COMPRESSOR_ERROR_EMPTY_POSITION -4
#define WORD_COMPRESSOR_ERROR_ENCODE -3
#define WORD_COMPRESSOR_ERROR_SIZE -2
#define WORD_COMPRESSOR_ERROR_ALLOC -1
#define WORD_COMPRESSOR_SUCCESS 0

#include <time.h>

char *word_compressor_last_error();
short word_compressor_error(short, char *, ...);
time_t word_compressor_last_error_time();
short word_compressor_last_error_code();

#endif