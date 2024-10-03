#include <assert.h>
#include <stdio.h>

#include "stack.h"

void test_stack(void)
{
  Stack *stack = create_stack();

  assert(stack->size == 0);

  push(stack, 5);
  assert(stack->size == 1);

  push(stack, 8);
  assert(stack->size == 2);

  push(stack, 42);
  assert(stack->size == 3);

  int popped_value = pop(stack);
  assert(popped_value == 42);
  assert(stack->size == 2);

  popped_value = pop(stack);
  assert(popped_value == 8);
  assert(stack->size == 1);

  popped_value = pop(stack);
  assert(popped_value == 5);
  assert(stack->size == 0);

  // This will cause an underflow error.
  assert(pop(stack) == -1);
}

int main(void) {
  test_stack();
  printf("stack_test.c: All tests passed.\n");
  return 0;
}

