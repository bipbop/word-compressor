#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "codes.h"
#include "decoder.h"
#include "memory-management.h"

char message_alloc[] = "no memory";
char message_invalid_encode[] = "invalid encode";
char message_default_error[] = "(lib-word-compression) an internal library "
                               "error has occurred, contact the developer";
char success[] = "\0";

short last_error = WORD_COMPRESSION_SUCCESS;
char last_error_message[256] = "\0";
time_t last_error_at = 0;

char *default_error_message(short code) {
  switch (code) {
  case WORD_COMPRESSION_SUCCESS:
    return success;
  case WORD_COMPRESSION_ERROR_ALLOC:
    return message_alloc;
  case WORD_COMPRESSION_ERROR_ENCODE:
    return message_invalid_encode;
  }
  return message_default_error;
}

time_t word_compression_last_error_time() { return last_error_at; }

short word_compression_last_error_code() { return last_error; }

char *word_compression_last_error() {
  if (last_error_message[0] == '\0') {
    return default_error_message(last_error);
  }
  return last_error_message;
}

short word_compression_error(short code, char *format, ...) {
  va_list args;

  if (code > 0)
    return code;

  last_error = code;
  bzero(last_error_message, sizeof(last_error_message));
  time(&last_error_at);

  if (format == NULL) {
    if (code == WORD_COMPRESSION_ERROR_STDIO) {
      return word_compression_error(code, "stdio error - %d: %s", errno,
                                    strerror(errno));
    }
    return code;
  }

  va_start(args, format);
  vsnprintf(last_error_message, sizeof(last_error_message), format, args);
  va_end(args);
  return code;
}