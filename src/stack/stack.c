#include "stack.h"

Stack *create_stack(int capacity) 
{
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->data = (int *)malloc(capacity * sizeof(int));
  stack->top = -1;
  stack->capacity = capacity;
  return stack;
}

void push(Stack *stack, int value) 
{
  if (stack->top == stack->capacity - 1) {
    return;
  }
  stack->top++;
  stack->data[stack->top] = value;
}

int pop(Stack *stack) 
{
  if (stack->top == -1) {
    return -1;
  }
  int popped_value = stack->data[stack->top];
  stack->top--;
  return popped_value;
}

int stack_size(Stack *stack) 
{
  return stack->top + 1;
}

int top(Stack *stack) 
{
  return stack->data[stack->top];
}

void dump(Stack *stack) 
{
  int stackSize = stack_size(stack);

  printf("+---------+---------+\n");
  printf("|  Index  |  Value  |\n");
  printf("+---------+---------+\n");
  
  if (stackSize == 0) {
    printf("|       Empty       |\n");
  } else {
    for (int i = 0; i < stackSize; i++) {
      printf("| %-7d | %-7d |\n", i, stack->data[i]);
    }
  }
  
  printf("+---------+---------+\n");
}

