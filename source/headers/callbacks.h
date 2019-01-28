#ifndef WORD_COMPRESSOR_CALLBACKS_DEFINED
#define WORD_COMPRESSOR_CALLBACKS_DEFINED 1

#include "./types.h"
#include <stddef.h>

typedef short (*CallbackNode)(DictionaryNode **, void *, short *);
typedef long (*Callback)(char *, unsigned long, unsigned long, void **, short,
                         short *);

#endif