#ifndef _stack_h_
#define _stack_h_

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

/**
 * @file stack.h
 * @brief Stack implementation.
 */

typedef struct StackNode {
    int value;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int size;
} Stack;

/**
 * @brief Creates a new stack with the given size.
 * @return A pointer to the newly created stack.
 */
Stack *create_stack();

/**
 * @brief Pushes a value onto the stack.
 * @param stack The stack to push the value onto.
 * @param value The value to push onto the stack.
 */
void push(Stack *stack, int value);

/**
 * @brief Pops a value from the stack.
 * @param stack The stack to pop the value from.
 * @return The value popped from the stack.
 */
int pop(Stack *stack);

/**
 * @brief Returns the size of the stack.
 * @param stack The stack to get the size of.
 * @return The size of the stack.
  */
int stack_size(Stack *stack);

#endif // _stack_h_
