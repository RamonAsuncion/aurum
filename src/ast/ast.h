#ifndef _ast_h_
#define _ast_h_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../lexer/lexer.h"

/**
 * @brief Represents an Abstract Syntax Tree node.
 */
typedef struct ASTNode {
    TokenType type;
    const char *value;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

/**
 * @brief Creates a new block node.
 * @return A pointer to the newly created block node.
 */
ASTNode *new_block_node();

/**
 * @brief Creates a new void node.
 * @return A pointer to the newly created void node.
 */
ASTNode *new_void_node();

/**
 * @brief Creates a new AST node with the given token type.
 * @param type The token type for the new AST node.
 * @return A pointer to the newly created AST node.
 */
ASTNode *new_ast_node(TokenType type);

/**
 * @brief Destroys an AST node and frees its memory.
 * @param node The AST node to destroy.
 */
void destroy_ast_node(ASTNode *node);

/**
 * @brief Adds a child node to a parent node.
 * @param parent The parent node.
 * @param child The child node to add.
 */
ASTNode *add_child_to_node(ASTNode *parent, ASTNode *child);

/**
 * @brief Creates a new if_expression node with the given condition, then_branch, and else_branch.
 * @param condition The condition node.
 * @param then_branch The then_branch node.
 * @param else_branch The else_branch node.
 * @return A pointer to the newly created if_expression node.
 */
ASTNode *new_if_expression_node(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch);

/**
 * @brief Creates a new while_expression node with the given condition and body.
 * @param condition The condition node.
 * @param body The body node.
 * @return A pointer to the newly created while_expression node.
 */
ASTNode *new_while_expression_node(ASTNode *condition, ASTNode *body);

/**
 * @brief Creates a new for_expression node with the given initializer, condition, increment, and body.
 * @param initializer The initializer node.
 * @param condition The condition node.
 * @param increment The increment node.
 * @param body The body node.
 * @return A pointer to the newly created for_expression node.
 */
ASTNode *new_for_expression_node(ASTNode *initializer, ASTNode *condition, ASTNode *increment, ASTNode *body);

/** 
 * @brief Creates a new foreach_expression node with the given variable, iterable, and body.
 * @param variable The variable node.
 * @param iterable The iterable node.
 * @param body The body node.
 * @return A pointer to the newly created foreach_expression node.
 */
ASTNode *new_foreach_expression_node(ASTNode *variable, ASTNode *iterable, ASTNode *body);

/**
 * @brief Creates a new try_except_expression node with the given try_body, exception_variable, and except_body.
 * @param try_body The try_body node.
 * @param exception_variable The exception_variable node.
 * @param except_body The except_body node.
 * @return A pointer to the newly created try_except_expression node.
 */
ASTNode *new_try_except_expression_node(ASTNode *try_body, ASTNode *exception_variable, ASTNode *except_body);

/**
 * @brief Creates a new boolean_node with the given value.
 * @param value The value to store in the boolean_node.
 * @return A pointer to the newly created boolean_node.
 */
ASTNode *new_boolean_node(const char *value);

/**
 * @brief Creates a new binary_operation_node with the given operator and left and right children.
 * @param operator The operator to use.
 * @param left The left child node.
 * @param right The right child node.
 * @return A pointer to the newly created binary_operation_node.
 */
ASTNode *new_binary_operation_node(TokenType operator, ASTNode *left, ASTNode *right);

/**
 * @brief Creates a new number_node with the given value and type.
 * @param value The value to store in the number_node.
 * @param type The type of the number_node.
 * @return A pointer to the newly created number_node.
 */
ASTNode *new_number_node(const char *value, TokenType type);

/**
 * @brief Creates a new identifier_node with the given name and type.
 * @param name The name of the identifier_node.
 * @param type The type of the identifier_node.
 * @return A pointer to the newly created identifier_node.
 */
ASTNode *new_identifier_node(const char *name, TokenType type);

/**
 * @brief Creates a new const_declaration node with the given type, name, and initializer.
 * @param type The type of the const_declaration node.
 * @param name The name of the const_declaration node.
 * @param initializer The initializer node.
 * @return A pointer to the newly created const_declaration node.
 */
ASTNode *new_const_declaration_node(TokenType type, const char *name, ASTNode *initializer);

/**
 * @brief Creates a new variable_declaration node with the given type, name, and initializer.
 * @param type The type of the variable_declaration node.
 * @param name The name of the variable_declaration node.
 * @param initializer The initializer node.
 * @return A pointer to the newly created variable_declaration node.
 */
ASTNode *new_variable_declaration_node(TokenType type, const char *name, ASTNode *initializer);

/**
 * @brief Generates the DOT representation of the given AST node.
 * @param node A pointer to the root node of the AST.
 * @param out A pointer to the output file stream where the DOT representation will be written.
 */
void generate_dot(ASTNode *node, FILE *out);

#endif
