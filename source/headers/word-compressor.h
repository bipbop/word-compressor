#ifndef WORD_COMPRESSION_DEFINED
#define WORD_COMPRESSION_DEFINED 1

#define WORD_COMPRESSION_FORMAT "%s\t%s\t%ld\n"

unsigned long word_compressor(char *, unsigned long, unsigned long, void **,
                              short, short *);
char *word_compressor_file(char *, char *, short *);

#endif