#ifndef WORD_COMPRESSION_DEFINED
#define WORD_COMPRESSION_DEFINED 1

unsigned long word_compressor(char *, unsigned long, unsigned long, void **, short, short *);
char * word_compressor_dict(char *, char *, short *);

#endif