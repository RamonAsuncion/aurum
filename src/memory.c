#include <stdint.h>
#include <stdio.h>
#include <unistd.h> // for read() and write()

#include "memory.h"

void print_memory(char *memory, int size) 
{
  printf("Memory content: ");
  for (int i = 0; i < size; ++i) {
    printf("[%d]: %c ", i, memory[i]);
  }
  printf("\n");
}
