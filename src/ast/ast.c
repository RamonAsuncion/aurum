#include "ast.h"
#include <stdlib.h>

ASTNode *new_identifier_node(const char *lexeme, TokenType type)
{
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
  node->type = type;
  node->value = strdup(lexeme);
  return node;
}

ASTNode *new_block_node(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *block_node = (ASTNode *)malloc(sizeof(ASTNode));
    block_node->left = condition;
    block_node->right = then_branch;
    return block_node;
}

ASTNode *new_void_node() {
    ASTNode *void_node = (ASTNode *)malloc(sizeof(ASTNode));
    void_node->left = NULL;
    void_node->right = NULL;
    return void_node;
}

ASTNode *add_child_to_node(ASTNode *parent, ASTNode *child) {
    if (parent->left == NULL) {
        parent->left = child;
    } else {
        parent->right = child;
    }
    return parent;
}

ASTNode *new_if_expression_node(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *if_node = (ASTNode *)malloc(sizeof(ASTNode));
    if_node->left = condition;
    if_node->right = then_branch;
    if_node->right->right = else_branch;
    return if_node;
}

ASTNode *new_while_expression_node(ASTNode *condition, ASTNode *body) {
    ASTNode *while_node = (ASTNode *)malloc(sizeof(ASTNode));
    while_node->left = condition;
    while_node->right = body;
    return while_node;
}

ASTNode *new_for_expression_node(ASTNode *initializer, ASTNode *condition, ASTNode *increment, ASTNode *body) {
    ASTNode *for_node = (ASTNode *)malloc(sizeof(ASTNode));
    for_node->left = initializer;
    for_node->right = condition;
    for_node->right->right = increment;
    for_node->right->right->right = body;
    return for_node;
}

ASTNode *new_foreach_expression_node(ASTNode *variable, ASTNode *iterable, ASTNode *body) {
    ASTNode *foreach_node = (ASTNode *)malloc(sizeof(ASTNode));
    foreach_node->left = variable;
    foreach_node->right = iterable;
    foreach_node->right->right = body;
    return foreach_node;
}

ASTNode *new_try_except_expression_node(ASTNode *try_body, ASTNode *exception_variable, ASTNode *except_body) {
    ASTNode *try_except_node = (ASTNode *)malloc(sizeof(ASTNode));
    try_except_node->left = try_body;
    try_except_node->right = exception_variable;
    try_except_node->right->right = except_body;
    return try_except_node;
}

ASTNode *new_boolean_node(const char *value) {
    ASTNode *boolean_node = (ASTNode *)malloc(sizeof(ASTNode));
    boolean_node->left = NULL;
    boolean_node->right = NULL;
    boolean_node->value = value;
    return boolean_node;
}

ASTNode *new_binary_operation_node(TokenType operator, ASTNode *left, ASTNode *right) {
    ASTNode *binary_operation_node = (ASTNode *)malloc(sizeof(ASTNode));
    binary_operation_node->left = left;
    binary_operation_node->right = right;
    binary_operation_node->type = operator;
    return binary_operation_node;
}

ASTNode *new_number_node(const char *value, TokenType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode *new_variable_declaration_node(TokenType type, const char *name, ASTNode *initializer) {
    ASTNode *variable_declaration_node = (ASTNode *)malloc(sizeof(ASTNode));
    variable_declaration_node->left = initializer;
    variable_declaration_node->right = NULL;
    variable_declaration_node->type = type;
    variable_declaration_node->value = name;
    return variable_declaration_node;
}

ASTNode *new_const_declaration_node(TokenType type, const char *name, ASTNode *initializer) {
    ASTNode *const_declaration_node = (ASTNode *)malloc(sizeof(ASTNode));
    const_declaration_node->left = initializer;
    const_declaration_node->right = NULL;
    const_declaration_node->type = type;
    const_declaration_node->value = name;
    return const_declaration_node;
}

ASTNode *new_return_node(ASTNode *expression) {
    ASTNode *return_node = (ASTNode *)malloc(sizeof(ASTNode));
    return_node->left = expression;
    return_node->right = NULL;
    return return_node;
}

ASTNode *new_break_node() {
    ASTNode *break_node = (ASTNode *)malloc(sizeof(ASTNode));
    break_node->left = NULL;
    break_node->right = NULL;
    return break_node;
}

ASTNode *new_continue_node() {
    ASTNode *continue_node = (ASTNode *)malloc(sizeof(ASTNode));
    continue_node->left = NULL;
    continue_node->right = NULL;
    return continue_node;
}

void destroy_ast_node(ASTNode *node) {
    if (node == NULL) return;
    destroy_ast_node(node->left);
    destroy_ast_node(node->right);
    free(node);
}

void generate_dot_node(ASTNode *node, FILE *out) {
    switch (node->type) {
        case TOKEN_NUMBER:
            fprintf(out, "n%p [label=\"%d\"];\n", (void *)node, atoi(node->value));
            break;
        case TOKEN_ADD:
            fprintf(out, "n%p [label=\"+\"];\n", (void *)node);
            generate_dot_node(node->left, out);
            generate_dot_node(node->right, out);
            fprintf(out, "n%p -> n%p;\n", (void *)node, (void *)node->left);
            fprintf(out, "n%p -> n%p;\n", (void *)node, (void *)node->right);
            break;
        case TOKEN_MULTIPLY:
            fprintf(out, "n%p [label=\"*\"];\n", (void *)node);
            generate_dot_node(node->left, out);
            generate_dot_node(node->right, out);
            fprintf(out, "n%p -> n%p;\n", (void *)node, (void *)node->left);
            fprintf(out, "n%p -> n%p;\n", (void *)node, (void *)node->right);
            break;
        case TOKEN_DIVIDE:
            fprintf(out, "n%p [label=\"/\"];\n", (void *)node);
            generate_dot_node(node->left, out);
            generate_dot_node(node->right, out);
            fprintf(out, "n%p -> n%p;\n", (void *)node, (void *)node->left);
            fprintf(out, "n%p -> n%p;\n", (void *)node, (void *)node->right);
            break;
        case TOKEN_SUBTRACT:
            fprintf(out, "n%p [label=\"-\"];\n", (void *)node);
            generate_dot_node(node->left, out);
            generate_dot_node(node->right, out);
            fprintf(out, "n%p -> n%p;\n", (void *)node, (void *)node->left);
            fprintf(out, "n%p -> n%p;\n", (void *)node, (void *)node->right);
            break;
        case TOKEN_IDENTIFIER:
            fprintf(out, "n%p [label=\"%s\"];\n", (void *)node, node->value);
            break;
        default:
            break;
    }
}

void generate_dot(ASTNode *node, FILE *out) {
    fprintf(out, "digraph G {\n");
    generate_dot_node(node, out);
    fprintf(out, "}\n");
}