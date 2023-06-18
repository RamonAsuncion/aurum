#ifndef _memory_h_
#define _memory_h_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h> // for read() and write()

#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_EXIT 60

#define MEMORY_CAPACITY (1024 * 1024 * 1024)

void print_memory(char *memory, int size);

#endif
