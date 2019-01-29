#ifndef WORD_COMPRESSION_CALLBACKS_DEFINED
#define WORD_COMPRESSION_CALLBACKS_DEFINED 1

#include <stddef.h>

#include "./types.h"
#include "./wrappers.h"

typedef short (*WordCompressionCallback)(WordCompressionNode **, void *,
                                         short *);
typedef unsigned long (*WordCompressionCallbackReader)(char *, unsigned long,
                                                       unsigned long, void **,
                                                       short, short *);

#endif