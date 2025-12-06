#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

struct stack *stack_create(void)
{
  struct stack *stack;

  stack = malloc(sizeof(*stack));
  stack->data = malloc(DEFAULT_CAPACITY * sizeof(int));
  stack->top = -1;
  stack->capacity = DEFAULT_CAPACITY;
  stack->size = 0;
  return stack;
}

void stack_push(struct stack *stack, int value)
{
  int new_capacity;
  int *new_data;

  if (stack->top == stack->capacity - 1) {
    new_capacity = stack->capacity * 2;
    new_data = realloc(stack->data, new_capacity * sizeof(int));
    if (new_data == NULL)
      return;
    stack->data = new_data;
    stack->capacity = new_capacity;
  }
  stack->top++;
  stack->size++;
  stack->data[stack->top] = value;
}

int stack_pop(struct stack *stack)
{
  int popped_value;

  if (stack->top <= -1)
    return -1;
  popped_value = stack->data[stack->top];
  stack->top--;
  stack->size--;
  return popped_value;
}

int stack_top(struct stack *stack)
{
  return stack->data[stack->top];
}

void stack_dump(struct stack *stack)
{
  int size;
  int i;

  size = stack->size;
  printf("+---------+---------+\n");
  printf("|  Index  |  Value  |\n");
  printf("+---------+---------+\n");
  if (size == 0) {
    printf("|       Empty       |\n");
  } else {
    for (i = 0; i < size; ++i)
      printf("| %-7d | %-7d |\n", i, stack->data[i]);
  }
  printf("+---------+---------+\n");
}

bool is_stack_empty(struct stack *stack)
{
  return stack->size == 0;
}

