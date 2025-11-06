#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "memory.h"

void print_memory(char *memory, int size)
{
  for (int i = 0; i < size; ++i) {
    printf("[%d]: %c ", i, memory[i]);
  }
  printf("\n");
}

