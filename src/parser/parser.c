#include "lexer.h"
#include "scanner.h"
#include "parser.h"
#include "stack.h"

Token current_token;

void parse_error(Scanner *scanner, const char *msg)
{
  fprintf(stderr, "Parse error: %s\n", msg);
  fprintf(stderr, "Unexpected token: %.*s (type: %d)\n", (int)(current_token.lexeme - scanner->start), scanner->start, current_token.type);
  exit(1);
}

static int count_indentation(Scanner *scanner) 
{
  int indentation = 0;
  while (true) {
    char c = peek(scanner);
    if (c == ' ') {
      indentation++;
      advance(scanner);
    } else if (c == '\t') {
      indentation += TAB_WIDTH;
      advance(scanner);
    } else {
      break;
    }
  }
  return indentation;
}

void consume_token(Scanner *scanner)
{
  current_token = scan_token(scanner);
}

void expression(Scanner *scanner, Stack *stack)
{
  term(scanner, stack);
  while (current_token.type == TOKEN_ADD || current_token.type == TOKEN_SUBTRACT) {
    TokenType op = current_token.type;
    consume_token(scanner);
    term(scanner, stack);
    char *right = pop(stack);
    char *left = pop(stack);
    char *result = perform_operation(left, right, op);
    push(stack, result);
  }
}

void term(Scanner *scanner, Stack *stack)
{
  factor(scanner, stack);
  while (current_token.type == TOKEN_MULTIPLY || current_token.type == TOKEN_DIVIDE) {
    TokenType op = current_token.type;
    consume_token(scanner);
    factor(scanner, stack);
    char *right = pop(stack);
    char *left = pop(stack);
    char *result = perform_operation(left, right, op);
    push(stack, result);
  }
}

void factor(Scanner *scanner, Stack *stack)
{
  if (current_token.type == TOKEN_NUMBER) {
    push(stack, current_token.value);
    consume_token(scanner);
  } else if (current_token.type == TOKEN_LPAREN) {
    consume_token(scanner);
    expression(scanner, stack);
    if (current_token.type != TOKEN_RPAREN) {
      parse_error(scanner, "Expected ')'");
    }
    consume_token(scanner);
  } else {
    parse_error(scanner, "Expected number or '('");
  }
}

char *perform_operation(char *left, char *right, TokenType op)
{
  // Perform the operation on the two operands and return the result as a string
}
