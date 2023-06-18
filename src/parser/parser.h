#ifndef _parser_h_
#define _parser_h_

#include "scanner.h"

/**
 * @brief The main entry point for parsing the input and returns the root node of the AST.
 * @param scanner The scanner to read tokens from.
 * @return The root node of the parsed AST.
 */
ASTNode *parse(Scanner *scanner);

/**
 * @brief Parse an error message for the given scanner.
 * @param scanner The scanner to read tokens from.
 * @param msg The error message to display.
 */
void parse_error(Scanner *scanner, const char *msg);

/**
 * @brief Consume the next token from the scanner and advance the scanner.
 * @param scanner The scanner to read tokens from.
 */
void consume_token(Scanner *scanner);

/**
 * @brief Parse a block node containing statements.
 * @param scanner The scanner to read tokens from.
 * @return The parsed block node.
 */
ASTNode *block(Scanner *scanner);

/**
 * @brief Parse an if_expression node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed if_expression node.
 */
ASTNode *if_expression(Scanner *scanner);

/**
 * @brief Parse a for_expression node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed for_expression node.
 */
ASTNode *for_expression(Scanner *scanner);

/**
 * @brief Parse a while_expression node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed while_expression node.
 */
ASTNode *while_expression(Scanner *scanner);

/**
 * @brief Parse a factor node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed factor node.
 */
ASTNode *factor(Scanner *scanner);

/**
 * @brief Parse a term node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed term node.
 */
ASTNode *term(Scanner *scanner);

/**
 * @brief Parse a bitwise_term node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed bitwise_term node.
 */
ASTNode *bitwise_term(Scanner *scanner);

/**
 * @brief Parse a shift_term node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed shift_term node.
 */
ASTNode *shift_term(Scanner *scanner);

/**
 * @brief Parse an expression node.
 * @param scanner The scanner to read tokens from.
 */
void expression(Scanner *scanner);

/**
 * @brief Parse a comparison node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed comparison node.
 */
ASTNode *comparison(Scanner *scanner);

/**
 * @brief Parse a statement node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed statement node.
 */
ASTNode *statement(Scanner *scanner);

/** * @brief Parse a const_declaration node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed const_declaration node.
 */
ASTNode *parse_const_declaration(Scanner *scanner);

/**
 * @brief Parse a variable_declaration node.
 * @param scanner The scanner to read tokens from.
 * @return The parsed variable_declaration node.
 */
ASTNode *parse_variable_declaration(Scanner *scanner);

/**
 * @brief Check if the given token type is a comparison operator.
 * @param type The token type to check.
 * @return 1 if the token type is a comparison operator, 0 otherwise.
 */
bool is_comparison_operator(TokenType type);

#endif // _parser_h_
