#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>

#include "lexer.h"
#include "scanner.h"
#include "stack.h"
#include "interpreter.h"
#include "memory.h"
#include "hashmap.h"
#include <inttypes.h>

static void print_result(struct stack *stack)
{
  if (stack->size > 0)
    printf("%" PRIdPTR "\n", stack_pop(stack));
  else
    fprintf(stderr, "[aurum] empty stack.\n");
}

static void op_print(struct stack *stack)
{
  print_result(stack);
}

static void op_dump(struct stack *stack)
{
  stack_dump(stack);
}

static void op_while(struct stack *loop_stack, const struct scanner *scanner)
{
  if (stack_top(loop_stack) != scanner->position)
    stack_push(loop_stack, scanner->position);
}

static void op_do(struct stack *stack, struct stack *loop_stack,
    struct stack *end_stack, struct scanner *scanner)
{
  intptr_t condition;
  int keyword_length;

  condition = stack_pop(stack);
  if (condition == 0) {
    stack_pop(loop_stack);
    scanner->position = stack_pop(end_stack);
    keyword_length = strlen("do") + 1;
    scanner->current = scanner->source + scanner->position +
      keyword_length;
    stack_pop(stack);
  }
}

static void op_end(struct stack *stack, struct stack *loop_stack,
    struct stack *end_stack, struct scanner *scanner)
{
  intptr_t loop_start;

  loop_start = stack_pop(loop_stack);
  if (stack_top(stack) != scanner->position)
    stack_push(end_stack, scanner->position);
  scanner->position = loop_start;
  scanner->current = scanner->source + scanner->position;
}

static void op_number(struct stack *stack, const struct token *token)
{
  intptr_t value;

  value = (token->type == TOKEN_CHAR) ? (intptr_t)token->lexeme[0] : (intptr_t)atoi(token->lexeme);
  stack_push(stack, value);
}

static void op_arithmetic(struct stack *stack, const struct token *token)
{
  intptr_t a;
  intptr_t b;
  intptr_t result;

  b = stack_pop(stack);
  a = stack_pop(stack);
  result = 0;

  switch (token->type) {
  case TOKEN_ADD:
    result = a + b;
    break;
  case TOKEN_SUBTRACT:
    result = a - b;
    break;
  case TOKEN_MULTIPLY:
    result = a * b;
    break;
  default:
    return;
  }

  stack_push(stack, result);
}

static void op_comparison(struct stack *stack, const struct token *token)
{
  intptr_t a;
  intptr_t b;
  intptr_t result;

  b = stack_pop(stack);
  a = stack_pop(stack);
  result = 0;

  switch (token->type) {
  case TOKEN_EQUAL:
    result = (a == b);
    break;
  case TOKEN_GREATER:
    result = (a > b);
    break;
  case TOKEN_LESS:
    result = (a < b);
    break;
  case TOKEN_GREATER_EQUAL:
    result = (a >= b);
    break;
  case TOKEN_LESS_EQUAL:
    result = (a <= b);
    break;
  default:
    return;
  }

  stack_push(stack, result);
}

static void op_bitwise(struct stack *stack, const struct token *token)
{
  intptr_t a;
  intptr_t b;
  intptr_t result;

  b = stack_pop(stack);
  a = stack_pop(stack);
  result = 0;

  switch (token->type) {
  case TOKEN_BITWISE_AND:
    result = a & b;
    break;
  case TOKEN_BITWISE_OR:
    result = a | b;
    break;
  case TOKEN_BITWISE_XOR:
    result = a ^ b;
    break;
  case TOKEN_BITWISE_NOT:
    result = ~a;
    stack_push(stack, result);
    return;
  default:
    return;
  }

  stack_push(stack, result);
}

static void op_store(struct stack *stack, char *memory)
{
  intptr_t raw_byte = stack_pop(stack);
  char byte = (char)raw_byte;
  intptr_t addr = stack_pop(stack);
  if (addr < 0 || (size_t)addr >= MEMORY_CAPACITY) {
    fprintf(stderr, "[aurum] invalid memory address: %ld\n", (long)addr);
    exit(1);
  }
  memory[(size_t)addr] = (char)(byte & 0xFF);
}

static void op_fetch(struct stack *stack, char *memory)
{
  intptr_t addr = stack_pop(stack);
  if (addr < 0 || (size_t)addr >= MEMORY_CAPACITY) {
    fprintf(stderr, "[aurum] invalid memory address: %ld\n", (long)addr);
    exit(1);
  }
  char byte = memory[(size_t)addr];
  stack_push(stack, (intptr_t)byte);
}

static void op_memory(struct stack *stack, char *memory)
{
  stack_push(stack, (intptr_t)0);
}

static void op_syscall(struct stack *stack, char *memory)
{
  int args[3];
  int argument_count;
  int syscall_number;

#ifdef DEBUG
  fprintf(stderr, "[debug] syscall: stack has %d items before syscall\n", stack->size);
#endif
  argument_count = (int)stack_pop(stack);
#ifdef DEBUG
  fprintf(stderr, "[debug] syscall: argument_count=%d\n", argument_count);
#endif

  if (argument_count < 1 || argument_count > 3) {
    fprintf(stderr, "[aurum] invalid number of arguments: %d.\n", argument_count);
    exit(1);
  }

  syscall_number = (int)stack_pop(stack);
#ifdef DEBUG
  fprintf(stderr, "[debug] syscall: syscall_number=%d\n", syscall_number);
#endif

  args[0] = (int)stack_pop(stack);

#ifdef DEBUG
  fprintf(stderr, "[debug] syscall: args[0]=%d\n", args[0]);
#endif
  args[1] = (argument_count >= 2) ? (int)stack_pop(stack) : 0;
#ifdef DEBUG
  fprintf(stderr, "[debug] syscall: args[1]=%d\n", args[1]);
#endif
  args[2] = (argument_count >= 3) ? (int)stack_pop(stack) : 0;
#ifdef DEBUG
  fprintf(stderr, "[debug] syscall: args[2]=%d\n", args[2]);
#endif
  switch (syscall_number) {
  case SYS_READ: {
    int fd;
    int buf;
    int count;
    char *data;
    int bytes_read;

    fd = args[0];
    buf = args[1];
    count = args[2];
    data = malloc((size_t)count * sizeof(char));
    bytes_read = (int)read(fd, data, (size_t)count);
    memcpy(memory + (size_t)buf, data, (size_t)bytes_read);
    free(data);
    stack_push(stack, (intptr_t)bytes_read);
    break;
  }
  case SYS_WRITE: {
    int fd;
    int buf;
    int count;
    int bytes_written;

    fd = args[0];
    buf = args[1];
    count = args[2];
    bytes_written = (int)write(fd, memory + (size_t)buf, (size_t)count);
    stack_push(stack, (intptr_t)bytes_written);
    break;
  }
  case SYS_EXIT:
    exit(args[0]);
  }
}

static void op_include(struct scanner *scanner)
{
  const char *filename;
  size_t filename_length;
  char *cleaned_filename;
  FILE *file;
  long file_size;
  char *buffer;
  intptr_t position;
  char *new_source;
  int i;

  filename = scan_token(scanner).lexeme;
  filename_length = strlen(filename);
  cleaned_filename = malloc(filename_length - 1);
  strncpy(cleaned_filename, filename + 1, filename_length - 2);
  cleaned_filename[filename_length - 2] = '\0';

  file = fopen(cleaned_filename, "r");
  if (!file) {
    fprintf(stderr, "[aurum] failed to open file: %s\n", cleaned_filename);
    exit(1);
  }

  fseek(file, 0L, SEEK_END);
  file_size = ftell(file);
  rewind(file);

  buffer = malloc((size_t)file_size + 1);
  fread(buffer, sizeof(char), (size_t)file_size, file);
  buffer[file_size] = '\0';
  fclose(file);

  for (i = 0; i < (int)file_size; ++i) {
    if (buffer[i] == '\n')
      buffer[i] = ' ';
  }

  position = scanner->position + (intptr_t)strlen(cleaned_filename) + 2;
  new_source = malloc(strlen(scanner->source) + strlen(buffer) + 1);
  strncpy(new_source, scanner->source, (size_t)position);
  strcat(new_source, buffer);
  strcat(new_source, scanner->source + position);

  scanner->source = new_source;
  scanner->current = new_source + position;

  free(buffer);
  free(cleaned_filename);
}

static void op_define(struct scanner *scanner, struct hashmap *hashmap)
{
  struct token macro_name;
  struct token *macros;
  int i;
  struct token temp_token;

  macro_name = scan_token(scanner);
  macros = malloc(sizeof(struct token) * 100);
  i = 0;

  while ((temp_token = scan_token(scanner)).type != TOKEN_END)
    macros[i++] = temp_token;

#ifdef DEBUG
  fprintf(stderr, "[debug] define: macro '%s' with %d tokens\n",
      macro_name.lexeme, i);
#endif
  hashmap_insert(hashmap, macro_name.lexeme, macros, i);
}

static void op_string_literal(struct stack *stack, char *memory,
    const struct token *token);

static void execute_token(struct stack *stack, struct stack *loop_stack,
    struct stack *end_stack, struct scanner *scanner,
    struct hashmap *hashmap, char *memory,
    const struct token *token);

static void op_macro(struct stack *stack, struct stack *loop_stack,
    struct stack *end_stack, struct scanner *scanner,
    struct hashmap *hashmap, char *memory,
    const struct token *token)
{
  const char *macro_name;
  struct macro *macro;
  int i;

  macro_name = token->lexeme;
#ifdef DEBUG
  fprintf(stderr, "[debug] macro: looking up '%s'\n", macro_name);
#endif
  macro = hashmap_get(hashmap, macro_name);

  if (!macro) {
    fprintf(stderr, "[aurum] unknown identifier: '%s'\n", macro_name);
    exit(1);
  }

#ifdef DEBUG
  fprintf(stderr, "[debug] macro: found '%s' with %d tokens\n",
      macro_name, macro->num_tokens);
#endif

  for (i = 0; i < macro->num_tokens; ++i) {
#ifdef DEBUG
    fprintf(stderr, "[debug] macro: executing token %d: type=%d, lexeme='%s'\n",
        i, macro->tokens[i].type, macro->tokens[i].lexeme);
#endif
    execute_token(stack, loop_stack, end_stack, scanner, hashmap,
        memory, &macro->tokens[i]);
  }
}

static void op_dup(struct stack *stack)
{
  intptr_t a;

  a = stack_pop(stack);

  stack_push(stack, a);
  stack_push(stack, a);
}

static void op_two_dup(struct stack *stack)
{
  intptr_t a;
  intptr_t b;

  b = stack_pop(stack);
  a = stack_pop(stack);

  stack_push(stack, a);
  stack_push(stack, b);
  stack_push(stack, a);
  stack_push(stack, b);
}

static void op_drop(struct stack *stack)
{
  stack_pop(stack);
}

static void op_two_drop(struct stack *stack)
{
  stack_pop(stack);
  stack_pop(stack);
}

static void op_swap(struct stack *stack)
{
  intptr_t a;
  intptr_t b;

  b = stack_pop(stack);
  a = stack_pop(stack);

  stack_push(stack, b);
  stack_push(stack, a);
}

static void op_two_swap(struct stack *stack)
{
  intptr_t a;
  intptr_t b;
  intptr_t c;
  intptr_t d;

  d = stack_pop(stack);
  c = stack_pop(stack);
  b = stack_pop(stack);
  a = stack_pop(stack);

  stack_push(stack, c);
  stack_push(stack, d);
  stack_push(stack, a);
  stack_push(stack, b);
}

static void op_over(struct stack *stack)
{
  intptr_t a;
  intptr_t b;

  b = stack_pop(stack);
  a = stack_pop(stack);

  stack_push(stack, a);
  stack_push(stack, b);
  stack_push(stack, a);
}

static void op_two_over(struct stack *stack)
{
  intptr_t a;
  intptr_t b;
  intptr_t c;

  c = stack_pop(stack);
  b = stack_pop(stack);
  a = stack_pop(stack);

  stack_push(stack, a);
  stack_push(stack, b);
  stack_push(stack, c);
  stack_push(stack, a);
  stack_push(stack, b);
  stack_push(stack, c);
}

static void op_rot(struct stack *stack)
{
  intptr_t a;
  intptr_t b;
  intptr_t c;

  c = stack_pop(stack);
  b = stack_pop(stack);
  a = stack_pop(stack);

  stack_push(stack, b);
  stack_push(stack, c);
  stack_push(stack, a);
}

static void op_peek(struct stack *stack)
{
  intptr_t a;

  a = stack_pop(stack);
  stack_push(stack, a);
}

static void execute_token(struct stack *stack, struct stack *loop_stack,
    struct stack *end_stack, struct scanner *scanner,
    struct hashmap *hashmap, char *memory,
    const struct token *token)
{
  switch (token->type) {
  case TOKEN_NUMBER:
  case TOKEN_CHAR:
    op_number(stack, token);
    break;
  case TOKEN_ADD:
  case TOKEN_SUBTRACT:
  case TOKEN_MULTIPLY:
    op_arithmetic(stack, token);
    break;
  case TOKEN_QUESTION:
    op_print(stack);
    break;
  case TOKEN_WHILE:
    op_while(loop_stack, scanner);
    break;
  case TOKEN_DO:
    op_do(stack, loop_stack, end_stack, scanner);
    break;
  case TOKEN_DUMP:
    op_dump(stack);
    break;
  case TOKEN_END:
    op_end(stack, loop_stack, end_stack, scanner);
    break;
  case TOKEN_EQUAL:
  case TOKEN_GREATER:
  case TOKEN_LESS:
  case TOKEN_GREATER_EQUAL:
  case TOKEN_LESS_EQUAL:
    op_comparison(stack, token);
    break;
  case TOKEN_BITWISE_AND:
  case TOKEN_BITWISE_OR:
  case TOKEN_BITWISE_XOR:
  case TOKEN_BITWISE_NOT:
    op_bitwise(stack, token);
    break;
  case TOKEN_STRING_LITERAL:
    op_string_literal(stack, memory, token);
    break;
  case TOKEN_SYSCALL:
    op_syscall(stack, memory);
    break;
  case TOKEN_DEFINE:
    op_define(scanner, hashmap);
    break;
  case TOKEN_INCLUDE:
    op_include(scanner);
    break;
  case TOKEN_MEMORY:
    op_memory(stack, memory);
    break;
  case TOKEN_STORE:
    op_store(stack, memory);
    break;
  case TOKEN_FETCH:
    op_fetch(stack, memory);
    break;
  case TOKEN_DUP:
    op_dup(stack);
    break;
  case TOKEN_TWO_DUP:
    op_two_dup(stack);
    break;
  case TOKEN_DROP:
    op_drop(stack);
    break;
  case TOKEN_TWO_DROP:
    op_two_drop(stack);
    break;
  case TOKEN_SWAP:
    op_swap(stack);
    break;
  case TOKEN_TWO_SWAP:
    op_two_swap(stack);
    break;
  case TOKEN_OVER:
    op_over(stack);
    break;
  case TOKEN_TWO_OVER:
    op_two_over(stack);
    break;
  case TOKEN_ROT:
    op_rot(stack);
    break;
  case TOKEN_PEEK:
    op_peek(stack);
    break;
  case TOKEN_IDENTIFIER:
    op_macro(stack, loop_stack, end_stack, scanner, hashmap,
        memory, token);
    break;
  default:
    fprintf(stderr, "[aurum](%d:%d) unknown token type: %s\n",
        scanner->line, scanner->column, token->lexeme);
    exit(1);
  }
}

static void op_string_literal(struct stack *stack, char *memory,
    const struct token *token)
{
  size_t memory_index;
  size_t string_length;
  char *string;
  char *literal;
  size_t literal_length;
  size_t i;

  memory_index = 0;
  string_length = strlen(token->lexeme);
  string = malloc((string_length + 1) * sizeof(char));
  strcpy(string, token->lexeme);

  literal = string + 1;
  literal[string_length - 2] = '\0';

  literal_length = strlen(literal);
  for (i = 0; i < literal_length; ++i) {
    if (literal[i] == '\\') {
      switch (literal[i + 1]) {
      case 'n':
        memory[memory_index++] = '\n';
        i++;
        break;
      case 't':
        memory[memory_index++] = '\t';
        i++;
        break;
      default:
        memory[memory_index++] = literal[i];
        break;
      }
    } else {
      memory[memory_index++] = literal[i];
    }
  }
  memory[memory_index] = '\0';

#ifdef DEBUG
  fprintf(stderr, "[debug] string_literal: wrote '%s' to memory[0-%d], pushing length=%d, start=%d\n",
      memory, memory_index-1, memory_index, 0);
  fprintf(stderr, "[debug] string_literal: stack before push has %d items\n", stack->size);
#endif
  stack_push(stack, (intptr_t)memory_index);
  stack_push(stack, 0);
#ifdef DEBUG
  fprintf(stderr, "[debug] string_literal: stack after push has %d items\n", stack->size);
#endif

  free(string);
}

void run_interpreter(const char *source_code)
{
  struct scanner scanner;
  struct token token;
  struct stack *stack;
  struct stack *loop_stack;
  struct stack *end_stack;
  struct hashmap *hashmap;
  char *memory;

  stack = stack_create();
  loop_stack = stack_create();
  end_stack = stack_create();
  hashmap = hashmap_create();
  memory = memory_create(MEMORY_CAPACITY);

  init_scanner(&scanner, source_code);

  while ((token = scan_token(&scanner)).type != TOKEN_EOF) {
#ifdef DEBUG
    fprintf(stderr, "[debug] main_loop: executing type=%d, lexeme='%s'\n",
        token.type, token.lexeme);
#endif
    execute_token(stack, loop_stack, end_stack, &scanner, hashmap,
        memory, &token);
  }

  while (stack->size > 0)
    stack_pop(stack);
  free(stack);
  free(loop_stack);
  free(end_stack);
  hashmap_free(hashmap);
  free(memory);
}

