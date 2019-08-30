#include <codes.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

#include "memory-management.h"
#include "liberrors.h"

#include "types.h"
#include "cache.h"
#include "dictionary.h"

unsigned long cache_size = 0;
unsigned long cache_items = 0;
DictCache **cache_list = NULL;

unsigned long word_compression_create_cache(const char *path,
                                            WordCompressionNode **node,
                                            short index_value, short *error) {
  DictCache *cache = (DictCache *)malloc(sizeof(DictCache));
  unsigned long path_length = strlen(path) * sizeof(char);

  if (!cache) {
    return 0;
  }
  
  cache->path = (char *)word_compression_malloc(path_length + 1);
  if (cache->path == NULL) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    word_compression_free((void **)&cache, sizeof(DictCache));
    return 0;
  }

  memcpy((void *)cache->path, path, path_length);
  
  cache->index_value = index_value;
  cache->words = word_compression_open(path, node, index_value, error);
  if (*error != WORD_COMPRESSION_SUCCESS) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    word_compression_free((void **)&cache->path, path_length + 1);
    word_compression_free((void **)&cache, sizeof(DictCache));
    return 0;
  }

  cache->node = *node;

  if (!word_compression_realloc((void **)&cache_list, &cache_size, sizeof(cache_list))) {
    *error = word_compression_error(WORD_COMPRESSION_ERROR_ALLOC, NULL);
    word_compression_free((void **)&cache->path, path_length + 1);
    word_compression_free((void **)&cache, sizeof(DictCache));
    return 0;
  }
  
  cache_list[cache_items++] = cache;
  return cache->words;
}

unsigned short word_compression_node_cache(WordCompressionNode *node) {
  int i = 0;
  for (i = 0; i < cache_items; i++) {
    if (cache_list[i]->node == node) {
      return 1;
    }
  }
  return 0;
}

void word_compression_cache_flush() {
  int i;
  for (i = 0; i < cache_items; i++) {
    word_compression_free_dictionary(&cache_list[i]->node, 1, 1, 1);
    word_compression_free((void **)&cache_list[i]->path, strlen(cache_list[i]->path) + 1);
    word_compression_free((void **)&cache_list[i], sizeof(DictCache));
  }

  word_compression_free((void**)&cache_list, cache_size);
  cache_size = 0;
  cache_items = 0;
}

unsigned long word_compression_cache(const char *path,
                                     WordCompressionNode **node,
                                     short index_value, short *error) {
  int i;
  if (cache_list == NULL || cache_items == 0 || cache_size == 0) {
    return word_compression_create_cache(path, node, index_value, error);
  }

  for (i = 0; i < cache_items; i++) {
    if (index_value == cache_list[i]->index_value && strcmp(path, cache_list[i]->path) == 0) {
      /* has cache */
      *node = cache_list[i]->node;
      return cache_list[i]->words;
    }
  }

  return word_compression_create_cache(path, node, index_value, error);
}