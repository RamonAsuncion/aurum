#include <stdio.h>
#include <stdlib.h>

int main() {
  char* numbers;
  int size = 5;

  // Allocate memory for an array of integers
  numbers = (char*) malloc(size * sizeof(char));

  printf("memory: %ld\n", (intptr_t)numbers);

  if (numbers == NULL) {
    printf("Failed to allocate memory\n");
    return 1;
  }

  // Access and modify the allocated memory
  for (int i = 0; i < size; i++) {
    numbers[i] = i * 10;
  }

  // Print the array
  for (int i = 0; i < size; i++) {
    printf("%d ", numbers[i]);
  }

  // Free the allocated memory
  free(numbers);

  return 0;
}

