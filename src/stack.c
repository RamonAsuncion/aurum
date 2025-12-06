#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "stack.h"

struct stack *stack_create(void)
{
  struct stack *stack;

  stack = malloc(sizeof(*stack));
  stack->data = malloc(DEFAULT_CAPACITY * sizeof(intptr_t));
  stack->top = -1;
  stack->capacity = DEFAULT_CAPACITY;
  stack->size = 0;
  return stack;
}

void stack_push(struct stack *stack, intptr_t value)
{
  int new_capacity;
  intptr_t *new_data;

  if (stack->top == stack->capacity - 1) {
    new_capacity = stack->capacity * 2;
    new_data = realloc(stack->data, new_capacity * sizeof(intptr_t));
    if (new_data == NULL)
      return;
    stack->data = new_data;
    stack->capacity = new_capacity;
  }
  stack->top++;
  stack->size++;
  stack->data[stack->top] = value;
}

intptr_t stack_pop(struct stack *stack)
{
  intptr_t popped_value;

  if (stack->top <= -1)
    return (intptr_t)-1;
  popped_value = stack->data[stack->top];
  stack->top--;
  stack->size--;
  return popped_value;
}

intptr_t stack_top(struct stack *stack)
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
      printf("| %-7d | %-7" PRIdPTR " |\n", i, stack->data[i]);
  }
  printf("+---------+---------+\n");
}

void stack_free(struct stack *stack)
{
  if (!stack) return;
  free(stack->data);
  free(stack);
}

bool is_stack_empty(struct stack *stack)
{
  return stack->size == 0;
}


