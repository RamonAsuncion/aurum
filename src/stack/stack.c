#include "stack.h"

Stack *create_stack() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->top = NULL;
  stack->size = 0;
  return stack;
}

void push(Stack *stack, int value) {
  StackNode *new_node = (StackNode *)malloc(sizeof(StackNode));
  new_node->value = value;
  new_node->next = stack->top;
  stack->top = new_node;
  stack->size++;
}

int pop(Stack *stack) {
  if (stack->top == NULL) {
    printf("Error: Underflow!\n");
    return -1; // Return an error value or handle the error appropriately
  }

  StackNode *temp = stack->top;
  int popped_value = temp->value;
  stack->top = temp->next;
  stack->size--;
  free(temp);
  return popped_value;
}

int stack_size(Stack *stack) {
  return stack->size;
}
