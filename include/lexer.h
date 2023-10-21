#ifndef _lexer_h_
#define _lexer_h_

/**
 * @brief Token types.
 */
typedef enum {
  // Basic tokens
  TOKEN_UNKNOWN,
  TOKEN_NUMBER,
  TOKEN_IDENTIFIER,
  TOKEN_STRING_LITERAL,
  TOKEN_CHAR,
  TOKEN_EOF,
  // Stack operations
  TOKEN_DUP,
  TOKEN_TWO_DUP,
  TOKEN_SWAP,
  TOKEN_TWO_SWAP,
  TOKEN_OVER,
  TOKEN_TWO_OVER,
  TOKEN_ROT,
  TOKEN_DROP,
  TOKEN_TWO_DROP,
  TOKEN_PEEK,
  // Memory operations
  TOKEN_MEMORY,
  TOKEN_WRITE,
  TOKEN_READ,
  TOKEN_STORE,
  TOKEN_FETCH,
  // Control flow
  TOKEN_WHILE,
  TOKEN_ELSE,
  TOKEN_IF,
  TOKEN_END,
  TOKEN_THEN,
  TOKEN_DO,
  // Arithmetic operations
  TOKEN_ADD,
  TOKEN_SUBTRACT,
  TOKEN_MULTIPLY,
  // Relational operations
  TOKEN_EQUAL, 
  TOKEN_LESS, 
  TOKEN_LESS_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL, 
  // Bitwise operations
  TOKEN_BITWISE_AND,
  TOKEN_BITWISE_OR, 
  TOKEN_BITWISE_XOR,
  TOKEN_BITWISE_NOT, 
  TOKEN_LEFT_SHIFT, 
  TOKEN_RIGHT_SHIFT, 
  // Escape sequences
  TOKEN_ESCAPE_SEQUENCE,
  TOKEN_NEW_LINE,
  TOKEN_CARRIAGE_RETURN,
  // Kernel
  TOKEN_SYSCALL,
  // Print stack
  TOKEN_DUMP,
  TOKEN_PERIOD,
  // Preprocessor
  TOKEN_DEFINE,
  TOKEN_MACRO,
  TOKEN_INCLUDE,
} TokenType;

/**
 * @brief Token structure.
 */
struct Token {
  TokenType type;
  const char *lexeme;
  int line;
};

typedef struct Token Token;

/**
 * @brief Trie Data structure.
 */
typedef struct TrieNode {
  TokenType type;
  struct TrieNode *children[128];
} TrieNode;

#endif // _lexer_h_
