#ifndef _stack_h_
#define _stack_h_

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

/**
 * @file stack.h
 * @brief Stack implementation.
 */

typedef struct {
    int *data;
    int top;
    int capacity;
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

/**
 * @brief Returns the top value from the stack.
 * @param stack The stack to top the value from.
 * @return The value dropped from the stack.
*/
int top(Stack *stack);

/**
 * @brief Prints the stack.
 * @param stack The stack to print.
*/
void dump(Stack *stack);

#endif // _stack_h_
