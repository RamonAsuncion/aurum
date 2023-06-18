#include "lexer.h"
#include "scanner.h"
#include "stack.h"
#include "interpreter.h"
#include "memory.h"

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
    fprintf(stderr, "Empty stack.\n");
  }
}

void run_interpreter(const char *source_code)
{
  Scanner scanner;
  init_scanner(&scanner, source_code);
  Token token;
  Stack *stack = create_stack();
  char byte;
  uintptr_t addr;

  char *memory = malloc(MEMORY_CAPACITY * sizeof(char));

  if (memory == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }

  memset(memory, 0, MEMORY_CAPACITY * sizeof(char));

  int value; 
  int a, b, c;
  bool else_case_encountered = false;
  bool inside_loop = false;  
  const char* loop_start_pos = NULL;
  bool execute_loop = true;

  while ((token = scan_token(&scanner)).type != TOKEN_EOF) {
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
        a = pop(stack);
        push(stack, a);
        break;
      // I want to be able to do 2 dup 3 dup 4 dup . . . It's just 1 dup is basically 2 dup since push, push...
      case TOKEN_DUP:
        a = pop(stack);
        push(stack, a);
        push(stack, a);
        break;
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
        push(stack, a == b);
        break;
      case TOKEN_POP:
        pop(stack);
        break;
      case TOKEN_IF:
        if (pop(stack) == 0) {
          int block_depth = 1;
          bool else_case_encountered = false;
          while (block_depth > 0) {
              token = scan_token(&scanner);
              if (token.type == TOKEN_IF) {
                  block_depth++;
              } else if (token.type == TOKEN_ELSE && !else_case_encountered && block_depth == 1) {
                  else_case_encountered = true;
                  break;
              } else if (token.type == TOKEN_END) {
                  block_depth--;
              } else {
                  continue;
              }
          }
        }
        break;
      case TOKEN_ELSE:
        if (!else_case_encountered) {
          int block_depth = 1;
          while (block_depth > 0) {
              token = scan_token(&scanner);
              if (token.type == TOKEN_IF) {
                  block_depth++;
              } else if (token.type == TOKEN_END) {
                  block_depth--;
              }
          }
        }
        break; 
      case TOKEN_GREATER:
        b = pop(stack);
        a = pop(stack);
        push(stack, a > b);
        break;
      case TOKEN_LESS:
        b = pop(stack);
        a = pop(stack);
        push(stack, a < b);
        break;
      case TOKEN_WHILE:
        loop_start_pos = scanner.current;
        inside_loop = true;
        break;
      case TOKEN_THEN:
        if (pop(stack) == 0) 
          execute_loop = false;
        break;
      case TOKEN_END:
        if (execute_loop) {
            scanner.current = loop_start_pos;
        } else {
            inside_loop = false;
            execute_loop = true;
        }
        break;
      case TOKEN_MEMORY:
        push(stack, (intptr_t)memory);
        break;
      case TOKEN_FETCH:
        addr = pop(stack);
        byte = memory[addr];
        printf("Load: addr: %lu: byte: %c\n", addr, byte);
        push(stack, byte);
        break;
      case TOKEN_STORE:
        byte = pop(stack);
        addr = pop(stack);
        printf("Store: addr: %lu: byte: %c\n", addr, byte);
        memory[addr] = byte & 0xFF;
        break;
      case TOKEN_EOF: break;
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
  free(memory);
}
