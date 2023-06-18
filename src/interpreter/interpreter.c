#include "lexer.h"
#include "scanner.h"
#include "stack.h"
#include "interpreter.h"
#include "memory.h"

Token token;
Stack *stack;
Scanner scanner;

bool else_case_encountered = false;
bool inside_loop = false;
const char *loop_start_pos = NULL;
bool execute_loop = true;
char *memory;

void print_result(Stack *stack)
{
  if (stack_size(stack) > 0) {
    int result_str = pop(stack);
    printf("%d\n", result_str);
  }
  else {
    fprintf(stderr, "Empty stack.\n");
  }
}

void action_number()
{
  int value = atoi(token.lexeme);
  push(stack, value);
}

void action_add()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a + b);
}

void action_subtract()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a - b);
}

void action_multiply()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a * b);
}

void action_divide()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a / b);
}

void action_print()
{
  print_result(stack);
}

void action_while()
{
  loop_start_pos = scanner.current;
  inside_loop = true;
}

void action_then()
{
  if (pop(stack) == 0)
    execute_loop = false;
}

void action_end()
{
  if (execute_loop) {
    scanner.current = loop_start_pos;
  }
  else {
    inside_loop = false;
    execute_loop = true;
  }
}

void action_gt()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a > b);
}

// TODO: I don't think 3+ dup works.
void action_dup() {
  int n = pop(stack);

  if (n > 1 && stack_size(stack) >= n) {
    int *temp = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) 
      temp[i] = pop(stack);
  

    for (int j = 0; j < 2; ++j) 
      for (int i = n - 1; i >= 0; --i) 
          push(stack, temp[i]);

    free(temp);
  } else {
    push(stack, n);
    push(stack, n);
  }
}

void action_pop()
{
  pop(stack);
}

void action_equal()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a == b);
}

void action_swap()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, b);
  push(stack, a);
}

void action_over()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a);
  push(stack, b);
  push(stack, a);
}

void action_rot()
{
  int c = pop(stack);
  int b = pop(stack);
  int a = pop(stack);
  push(stack, b);
  push(stack, c);
  push(stack, a);
}

/**
 * FIXME: This is suppose to print the value from the top without popping it.
 * 
 * 
 * Try to look at drop and pop from pandas and python... or anything else. 
*/

void action_drop()
{
  int a = pop(stack);
  push(stack, a);
}

void action_if()
{
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
}

void action_else()
{
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
}

void action_store()
{
  char byte = pop(stack);
  uintptr_t addr = pop(stack);
  // printf("Store: addr: %lu: byte: %c\n", addr, byte);
  memory[addr] = byte & 0xFF;
}

void action_fetch()
{
  uintptr_t addr = pop(stack);
  char byte = memory[addr];
  // printf("Load: addr: %lu: byte: %c\n", addr, byte);
  push(stack, byte);
}

void action_memory()
{
  push(stack, (intptr_t)memory);
}

void action_modulo()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a % b);
}

void action_lt()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a < b);
}

void action_right_shift()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a >> b);
}

void action_left_shift()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a << b);
}

void action_bitwise_and()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a & b);
}

void action_bitwise_or()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a | b);
}

void action_bitwise_not()
{
  int a = pop(stack);
  push(stack, ~a);
}

void action_bitwise_xor()
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a ^ b);
}

void action_syscall() 
{
    int argument_count = pop(stack);

    if (argument_count < 1 || argument_count > 3) {
        fprintf(stderr, "Invalid number of arguments for syscall.\n");
        exit(1);
    }

    int syscall_number = pop(stack);
    int arg1 = (argument_count >= 1) ? pop(stack) : 0;
    int arg2 = (argument_count >= 2) ? pop(stack) : 0;
    int arg3 = (argument_count >= 3) ? pop(stack) : 0;


    switch (syscall_number) {
        case SYS_READ: {
            int fd = arg1;
            int buf = arg2;
            int count = arg3;
            char* data = malloc(count * sizeof(char));
            int bytes_read = read(fd, data, count);
            memcpy(memory + buf, data, bytes_read);
            free(data);
            push(stack, bytes_read);
            break;
        }
        case SYS_WRITE: {
            int fd = arg1;
            int buf = arg2;
            int count = arg3;
            int bytes_written = write(fd, memory + buf, count);
            push(stack, bytes_written);
            break;
        }
        case SYS_EXIT: {
            int status = arg1;
            exit(status);
        }
    }
}

void action_ascii() 
{
  int value = pop(stack);
  // Check if the value is within the range of graphic characters (0x20 to 0x7E) including space.
  if (value >= 0x20 && value <= 0x7E) {
    printf("%c", value);
  } else if (value == 0x0A) { // Check if the value is a newline character (FIXME: Temporary)
    printf("\n");
  } else {
    printf("?");
  }

  push(stack, value);
}

void action_dump()
{
  dump(stack);
}

void run_interpreter(const char *source_code)
{
  stack = create_stack();
  init_scanner(&scanner, source_code);
  
  memory = malloc(MEMORY_CAPACITY * sizeof(char));

  if (memory == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }

  memset(memory, 0, MEMORY_CAPACITY * sizeof(char));

  typedef void (*action_func_t)();
  action_func_t actions[] = {
      [TOKEN_NUMBER] = action_number,
      [TOKEN_ASCII] = action_ascii,
      [TOKEN_ADD] = action_add,
      [TOKEN_SUBTRACT] = action_subtract,
      [TOKEN_MULTIPLY] = action_multiply,
      [TOKEN_DIVIDE] = action_divide,
      [TOKEN_MODULO] = action_modulo,
      [TOKEN_PERIOD] = action_print,
      [TOKEN_WHILE] = action_while,
      [TOKEN_THEN] = action_then,
      [TOKEN_END] = action_end,
      [TOKEN_GREATER] = action_gt,
      [TOKEN_LESS] = action_lt,
      [TOKEN_DUP] = action_dup,
      [TOKEN_POP] = action_pop,
      [TOKEN_EQUAL] = action_equal,
      [TOKEN_SWAP] = action_swap,
      [TOKEN_OVER] = action_over,
      [TOKEN_ROT] = action_rot,
      [TOKEN_DROP] = action_drop,
      [TOKEN_IF] = action_if,
      [TOKEN_ELSE] = action_else,
      [TOKEN_STORE] = action_store,
      [TOKEN_FETCH] = action_fetch,
      [TOKEN_MEMORY] = action_memory,
      [TOKEN_RIGHT_SHIFT] = action_right_shift,
      [TOKEN_LEFT_SHIFT] = action_left_shift,
      [TOKEN_BITWISE_AND] = action_bitwise_and,
      [TOKEN_BITWISE_OR] = action_bitwise_or,
      [TOKEN_BITWISE_NOT] = action_bitwise_not,
      [TOKEN_BITWISE_XOR] = action_bitwise_xor,
      [TOKEN_SYSCALL] = action_syscall,
      [TOKEN_DUMP] = action_dump,
  };

  while ((token = scan_token(&scanner)).type != TOKEN_EOF) {
    action_func_t action = actions[token.type];
    if (action != NULL) {
      action();
    }
    else {
      fprintf(stderr, "Unknown token type: %u\n", token.type);
      exit(1);
    }
  }
  // Free the stack and its contents
  while (stack_size(stack) > 0) {
    pop(stack);
  }
  free(stack);
  free(memory);
}
