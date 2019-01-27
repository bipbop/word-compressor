#ifndef word_compressor_decoder_defined
#define word_compressor_decoder_defined 1

short word_compressor_utf8alnum(char *);
long word_compressor_get_last_decoder_error();
long word_compressor_utf8iterate(char *, unsigned long, int *);

#endif