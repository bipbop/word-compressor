#include "./headers/word-reader.h"
#include "./headers/callbacks.h"
#include "./headers/codes.h"
#include "./headers/decoder.h"
#include "./headers/types.h"

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
  Callback callback;
} WordReaderInstance;

void iterate_text_reader(WordReaderInstance *this, short *error) {
  short alphanum = word_compressor_utf8alnum(this->ptr_text);
  unsigned end_size;

  if (alphanum < 0) {
    *error = alphanum;
    return;
  }

  if (alphanum == this->alphanum) {
    return;
  }

  end_size = this->total_size - this->size;
  this->alphanum = alphanum;

  if (end_size != this->begin_size)
    this->words +=
        this->callback(this->text, this->begin_size, end_size,
                       this->callback_parameters, !this->alphanum, error);

  this->begin_size = end_size;
}

unsigned long word_compressor_reader(char *text, unsigned long total_size,
                                     Callback callback,
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

  *error = WORD_COMPRESSOR_SUCCESS;

  if (text == NULL)
    return 0;

  while ((n = word_compressor_utf8iterate(this.ptr_text, this.size, &data)) !=
             0 &&
         *error == WORD_COMPRESSOR_SUCCESS) {
    if (n < 0) {
      *error = n;
      break;
    }

    if (*this.ptr_text == '\0')
      break;
    iterate_text_reader(&this, error);
    this.size -= n;
    this.ptr_text += n;
  }

  if (*error == WORD_COMPRESSOR_SUCCESS) {
    iterate_text_reader(&this, error);
  }

  return this.words;
}
