#include <string.h>

#include "codes.h"
#include "position-index.h"

char item_list[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

void reverse_string(char *str) {
  int len = strlen(str);
  int i = 0;
  char temp = '\0';
  for (i = 0; i < len / 2; i++) {
    temp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = temp;
  }
}

#define word_compression_has_input res_length != 0 && res != NULL
#define has_index                                                              \
  if (res_length < index)                                                      \
  return WORD_COMPRESSION_ERROR_SIZE

short word_compression_position_index(char *res, unsigned long res_length,
                                      unsigned long position) {
  short index = 0;
  unsigned long itemListLength = sizeof(item_list) - 1;

  do {
    if (word_compression_has_input) {
      has_index;
      res[index] = item_list[(position % itemListLength)];
    }
    position /= itemListLength;
    index++;
  } while (position--);

  if (word_compression_has_input) {
    has_index;
    res[index] = '\0';
    reverse_string(res);
  }

  return index + 1;
}