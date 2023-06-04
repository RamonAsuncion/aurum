#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "scanner.h"
#include "stack.h"

int apply_operator(int operator_type, int a, int b)
{
  switch (operator_type) {
    case TOKEN_ADD:
      return a + b;
    case TOKEN_SUBTRACT:
      return a - b;
    case TOKEN_MULTIPLY:
      return a * b;
    case TOKEN_DIVIDE:
      return a / b;
    case TOKEN_MODULO:
      return a % b;
    default:
      fprintf(stderr, "Unknown operator: %d\n", operator_type);
      exit(1);
  }
}

void print_result(Stack *stack) 
{
  if (stack_size(stack) > 0) {
    int result_str = pop(stack);
    printf("%d\n", result_str);
  } else {
    fprintf(stderr, "Empty stack. Nothing to print.\n");
  }
}

void run_interpreter(const char *source_code)
{
  Scanner scanner;
  init_scanner(&scanner, source_code);
  Token token;
  Stack *stack = create_stack();

  int value; // Declare the "value" variable here
  int a, b, c;

  while ((token = scan_token(&scanner)).type != TOKEN_EOF) {
    // printf("Token: %u, Lexeme: %s\n", token.type, token.lexeme);
    switch (token.type) {
      case TOKEN_NUMBER:  
        value = atoi(token.lexeme);
        push(stack, value);
        break;
      case TOKEN_ADD:
      case TOKEN_SUBTRACT:
      case TOKEN_MULTIPLY:
      case TOKEN_DIVIDE:
      case TOKEN_MODULO:
        b = pop(stack); 
        a = pop(stack); 
        int result = apply_operator(token.type, a, b);
        push(stack, result);
        break;
      case TOKEN_PERIOD:
        print_result(stack);
        break;
      case TOKEN_DROP:
        pop(stack);
        break;
      case TOKEN_DUP:
        a = pop(stack);
        push(stack, a);
        push(stack, a);
        break;
      case TOKEN_SWAP:
        b = pop(stack);
        a = pop(stack);
        push(stack, b);
        push(stack, a);
        break;
      case TOKEN_OVER:
        b = pop(stack);
        a = pop(stack);
        push(stack, a);
        push(stack, b);
        push(stack, a);
        break;
      case TOKEN_ROT:
        c = pop(stack);
        b = pop(stack);
        a = pop(stack);
        push(stack, b);
        push(stack, c);
        push(stack, a);
        break;
      case TOKEN_EQUAL:
        b = pop(stack);
        a = pop(stack);
        if (a == b) {
          push(stack, 1);
        } else {
          push(stack, 0);
        }
        break;
      default:
        fprintf(stderr, "Unknown token type: %u\n", token.type);
        exit(1);
    }
  }

  // Free the stack and its contents
  while (stack_size(stack) > 0)
  {
    pop(stack);
  }
  free(stack);

  // destroy_scanner(&scanner);
}
