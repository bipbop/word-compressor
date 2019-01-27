#ifndef word_compressor_callbacks_defined
#define word_compressor_callbacks_defined 1

#include "./types.h"
#include <stddef.h>

typedef short (*CallbackNode)(DictionaryNode **, void *, short *);
typedef long (*Callback)(char *, unsigned long, unsigned long, void **, short,
                         short *);

#endif