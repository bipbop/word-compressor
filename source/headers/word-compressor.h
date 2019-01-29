#ifndef WORD_COMPRESSION_DEFINED
#define WORD_COMPRESSION_DEFINED 1

#define WORD_COMPRESSION_FORMAT "%s\t%s\t%ld\n"

unsigned long word_compressor(char *, unsigned long, unsigned long, void **,
                              short, short *);
short word_compressor_append_string(char *, unsigned long, unsigned long,
                                    WordCompressionArguments *);
char *word_compressor_file(char *, FILE *, short *);
void word_compression_free_arguments(WordCompressionArguments *);

#endif