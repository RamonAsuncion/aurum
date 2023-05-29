#ifndef _lexer_h_
#define _lexer_h_

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Token types.
 */
typedef enum {
  TOKEN_UNKNOWN,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_SEMICOLON,
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_ASSIGN,
  TOKEN_ASSIGN_ADD,
  TOKEN_EOF,
  TOKEN_CONTINUE,
  TOKEN_FOR,
  TOKEN_WHILE,
  TOKEN_ELSE,
  TOKEN_ELIF,
  TOKEN_TRY,
  TOKEN_CATCH,
  TOKEN_THROW,
  TOKEN_CONST,
  TOKEN_RETURN,
  TOKEN_IF,
  TOKEN_BREAK,
  TOKEN_UNION,
  TOKEN_STATIC,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_ALIAS,
  TOKEN_VOID,
  TOKEN_FOREACH,
  TOKEN_RANGE,
  TOKEN_IN,
  TOKEN_SWITCH,
  TOKEN_CASE,
  TOKEN_DEFAULT,
  TOKEN_GOTO,
  TOKEN_I8,
  TOKEN_U8,
  TOKEN_I16,
  TOKEN_U16,
  TOKEN_I32,
  TOKEN_U32,
  TOKEN_I64,
  TOKEN_U64,
  TOKEN_F32,
  TOKEN_F64,
  TOKEN_ADD,
  TOKEN_SUBTRACT,
  TOKEN_MULTIPLY,
  TOKEN_DIVIDE,
  TOKEN_MODULO,
  TOKEN_EQUAL,
  TOKEN_NOT_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_INCREMENT,
  TOKEN_DECREMENT,
  TOKEN_BITWISE_AND,
  TOKEN_BITWISE_OR,
  TOKEN_BITWISE_XOR,
  TOKEN_BITWISE_NOT,
  TOKEN_LEFT_SHIFT,
  TOKEN_RIGHT_SHIFT,
  TOKEN_ESCAPE_SEQUENCE,
  TOKEN_LBRACE,
  TOKEN_RBRACE
} TokenType;

/**
 * @brief Token structure.
 */
typedef struct {
  TokenType type;
  const char *lexeme;
  int line;
} Token;

/**
 * @brief Trie Data structure.
 */
typedef struct TrieNode {
  TokenType type;
  struct TrieNode *children[128];
} TrieNode;

#endif
