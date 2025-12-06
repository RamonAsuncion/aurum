#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

struct hashmap *hashmap_create(void)
{
  struct hashmap *map;
  int i;

  map = malloc(sizeof(*map));
  map->capacity = 16;
  map->size = 0;
  map->entries = malloc(map->capacity * sizeof(struct macro *));
  for (i = 0; i < map->capacity; ++i)
    map->entries[i] = NULL;
  return map;
}

void hashmap_insert(struct hashmap *map, const char *key,
    struct token *tokens, int num_tokens)
{
  struct macro *macro;
  size_t index;
  size_t i;

#ifdef DEBUG
  fprintf(stderr, "[debug] hashmap_insert: key='%s' at address %p\n", key, (void*)key);
#endif
  macro = malloc(sizeof(*macro));
  macro->key = strdup(key);
#ifdef DEBUG
  fprintf(stderr, "[debug] hashmap_insert: copied key='%s' at address %p\n",
      macro->key, (void*)macro->key);
#endif
  macro->tokens = malloc(num_tokens * sizeof(struct token));
  macro->num_tokens = num_tokens;
  for (i = 0; i < (size_t)num_tokens; ++i) {
    macro->tokens[i] = tokens[i];
    macro->tokens[i].lexeme = strdup(tokens[i].lexeme);
  }
  index = 0;
  for (i = 0; i < strlen(key); ++i)
    index += key[i];
  index %= map->capacity;
#ifdef DEBUG
  fprintf(stderr, "[debug] hashmap_insert: index=%zu\n", index);
#endif
  while (map->entries[index] != NULL) {
    index++;
    index %= map->capacity;
  }
  map->entries[index] = macro;
  map->size++;
#ifdef DEBUG
  fprintf(stderr, "[debug] hashmap_insert: stored at index=%zu\n", index);
#endif
}

struct macro *hashmap_get(struct hashmap *map, const char *key)
{
  size_t index;
  size_t i;
  size_t start_index;

  if (!map) {
    fprintf(stderr, "[aurum] hashmap is null.\n");
    return NULL;
  }
#ifdef DEBUG
  fprintf(stderr, "[debug] hashmap_get: looking for key='%s' at address %p\n",
      key, (void*)key);
#endif
  index = 0;
  for (i = 0; i < strlen(key); ++i)
    index += key[i];
  index %= map->capacity;
  start_index = index;
#ifdef DEBUG
  fprintf(stderr, "[debug] hashmap_get: starting at index=%zu\n", index);
#endif
  while (map->entries[index] != NULL) {
#ifdef DEBUG
    fprintf(stderr, "[debug] hashmap_get: checking index=%zu, key='%s'\n",
        index, map->entries[index]->key);
#endif
    if (strcmp(map->entries[index]->key, key) == 0) {
#ifdef DEBUG
      fprintf(stderr, "[debug] hashmap_get: match at index=%zu\n", index);
#endif
      return map->entries[index];
    }
    index++;
    index %= map->capacity;
    if (index == start_index) {
#ifdef DEBUG
      fprintf(stderr, "[debug] hashmap_get: searched full table, not found\n");
#endif
      break;
    }
  }
#ifdef DEBUG
  fprintf(stderr, "[debug] hashmap_get: hit null at index=%zu\n", index);
#endif
  return NULL;
}

void hashmap_free(struct hashmap *map)
{
  int i;

  for (i = 0; i < map->capacity; ++i) {
    if (map->entries[i] != NULL) {
      free(map->entries[i]->tokens);
      free(map->entries[i]);
    }
  }
  free(map->entries);
  free(map);
}

void hashmap_print(struct hashmap *map)
{
  int i;
  int j;

  for (i = 0; i < map->size; ++i) {
    if (map->entries[i] != NULL) {
      printf("Macro: %s\n", map->entries[i]->key);
      printf("Number of tokens: %d\n",
          map->entries[i]->num_tokens);
      printf("Tokens: ");
      for (j = 0; j < map->entries[i]->num_tokens; ++j)
        printf("%s ", map->entries[i]->tokens[j].lexeme);
      printf("\n");
    }
  }
}

