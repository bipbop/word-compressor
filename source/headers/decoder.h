#ifndef WORD_COMPRESSION_DECODER_DEFINED
#define WORD_COMPRESSION_DECODER_DEFINED 1

short word_compression_utf8alnum(char *);
long word_compression_get_last_decoder_error();
long word_compression_utf8iterate(char *, unsigned long, int *);

#endif