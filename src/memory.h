#ifndef _MEMORY_H_
#define _MEMORY_H_

#define SYS_READ  0
#define SYS_WRITE 1
#define SYS_EXIT  60

#include <stdio.h>

#define MEMORY_CAPACITY (64 * 1024)

/**
 * @brief Display the memory content.
 * @param memory The memory to display.
 * @param size The size of the memory.
 */
void print_memory(char *memory, int size);

/**
 * @brief Createe a memory tape.
 * @param n The number of bytes to allocate.
 */
void *memory_create(size_t n);

/**
 * clean up the memory tape.
 * @param memory The memory to display.
 */
void memory_free(char *memory);

#endif /* _MEMORY_H_ */

