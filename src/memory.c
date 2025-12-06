#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "memory.h"

void print_memory(char *memory, int size)
{
  for (int i = 0; i < size; ++i) {
    printf("[%d]: %c ", i, memory[i]);
  }
  printf("\n");
}

void *memory_create(size_t n)
{
  if (n == 0) {
    fprintf(stderr, "[aurum] memory_create: zero allocation\n");
    exit(1);
  }

  void *p = malloc(n);
  if (!p) {
    fprintf(stderr, "[aurum] memory_create: allocation of %zu bytes failed", n);
    exit(1);
  }

  return p;
}

void memory_free(char *memory)
{
  if (!memory) return;
  free(memory);
}

