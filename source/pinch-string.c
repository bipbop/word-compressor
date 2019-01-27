#include <string.h>

#include "./headers/memory-management.h"
#include "./headers/pinch-string.h"

char *word_compressor_pinch_string(char *text, unsigned long start,
                                   unsigned long end) {
  char *str = NULL;
  if (start > end)
    return NULL;
  end -= start;
  str = (char *)word_compressor_malloc((sizeof(char) * end) + 1);
  bzero(str, (sizeof(char) * end) + 1);
  if (str == NULL)
    return NULL; /* no-memory */
  memcpy(str, text + start, end);
  return str;
}