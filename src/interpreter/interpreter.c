#include "lexer.h"
#include "scanner.h"
#include "stack.h"
#include "interpreter.h"
#include "memory.h"

Token token;
Scanner scanner;

Stack *stack;

Stack *loop_stack;
Stack *end_stack;

bool else_case_encountered = false;

char *memory;

void print_result(Stack *stack)
{
  if (stack->size > 0) {
    int result_str = pop(stack);
    printf("%d\n", result_str);
  } else {
    fprintf(stderr, "Empty stack.\n");
  }
}

void action_number(void)
{
  int value = atoi(token.lexeme);
  push(stack, value);
}

void action_add(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a + b);
}

void action_subtract(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a - b);
}

void action_multiply(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a * b);
}

void action_divide(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a / b);
}

void action_print(void)
{
  print_result(stack);
}

int nested_loop = 0;

void action_while(void)
{
    if (top(loop_stack) != scanner.position)
      push(loop_stack, scanner.position);

}

void action_then(void)
{
    int condition = pop(stack);
    if (condition == 0) {
        pop(loop_stack);  
        scanner.position = pop(end_stack);
        scanner.current = scanner.source + scanner.position + 3;
        pop(stack);
    }
}

void action_end(void)
{
    int loop_start = pop(loop_stack);
    if (top(stack) != scanner.position)
      push(end_stack, scanner.position);
    scanner.position = loop_start;
    scanner.current = scanner.source + scanner.position;
}

// TODO: I don't think 3+ dup works.
void action_dup(void) 
{
  int n = pop(stack);

  if (n > 1 && stack->size >= n) {
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

void action_pop(void)
{
  pop(stack);
}

void action_equal(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a == b);
}

void action_swap(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, b);
  push(stack, a);
}

void action_over(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a);
  push(stack, b);
  push(stack, a);
}

void action_rot(void)
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

void action_drop(void)
{
  int a = pop(stack);
  push(stack, a);
}

void action_if(void)
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

void action_else(void)
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

void action_store(void)
{
  char byte = pop(stack);
  uintptr_t addr = pop(stack);
  // printf("Store: addr: %lu: byte: %c\n", addr, byte);
  memory[addr] = byte & 0xFF;
}

void action_fetch(void)
{
  uintptr_t addr = pop(stack);
  char byte = memory[addr];
  // printf("Load: addr: %lu: byte: %c\n", addr, byte);
  push(stack, byte);
}

void action_memory(void)
{
  push(stack, (intptr_t)memory);
}

void action_modulo(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a % b);
}

void action_lt(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a < b);
}

void action_gt(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a > b);
}

void action_lte(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a <= b);
}

void action_gte(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a >= b);
}

void action_right_shift(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a >> b);
}

void action_left_shift(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a << b);
}

void action_bitwise_and(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a & b);
}

void action_bitwise_or(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a | b);
}

void action_bitwise_not(void)
{
  int a = pop(stack);
  push(stack, ~a);
}

void action_bitwise_xor(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a ^ b);
}

void action_syscall(void) 
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

void action_ascii(void) 
{
  int value = pop(stack);
  // Check if the value is within the range of graphic characters (0x20 to 0x7E) including space.
  if (value >= 0x20 && value <= 0x7E) {
    printf("%c", value);
  } else if (value == 0x0A) {
    printf("\n");
  } else {
    printf("?");
  }

  push(stack, value);
}

void action_dump(void)
{
  dump(stack);
}

void run_interpreter(const char *source_code)
{
  stack = create_stack();
  loop_stack = create_stack();
  end_stack = create_stack();

  init_scanner(&scanner, source_code);
  
  memory = malloc(MEMORY_CAPACITY * sizeof(char));

  if (memory == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }

  memset(memory, 0, MEMORY_CAPACITY * sizeof(char));

  typedef void (*action_func_t)(void);
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
      [TOKEN_GREATER_EQUAL] = action_gte,
      [TOKEN_LESS_EQUAL] = action_lte,
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
    } else {
      fprintf(stderr, "Unknown token type: %u\n", token.type);
      exit(1);
    }
  }
  // Free the stack and its contents
  while (stack->size > 0) {
    pop(stack);
  }
  free(stack);
  free(memory);
}
