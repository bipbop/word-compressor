#include "./headers/dictionary-compare.h"
#include "./headers/types.h"

int word_compressor_dictionary_compare(const void *a, const void *b) {
  return ((DictionaryNode *)b)->occurrences -
         ((DictionaryNode *)a)->occurrences;
}

int word_compressor_dictionary_compare_reverse(const void *a, const void *b) {
  return word_compressor_dictionary_compare(b, a);
}

int word_compressor_dictionary_compare_ptr(const void *a, const void *b) {
  return word_compressor_dictionary_compare(*((const void **)a),
                                            *((const void **)b));
}

int word_compressor_dictionary_compare_reverse_ptr(const void *a,
                                                   const void *b) {
  return word_compressor_dictionary_compare(*((const void **)b),
                                            *((const void **)a));
}