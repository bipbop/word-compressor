#ifndef WORD_COMPRESSION_DEFINED
#define WORD_COMPRESSION_DEFINED 1

#define WORD_COMPRESSION_FORMAT "%s\t%s\t%ld\n"

char *word_compressor_file(char *, FILE *, short *);
char *word_decompressor_file(char *, FILE *, short *);
void word_compression_free_arguments(WordCompressionArguments *);
short word_compressor_create_dictionary(FILE *, FILE *, unsigned long);
short word_compressor_append_string(char *, unsigned long, unsigned long,
                                    WordCompressionArguments *);

#endif