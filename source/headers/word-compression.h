#ifndef WORD_COMPRESSION_LIB_DEFINED
#define WORD_COMPRESSION_LIB_DEFINED 1

#define WORD_COMPRESSION_MAJOR_VER 1
#define WORD_COMPRESSION_MINOR_VER 0
#define WORD_COMPRESSION_SUBMINOR_VER 0

#include "./callbacks.h"
#include "./codes.h"
#include "./create-index.h"
#include "./decoder.h"
#include "./dictionary-binary-tree.h"
#include "./dictionary-compare.h"
#include "./dictionary-generator.h"
#include "./dictionary-iterator.h"
#include "./dictionary.h"
#include "./file.h"
#include "./filter.h"
#include "./memory-management.h"
#include "./pinch-string.h"
#include "./position-index.h"
#include "./types.h"
#include "./word-compressor.h"
#include "./word-reader.h"
#include "./wrappers.h"

#define WORD_COMPRESSION_APP_NAME "word-compression"
#define WORD_COMPRESSION_FILTER "2"
#define WORD_COMPRESSION_EXTENSION ".dct"

const char *word_compression_version();

#endif