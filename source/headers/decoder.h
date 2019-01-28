#ifndef WORD_COMPRESSOR_DECODER_DEFINED
#define WORD_COMPRESSOR_DECODER_DEFINED 1

short word_compressor_utf8alnum(char *);
long word_compressor_get_last_decoder_error();
long word_compressor_utf8iterate(char *, unsigned long, int *);

#endif