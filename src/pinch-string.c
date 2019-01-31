#include <string.h>

#include "memory-management.h"
#include "pinch-string.h"

char *word_compression_pinch_string(char *text, unsigned long start,
                                    unsigned long end) {
  char *str = NULL;
  if (start > end)
    return NULL;
  end -= start;
  str = (char *)word_compression_malloc((sizeof(char) * end) + 1);
  if (str == NULL)
    return NULL; /* no-memory */
  memcpy(str, text + start, end);
  return str;
}