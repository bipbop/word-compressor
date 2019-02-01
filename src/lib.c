#include <stdio.h>
#include <string.h>

#include "word-compression.h"

char lib_version[10] = {0};

const char *word_compression_version() {
  if (lib_version[0] == '\0')
    snprintf(lib_version, sizeof(lib_version), "%d.%d.%d",
             WORD_COMPRESSION_MAJOR_VER, WORD_COMPRESSION_MINOR_VER,
             WORD_COMPRESSION_SUBMINOR_VER);
  return lib_version;
}
