
#include <stdio.h>
#include <string.h>

#include "word-compressor.h"

char * word_compressor_string(char *dict, char *buffer, short *error) {
    FILE *stream;
    char *response;
    stream = fmemopen(buffer, strlen (buffer), "r");
    response = word_compressor_file(dict, stream, error);
    fclose(stream);
    return response;
}

char * word_decompressor_string(char *dict, char *buffer, short *error) {
    FILE *stream;
    char *response;
    stream = fmemopen(buffer, strlen (buffer), "r");
    response = word_decompressor_file(dict, stream, error);
    fclose(stream);
    return response;
}