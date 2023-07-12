#include "lexer.h"
#include "scanner.h"
#include "stack.h"
#include "interpreter.h"
#include "memory.h"

#define ASCII_MAX_SIZE 100

Token token;
Scanner scanner;

/* Data stack */
Stack *stack;

/* Handle control flow */
Stack *loop_stack;
Stack *end_stack;

char memory[MEMORY_CAPACITY];

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

void action_while(void)
{
  if (top(loop_stack) != scanner.position)
    push(loop_stack, scanner.position);

}

void action_do(void)
{
  int condition = pop(stack);
  if (condition == 0) {
    pop(loop_stack);  // pop the position of the while
    scanner.position = pop(end_stack);
    // TODO: This should not be from the beginning of the source code.
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

void action_dup(void) 
{
  int a = pop(stack);
  push(stack, a);
  push(stack, a);
}

void action_two_dup(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a);
  push(stack, b);
  push(stack, a);
  push(stack, b);
}

void action_drop(void)
{
  pop(stack);
}

void action_two_drop(void)
{
  pop(stack);
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

void action_two_swap(void)
{
  int d = pop(stack);
  int c = pop(stack);
  int b = pop(stack);
  int a = pop(stack);
  push(stack, c);
  push(stack, d);
  push(stack, a);
  push(stack, b);
}

void action_over(void)
{
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a);
  push(stack, b);
  push(stack, a);
}

void action_two_over(void)
{
  int c = pop(stack);
  int b = pop(stack);
  int a = pop(stack);
  push(stack, a);
  push(stack, b);
  push(stack, c);
  push(stack, a);
  push(stack, b);
  push(stack, c);
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

void action_peek(void)
{
  int a = pop(stack);
  push(stack, a);
}

void action_if(void) {}

void action_then(void) 
{
  // Check the condition and continue the body if it is true. Otherwise, skip the body.
  int condition = pop(stack);
  if (condition == 0) {
    int block_depth = 1;
    while (block_depth > 0) {
      token = scan_token(&scanner);
      if (token.type == TOKEN_IF) {
          block_depth++;
      } else if (token.type == TOKEN_ELSE && block_depth == 1) { // !else_case_encountered && block_depth == 1) {
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
  // Skip the body if the condition is true. Otherwise, continue the body.
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

void action_store(void)
{
  char byte = pop(stack);
  uintptr_t addr = pop(stack);
  memory[addr] = byte & 0xFF;
}

void action_fetch(void)
{
  uintptr_t addr = pop(stack);
  char byte = memory[addr];
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
  int arg1 = pop(stack);
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

void action_ascii(void) {
    int value;
    int i = 0;
    char buffer[ASCII_MAX_SIZE];
    int length = pop(stack);
    
    for(int count = 0; count < length; count++) {
        value = pop(stack);
        if(value >= 0x20 && value <= 0x7E) {
            buffer[i++] = (char) value;
        } else if(value == 0x0A) {
            buffer[i++] = '\n';
        } else {
            buffer[i++] = '?';
        }
    }

    // Reverse the buffer and print the characters
    for(int j = i - 1; j >= 0; j--) {
        printf("%c", buffer[j]);
    }
}

void action_dump(void)
{
  dump(stack);
}

// This sometimes work with the memory that's allocated. Problem like the memory.au.
void action_string_literal(void)
{
  int memory_index = 0;
  int string_length = strlen(token.lexeme);
  char* string = malloc((string_length + 1) * sizeof(char));
  strcpy(string, token.lexeme);

  // Remove the quotes from the string literal
  char* literal = string + 1;
  literal[string_length - 2] = '\0';

  // Store the characters in memory
  int literal_length = strlen(literal);
  for (int i = 0; i < literal_length; ++i) {
    if (literal[i] == '\\') {
      // Handle escape sequences
      switch (literal[i + 1]) {
        case 'n': // Newline character
          memory[memory_index++] = '\n';
          i++; // Skip the next character ('\n')
          break;
        case 't': // Tab character
          memory[memory_index++] = '\t';
          i++; // Skip the next character ('\t')
          break;
        default: // Treat other characters as regular characters
          memory[memory_index++] = literal[i];
          break;
      }
    } else {
      memory[memory_index++] = literal[i];
    }
  }
  
  memory[memory_index] = '\0'; // Null-terminate the memory

  push(stack, literal_length);
  push(stack, memory_index - literal_length);

  free(string); // Free the allocated memory for the string
}

// Adding macros to the language.
void action_define(void)
{
  // Grab the name of the macro.
  Token macro_name = scan_token(&scanner); // Save the first token as macro_name

  Token* macros = malloc(sizeof(Token) * 100);
  int i = 0;
  Token token = scan_token(&scanner); // Start scanning tokens after the macro_name
  while (token.type != TOKEN_END) {
    macros[i++] = token;
    token = scan_token(&scanner);
  }
  macros[i] = token;

  for (int j = 0; j < i; ++j) {
    printf("token: %s\n", macros[j].lexeme);
  }

  printf("macro_name: %s\n", macro_name.lexeme);

  free(macros);
}

void run_interpreter(const char *source_code)
{
  stack = create_stack();
  loop_stack = create_stack();
  end_stack = create_stack();

  init_scanner(&scanner, source_code);

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
    [TOKEN_DO] = action_do,
    [TOKEN_END] = action_end,
    [TOKEN_GREATER] = action_gt,
    [TOKEN_LESS] = action_lt,
    [TOKEN_GREATER_EQUAL] = action_gte,
    [TOKEN_LESS_EQUAL] = action_lte,
    [TOKEN_DUP] = action_dup,
    [TOKEN_TWO_DUP] = action_two_dup,
    [TOKEN_DROP] = action_drop,
    [TOKEN_TWO_DROP] = action_two_drop,
    [TOKEN_EQUAL] = action_equal,
    [TOKEN_SWAP] = action_swap,
    [TOKEN_TWO_SWAP] = action_two_swap,
    [TOKEN_OVER] = action_over,
    [TOKEN_TWO_OVER] = action_two_over,
    [TOKEN_ROT] = action_rot,
    [TOKEN_PEEK] = action_peek,
    /* I don't know what I'm going to do for now. The if statements need a revamp. */
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
    [TOKEN_STRING_LITERAL] = action_string_literal,
    [TOKEN_DEFINE] = action_define,
  };

  while ((token = scan_token(&scanner)).type != TOKEN_EOF) {
    action_func_t action = actions[token.type];
    if (action != NULL) {
      action();
    } else {
      fprintf(stderr, "Unknown token type: %d\n", token.type);
      exit(1);
    }
  }
  // Free the stack and its contents
  while (stack->size > 0) {
    pop(stack);
  }

  free(loop_stack);
  free(end_stack);
  free(stack);
}
