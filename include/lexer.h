#ifndef _lexer_h_
#define _lexer_h_

/**
 * @brief Token types.
 */
typedef enum {
  TOKEN_UNKNOWN,
  TOKEN_NUMBER,
  TOKEN_IDENTIFIER,
  TOKEN_STRING_LITERAL,
  TOKEN_CHAR,
  TOKEN_EOF,
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
  TOKEN_MEMORY,
  TOKEN_WRITE,
  TOKEN_READ,
  TOKEN_STORE,
  TOKEN_FETCH,
  TOKEN_WHILE,
  TOKEN_ELSE,
  TOKEN_IF,
  TOKEN_END,
  TOKEN_THEN,
  TOKEN_DO,
  TOKEN_ADD,
  TOKEN_SUBTRACT,
  TOKEN_MULTIPLY,
  TOKEN_EQUAL,
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
  TOKEN_NEW_LINE,
  TOKEN_CARRIAGE_RETURN,
  TOKEN_SYSCALL,
  TOKEN_DUMP,
  TOKEN_PERIOD,
  TOKEN_DEFINE,
  TOKEN_MACRO,
  TOKEN_INCLUDE,
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

