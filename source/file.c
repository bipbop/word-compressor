#include <stdio.h>
#include <string.h>

#include "./headers/callbacks.h"
#include "./headers/codes.h"
#include "./headers/file.h"
#include "./headers/memory-management.h"
#include "./headers/word-reader.h"

unsigned int word_compressor_file(FILE *fp, Callback callback,
                                  void *callback_parameters, short *error) {
  unsigned long words = 0;
  char *fullstring = NULL;
  char buffer[1024] = {0};
  unsigned long current_size = 0;
  unsigned int numbytes = 0;
  unsigned int bytes = 0;

  while (fgets(buffer, sizeof(buffer), fp)) {
    if (!current_size) {
      fullstring = word_compressor_malloc(sizeof(buffer));
      if (fullstring == NULL) {
        return word_compressor_error(WORD_COMPRESSOR_ERROR_ALLOC,
                                     "can't allocate %ld bytes",
                                     sizeof(buffer));
      }
      current_size += sizeof(buffer);
    } else if (word_compressor_realloc((void **)&fullstring, &current_size,
                                       sizeof(buffer)) == NULL) {
      word_compressor_free((void **)&fullstring, current_size);
      return word_compressor_error(WORD_COMPRESSOR_ERROR_ALLOC,
                                   "can't reallocate %ld bytes as using %ld",
                                   current_size, sizeof(buffer));
    }

    bytes = strlen(buffer);
    if ((numbytes + bytes) > current_size) {
      return word_compressor_error(
          WORD_COMPRESSOR_ERROR_LOGICAL,
          "number of bytes to write are greather than allocated memory");
    }

    memcpy(fullstring + numbytes, buffer, bytes);
    numbytes += bytes;

    if (buffer[bytes - 1] == '\n') {
      words += word_compressor_reader(fullstring, numbytes, callback,
                                      callback_parameters, error);
      word_compressor_free((void **)&fullstring, current_size);
      if (*error != WORD_COMPRESSOR_SUCCESS) {
          return words;
      }
      current_size = 0;
      numbytes = 0;
    } 

    bzero(buffer, sizeof(buffer));
  }

  if (numbytes) {
    words += word_compressor_reader(fullstring, numbytes, callback,
                                    callback_parameters, error);
    word_compressor_free((void **)&fullstring, current_size);
  }

  return words;
}