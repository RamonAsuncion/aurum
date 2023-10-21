#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack *create_stack(void) 
{
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->data = (int *)malloc(DEFAULT_CAPACITY * sizeof(int));
  stack->top = -1;
  stack->capacity = DEFAULT_CAPACITY;
  stack->size = 0;
  return stack;
}

void push(Stack *stack, int value) 
{
  if (stack->top == stack->capacity - 1) {
    int new_capacity = stack->capacity * 2;
    int *new_data = (int *)realloc(stack->data, new_capacity * sizeof(int));
    if (new_data == NULL) {
      return;
    }
    stack->data = new_data;
    stack->capacity = new_capacity;
  }
  stack->top++;
  stack->size++;
  stack->data[stack->top] = value;
}

int pop(Stack *stack) 
{
  if (stack->top <= -1) {
    return -1;
  }
  int popped_value = stack->data[stack->top];
  stack->top--;
  stack->size--;
  return popped_value;
}

int top(Stack *stack) 
{
  return stack->data[stack->top];
}

void dump(Stack *stack) 
{
  int size = stack->size;

  printf("+---------+---------+\n");
  printf("|  Index  |  Value  |\n");
  printf("+---------+---------+\n");
  
  if (size == 0) {
    printf("|       Empty       |\n");
  } else {
    for (int i = 0; i < size; i++) {
      printf("| %-7d | %-7d |\n", i, stack->data[i]);
    }
  }
  
  printf("+---------+---------+\n");
}

bool is_empty(Stack *stack) 
{
  return stack->size == 0;
}
