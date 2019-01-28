#include "./headers/dictionary-compare.h"
#include "./headers/types.h"

int word_compression_dictionary_compare(const void *a, const void *b) {
  return ((WordCompressionNode *)b)->occurrences -
         ((WordCompressionNode *)a)->occurrences;
}

int word_compression_dictionary_compare_reverse(const void *a, const void *b) {
  return word_compression_dictionary_compare(b, a);
}

int word_compression_dictionary_compare_ptr(const void *a, const void *b) {
  return word_compression_dictionary_compare(*((const void **)a),
                                             *((const void **)b));
}

int word_compression_dictionary_compare_reverse_ptr(const void *a,
                                                    const void *b) {
  return word_compression_dictionary_compare(*((const void **)b),
                                             *((const void **)a));
}