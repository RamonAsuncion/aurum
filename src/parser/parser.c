#include "../lexer/lexer.h"
#include "../lexer/scanner.h"
#include "parser.h"
#include "../ast/ast.h"

/*
try_except_expression is not being called anywehere.
parse_variable and parse_const need fixing.
the assignment operator only works when there is a type declaration.a
*/

Token current_token;

void parse_error(Scanner *scanner, const char *msg)
{
  fprintf(stderr, "Parse error: %s\n", msg);
  fprintf(stderr, "Unexpected token: %.*s (type: %d)\n", (int)(current_token.lexeme - scanner->start), scanner->start, current_token.type);
  exit(1);
}

void consume_token(Scanner *scanner)
{
  current_token = scan_token(scanner);
}

ASTNode *block(Scanner *scanner)
{
  if (current_token.type != TOKEN_LBRACE) {
    parse_error(scanner, "Expected '{'");
  }
  consume_token(scanner); // consume '{'

  ASTNode *block_node = new_block_node();
  while (current_token.type != TOKEN_RBRACE && current_token.type != TOKEN_EOF) {
    ASTNode *statement_node = statement(scanner);
    add_child_to_node(block_node, statement_node);
  }

  if (current_token.type != TOKEN_RBRACE) {
    parse_error(scanner, "Expected '}'");
  }
  consume_token(scanner); // consume '}'

  return block_node;
}

ASTNode *if_expression(Scanner *scanner)
{
  ASTNode *node = NULL;
  consume_token(scanner); // consume "if"
  if (current_token.type != TOKEN_LPAREN) {
    parse_error(scanner, "Expected '('");
  }
  consume_token(scanner); // consume '('
  ASTNode *condition = expression(scanner);
  if (current_token.type != TOKEN_RPAREN) {
    parse_error(scanner, "Expected ')'");
  }
  consume_token(scanner); // consume ')'
  ASTNode *true_branch = block(scanner);
  ASTNode *false_branch = NULL;
  if (current_token.type == TOKEN_ELSE) {
    consume_token(scanner); // consume "else"
    false_branch = block(scanner);
  }
  node = new_if_expression_node(condition, true_branch, false_branch);
  return node;
}

ASTNode *for_expression(Scanner *scanner)
{
  ASTNode *node = NULL;
  consume_token(scanner); // consume "for"
  if (current_token.type != TOKEN_LPAREN) {
    parse_error(scanner, "Expected '('");
  }
  consume_token(scanner); // consume '('

  ASTNode *initialization = statement(scanner); // parse initialization statement
  ASTNode *condition = expression(scanner); // parse loop condition
  if (current_token.type != TOKEN_SEMICOLON) {
    parse_error(scanner, "Expected ';'");
  }
  consume_token(scanner); // consume ';'
  ASTNode *update = expression(scanner); // parse update expression

  if (current_token.type != TOKEN_RPAREN) {
    parse_error(scanner, "Expected ')'");
  }
  consume_token(scanner); // consume ')'

  ASTNode *body = block(scanner); // parse loop body
  node = new_for_expression_node(initialization, condition, update, body);
  return node;
}

ASTNode *while_expression(Scanner *scanner)
{
  ASTNode *node = NULL;
  consume_token(scanner); // consume "while"
  if (current_token.type != TOKEN_LPAREN) {
    parse_error(scanner, "Expected '('");
  }
  consume_token(scanner); // consume '('

  ASTNode *condition = expression(scanner); // parse loop condition

  if (current_token.type != TOKEN_RPAREN) {
    parse_error(scanner, "Expected ')'");
  }
  consume_token(scanner); // consume ')'

  ASTNode *body = block(scanner); // parse loop body
  node = new_while_expression_node(condition, body);
  return node;
}

TokenType parse_type(Scanner *scanner) {
    TokenType data_type = current_token.type;
    switch (data_type) {
        case TOKEN_I8:
        case TOKEN_U8:
        case TOKEN_I16:
        case TOKEN_U16:
        case TOKEN_I32:
        case TOKEN_U32:
        case TOKEN_I64:
        case TOKEN_U64:
        case TOKEN_F32:
        case TOKEN_F64:
            break;
        default:
            parse_error(scanner, "Invalid data type");
            break;
    }
    return data_type;
}

ASTNode *parse_variable_declaration(Scanner *scanner) 
{
  TokenType data_type = parse_type(scanner);
  consume_token(scanner); // Consume the identifier
  Token identifier_token = current_token;

  if (current_token.type != TOKEN_IDENTIFIER) {
    parse_error(scanner, "Expected identifier in variable declaration");
  }
  consume_token(scanner); // Consume the identifier

  ASTNode *express = NULL;
  if (current_token.type == TOKEN_ASSIGN) {
    consume_token(scanner); // Consume '='
    express = expression(scanner);
  }

  // Check for the semicolon
  if (current_token.type == TOKEN_SEMICOLON) {
    consume_token(scanner); // Consume ';'
  } else {
    parse_error(scanner, "Missing semicolon in variable declaration"); // Report an error: missing semicolon
  }
  return new_variable_declaration_node(data_type, identifier_token.lexeme, express);
}

ASTNode *parse_const_declaration(Scanner *scanner)
{
  consume_token(scanner); // Consume 'const'
  TokenType data_type = parse_type(scanner);
  consume_token(scanner); // Consume the identifier
  Token identifier_token = current_token;
  consume_token(scanner); // Consume '='
  ASTNode *express = expression(scanner);
  // Check for the semicolon
  if (current_token.type == TOKEN_SEMICOLON) {
    consume_token(scanner); // Consume ';'
  } else {
    // Report an error: missing semicolon
    parse_error(scanner, "Missing semicolon in variable declaration");
  }
  return new_const_declaration_node(data_type, identifier_token.lexeme, express);
}

ASTNode *foreach_expression(Scanner *scanner)
{
  ASTNode *node = NULL;
  consume_token(scanner); // consume "foreach"
  if (current_token.type != TOKEN_LPAREN) {
    parse_error(scanner, "Expected '('");
  }
  consume_token(scanner); // consume '('

  ASTNode *variable = parse_variable_declaration(scanner); // parse variable declaration
  if (current_token.type != TOKEN_IN) {
    parse_error(scanner, "Expected 'in'");
  }
  consume_token(scanner); // consume "in"

  ASTNode *iterable = expression(scanner); // parse iterable expression

  if (current_token.type != TOKEN_RPAREN) {
    parse_error(scanner, "Expected ')'");
  }
  consume_token(scanner); // consume ')'

  ASTNode *body = block(scanner); // parse loop body
  node = new_foreach_expression_node(variable, iterable, body);
  return node;
}

ASTNode *try_except_expression(Scanner *scanner)
{
  ASTNode *node = NULL;
  consume_token(scanner); // consume "try"
  ASTNode *try_block = block(scanner); // parse the try block

  if (current_token.type != TOKEN_CATCH) {
    parse_error(scanner, "Expected 'catch'");
  }
  consume_token(scanner); // consume "catch"

  ASTNode *exception_variable = NULL;
  if (current_token.type == TOKEN_IDENTIFIER) {
    exception_variable = parse_variable_declaration(scanner); // parse exception variable
  }
  consume_token(scanner); // consume exception variable

  if (current_token.type != TOKEN_RPAREN) {
    parse_error(scanner, "Expected ')'");
  }
  consume_token(scanner); // consume ')'

  ASTNode *except_block = block(scanner); // parse the except block

  node = new_try_except_expression_node(try_block, exception_variable, except_block);
  return node;
}

ASTNode *factor(Scanner *scanner)
{
  ASTNode *node = NULL;
  switch (current_token.type) {
    case TOKEN_NUMBER:
      node = new_number_node(current_token.lexeme, current_token.type);
      consume_token(scanner);
      break;
    case TOKEN_IDENTIFIER:
      node = new_identifier_node(current_token.lexeme, current_token.type);
      consume_token(scanner);
      break;
    case TOKEN_LPAREN:
      consume_token(scanner);
      node = expression(scanner);
      if (current_token.type != TOKEN_RPAREN) {
        parse_error(scanner, "Expected ')'");
      }
      consume_token(scanner);
      break;
    case TOKEN_TRUE:
      node = new_boolean_node("true");
      consume_token(scanner);
      break;
    case TOKEN_FALSE:
      node = new_boolean_node("false");
      consume_token(scanner);
      break;
    case TOKEN_VOID:
      node = new_void_node();
      consume_token(scanner);
      break;
    default:
      parse_error(scanner, "Unexpected token in factor.");
  }
  return node;
}

/*
 * term function: handles multiplication, division, and other
 * lower-precedence operations by calling the factor function.
 */
ASTNode *term(Scanner *scanner)
{
  ASTNode *node = factor(scanner);
  while (current_token.type == TOKEN_MULTIPLY || current_token.type == TOKEN_DIVIDE || current_token.type == TOKEN_MODULO) {
    TokenType op = current_token.type;
    consume_token(scanner);
    ASTNode *right = factor(scanner);
    node = new_binary_operation_node(op, node, right);
  }
  return node;
}

ASTNode *bitwise_term(Scanner *scanner)
{
  ASTNode *node = term(scanner);
  while (current_token.type == TOKEN_BITWISE_AND || current_token.type == TOKEN_BITWISE_OR || current_token.type == TOKEN_BITWISE_XOR) {
    TokenType op = current_token.type;
    consume_token(scanner);
    ASTNode *right = term(scanner);
    node = new_binary_operation_node(op, node, right);
  }
  return node;
}

ASTNode *shift_term(Scanner *scanner)
{
  ASTNode *node = bitwise_term(scanner);
  while (current_token.type == TOKEN_LEFT_SHIFT || current_token.type == TOKEN_RIGHT_SHIFT) {
    TokenType op = current_token.type;
    consume_token(scanner);
    ASTNode *right = bitwise_term(scanner);
    node = new_binary_operation_node(op, node, right);
  }
  return node;
}

/*
 * expression function: handles addition, subtraction, and other
 * higher-precedence operations by calling the term function.
 */
ASTNode *expression(Scanner *scanner)
{
  ASTNode *node = term(scanner);
  while (current_token.type == TOKEN_ADD || current_token.type == TOKEN_SUBTRACT) {
    TokenType op = current_token.type;
    consume_token(scanner);
    ASTNode *right = term(scanner);
    node = new_binary_operation_node(op, node, right);
  }
  return node;
}

bool is_comparison_operator(TokenType type)
{
  return type == TOKEN_EQUAL || type == TOKEN_NOT_EQUAL || type == TOKEN_LESS || type == TOKEN_LESS_EQUAL || type == TOKEN_GREATER || type == TOKEN_GREATER_EQUAL;
}

bool is_logical_operator(TokenType type)
{
  return type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_NOT;
}

/*
 * comparison function: handles comparison operators like
 * greater than, less than, equal to, etc. by calling the expression function.
 */
ASTNode *comparison(Scanner *scanner)
{
  ASTNode *node = expression(scanner);
  while (is_comparison_operator(current_token.type)) {
    TokenType op = current_token.type;
    consume_token(scanner);
    ASTNode *right = expression(scanner);
    node = new_binary_operation_node(op, node, right);
  }
  return node;
}

/*
 * comparison function: handles comparison operators like
 * greater than, less than, equal to, etc. by calling the expression function.
 */
ASTNode *logical(Scanner *scanner)
{
  ASTNode *node = comparison(scanner);
  while (is_logical_operator(current_token.type)) {
    TokenType op = current_token.type;
    consume_token(scanner);
    ASTNode *right = comparison(scanner);
    node = new_binary_operation_node(op, node, right);
  }
  return node;
}

ASTNode *statement(Scanner *scanner)
{
  ASTNode *node = NULL;
  switch (current_token.type) {
    case TOKEN_IF:
      node = if_expression(scanner);
      break;
    case TOKEN_FOR:
      node = for_expression(scanner);
      break;
    case TOKEN_WHILE:
      node = while_expression(scanner);
      break;
    case TOKEN_CONST:
      node = parse_const_declaration(scanner);
      break;
    case TOKEN_FOREACH:
      node = foreach_expression(scanner);
      break;
    case TOKEN_TRY:
      node = try_except_expression(scanner);
      break;
    default:
      parse_error(scanner, "Unexpected token in statement.");
  }
  return node;
}

ASTNode *parse(Scanner *scanner)
{
  consume_token(scanner);
  ASTNode *root = expression(scanner);
  if (current_token.type != TOKEN_EOF) {
    parse_error(scanner, "Unexpected token after expression");
  }
  return root;
}
