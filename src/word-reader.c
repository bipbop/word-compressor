#include <stdio.h>
#include <time.h>

#include "word-reader.h"
#include "callbacks.h"
#include "codes.h"
#include "decoder.h"
#include "types.h"

typedef struct ReadWordsStruct {
  void *callback_parameters;
  short *error;
  char *text;
  char *ptr_text;
  short alphanum;
  unsigned long words;
  unsigned long total_size;
  unsigned long begin_size;
  unsigned long size;
  WordCompressionCallbackReader callback;
} WordReaderInstance;

void iterate_text_reader(WordReaderInstance *this, short *error, short force) {
  short alphanum = word_compression_utf8alnum(this->ptr_text);
  unsigned end_size;

  if (alphanum < 0) {
    *error = word_compression_error(alphanum, "got an invalid UTF-8 data");
    return;
  }

  if (!force) {
    if (alphanum == this->alphanum) {
      return;
    }
    this->alphanum = alphanum;
  }

  end_size = this->total_size - this->size;

  if (end_size != this->begin_size) {
    this->words +=
        this->callback(this->text, this->begin_size, end_size,
                       this->callback_parameters, !this->alphanum, error);
  }

  this->begin_size = end_size;
}

unsigned long word_compression_reader(char *text, unsigned long total_size,
                                      WordCompressionCallbackReader callback,
                                      void *callback_parameters, short *error) {
  WordReaderInstance this = {.text = text,
                             .total_size = total_size,
                             .callback = callback,
                             .callback_parameters = callback_parameters,
                             .ptr_text = text,
                             .begin_size = 0,
                             .words = 0,
                             .size = total_size,
                             .alphanum = -1};

  unsigned long n = 0;
  int data = 0;

  *error = WORD_COMPRESSION_SUCCESS;

  if (text == NULL)
    return 0;

  while ((n = word_compression_utf8iterate(this.ptr_text, this.size, &data)) !=
             0 &&
         *error == WORD_COMPRESSION_SUCCESS) {

    if (*this.ptr_text == '\0')
      break;
    iterate_text_reader(&this, error, 0);
    this.size -= n;
    this.ptr_text += n;
  }

  if (*error == WORD_COMPRESSION_SUCCESS) {
    this.alphanum = !this.alphanum;
    iterate_text_reader(&this, error, 1);
  }

  return this.words;
}
