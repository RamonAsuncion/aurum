#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"

void test_stack() {
    Stack *stack = create_stack();

    assert(stack_size(stack) == 0);

    push(stack, "hello");
    assert(stack_size(stack) == 1);

    push(stack, "world");
    assert(stack_size(stack) == 2);

    push(stack, "42");
    assert(stack_size(stack) == 3);

    char *popped_value = pop(stack);
    assert(atoi(popped_value) == 42);
    assert(stack_size(stack) == 2);

    popped_value = pop(stack);
    assert(strcmp(popped_value, "world") == 0);
    assert(stack_size(stack) == 1);

    popped_value = pop(stack);
    assert(strcmp(popped_value, "hello") == 0);
    assert(stack_size(stack) == 0);

    // This will cause an underflow error.
    assert(pop(stack) == NULL);
}

int main() {
    test_stack();
    printf("stack_test.c: All tests passed.\n");
    return 0;
}