#include <ctype.h>
#include <utf8proc.h>

#include "codes.h"
#include "decoder.h"

utf8proc_ssize_t last_decoder_error = 0;

long word_compression_get_last_decoder_error() {
  return (long)last_decoder_error;
}

void word_compression_set_last_decoder_error(utf8proc_ssize_t error) {
  if (error >= 0)
    return;
  last_decoder_error = error;
}

short word_compression_utf8alnum(char *str) {
  int alphanum = 0;

  utf8proc_uint8_t *output = NULL;
  utf8proc_ssize_t len =
      utf8proc_map((utf8proc_uint8_t *)str, 1, &output,
                   UTF8PROC_NULLTERM | UTF8PROC_STABLE | UTF8PROC_STRIPMARK |
                       UTF8PROC_DECOMPOSE | UTF8PROC_CASEFOLD);
  if (len < 0) {

    if (output != NULL) {
      free(output);
      output = NULL;
    }
    last_decoder_error = len;
    return WORD_COMPRESSION_ERROR_ENCODE;
  }

  if (output != NULL) {
    alphanum = isalnum(output[0]) ? 1 : 0;
    free(output); /* utf8proc use malloc */
    output = NULL;
  }

  return alphanum;
}

long word_compression_utf8iterate(char *str, unsigned long size,
                                  int *codepoint_ref) {
  utf8proc_ssize_t n =
      utf8proc_iterate((utf8proc_uint8_t *)str, (utf8proc_ssize_t)size,
                       (utf8proc_int32_t *)&codepoint_ref);
  if (n < 0) {
    last_decoder_error = n;
    return word_compression_error(
        WORD_COMPRESSION_ERROR_ENCODE,
        "we got an invalid UTF-8 string during iterating process");
  }
  return n;
}