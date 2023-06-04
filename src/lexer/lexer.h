#ifndef _lexer_h_
#define _lexer_h_

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/**
 * @file lexer.h
 * @brief Lexer implementation.
*/

/**
 * @brief Token types.
 */
typedef enum {
  TOKEN_DUP,
  TOKEN_SWAP,
  TOKEN_POP,
  TOKEN_ROLL,
  TOKEN_OVER,
  TOKEN_ROT,
  TOKEN_FUNC,
  TOKEN_DROP,
  TOKEN_PEEK,
  TOKEN_PTR,
  TOKEN_ADDR,
  TOKEN_MEMORY,
  TOKEN_WRITE,
  TOKEN_READ,
  TOKEN_STORE,
  TOKEN_LOAD,
  TOKEN_SET,
  TOKEN_REQUIRE,
  TOKEN_UNKNOWN,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_EOF,
  TOKEN_FOR,
  TOKEN_WHILE,
  TOKEN_ELSE,
  TOKEN_ELIF,
  TOKEN_CONST,
  TOKEN_IF,
  TOKEN_BREAK,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_IN,
  TOKEN_INT, // i64
  TOKEN_FLOAT, // f64
  TOKEN_WHITESPACE,
  TOKEN_BOOL, 
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
  TOKEN_BITWISE_AND,
  TOKEN_BITWISE_OR, 
  TOKEN_BITWISE_XOR,
  TOKEN_BITWISE_NOT, 
  TOKEN_LEFT_SHIFT, 
  TOKEN_RIGHT_SHIFT, 
  TOKEN_ESCAPE_SEQUENCE, 
  TOKEN_SINGLE_QUOTE,
  TOKEN_DOUBLE_QUOTE,
  TOKEN_NEW_LINE,
  TOKEN_CARRIAGE_RETURN,
  TOKEN_END,
  TOKEN_THEN,
  TOKEN_AS,
  TOKEN_PERIOD
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

#endif // _lexer_h_
