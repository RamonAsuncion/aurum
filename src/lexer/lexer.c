#include "lexer.h"
#include "scanner.h"

TrieNode *trie_node_create(TokenType type)
{
  TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
  node->type = type;
  for (int i = 0; i < 128; ++i) {
    node->children[i] = NULL;
  }
  return node;
}

void trie_insert(TrieNode *root, const char *keyword, TokenType type)
{
  TrieNode *node = root;
  while (*keyword) {
    if (node->children[*keyword] == NULL) {
      node->children[*keyword] = trie_node_create(TOKEN_IDENTIFIER);
    }
    node = node->children[*keyword];
    keyword++;
  }
  node->type = type;
}

TrieNode *init_keyword_trie()
{
  TrieNode *root = trie_node_create(TOKEN_IDENTIFIER);
  trie_insert(root, "for", TOKEN_FOR);
  trie_insert(root, "while", TOKEN_WHILE);
  trie_insert(root, "else", TOKEN_ELSE);
  trie_insert(root, "final", TOKEN_CONST);
  trie_insert(root, "if", TOKEN_IF);
  trie_insert(root, "break", TOKEN_BREAK);
  trie_insert(root, "true", TOKEN_TRUE);
  trie_insert(root, "false", TOKEN_FALSE);
  trie_insert(root, "in", TOKEN_IN);
  trie_insert(root, "dup", TOKEN_DUP);
  trie_insert(root, "swap", TOKEN_SWAP);
  trie_insert(root, "roll", TOKEN_ROLL);
  trie_insert(root, "over", TOKEN_OVER);
  trie_insert(root, "peek", TOKEN_PEEK);
  trie_insert(root, "ptr", TOKEN_PTR);
  trie_insert(root, "addr", TOKEN_ADDR);
  trie_insert(root, "memory", TOKEN_MEMORY);
  trie_insert(root, "write", TOKEN_WRITE);
  trie_insert(root, "read", TOKEN_READ);
  trie_insert(root, "store", TOKEN_STORE);
  trie_insert(root, "load", TOKEN_LOAD);
  trie_insert(root, "set", TOKEN_SET);
  trie_insert(root, "func", TOKEN_FUNC);
  trie_insert(root, "int", TOKEN_INT);
  trie_insert(root, "float", TOKEN_FLOAT);
  trie_insert(root, "bool", TOKEN_BOOL);
  trie_insert(root, "pop", TOKEN_POP);
  trie_insert(root, "rot", TOKEN_ROT);
  trie_insert(root, "require", TOKEN_REQUIRE);
  trie_insert(root, "drop", TOKEN_DROP);
  trie_insert(root, "rshft", TOKEN_RIGHT_SHIFT);
  trie_insert(root, "lshft", TOKEN_LEFT_SHIFT);
  trie_insert(root, "end", TOKEN_END);
  trie_insert(root, "then", TOKEN_THEN);
  trie_insert(root, "as", TOKEN_AS);
  return root;
}

TokenType trie_search(TrieNode *root, Scanner *scanner) {
  TrieNode *node = root;
  const char *source = scanner->start;

  while (node && *source) {
    if (!isalnum(*source)) {
      break;
    }
    node = node->children[*source];
    source++;
  }

  if ((!node || !isalnum(*source)) && node && node->type != TOKEN_IDENTIFIER) {
    return node->type;
  }

  return TOKEN_IDENTIFIER;
}


TokenType check_keyword(Scanner *scanner)
{
  static TrieNode *keyword_trie = NULL;
  if (keyword_trie == NULL) {
    keyword_trie = init_keyword_trie();
  }
  return trie_search(keyword_trie, scanner);
}

void init_scanner(Scanner *scanner, const char *source)
{
  scanner->source = source;
  scanner->start = source;
  scanner->current = source;
  scanner->line = 1;
  scanner->column = 1;
  scanner->start_column = 1;
}

bool is_at_end(Scanner *scanner)
{
  return *scanner->current == '\0';
}

char advance(Scanner *scanner)
{
  scanner->column++;
  if (*scanner->current == '\n') {
    scanner->line++;
    scanner->column = 1;
  }
  return *scanner->current++;
}

char peek(Scanner *scanner)
{
  return *scanner->current;
}

char peek_next(Scanner *scanner)
{
  if (is_at_end(scanner)) return '\0';
  return scanner->current[1];
}

void skip_whitespace(Scanner *scanner)
{
    while (isspace(peek(scanner))) {
        advance(scanner);
    }
}

bool is_whitespace(Scanner *scanner) {
  return isspace(peek(scanner));
}

bool match(Scanner *scanner, char expected)
{
  if (is_at_end(scanner)) return false;
  if (*scanner->current != expected) return false;

  scanner->current++;
  scanner->column++;

  return true;
}

Token create_token(TokenType type, Scanner *scanner)
{
  Token token;
  token.type = type;
  token.line = scanner->line;

  size_t length = (size_t)(scanner->current - scanner->start);
  char *lexeme = (char *)malloc(length + 1);
  strncpy(lexeme, scanner->start, length);
  lexeme[length] = '\0';

  token.lexeme = lexeme;

  return token;
}

Token scan_token(Scanner *scanner)
{
  // if (is_whitespace(scanner)) {
  //   advance(scanner);
  //   return create_token(TOKEN_WHITESPACE, scanner);
  // }

  skip_whitespace(scanner);
  scanner->start = scanner->current;
  scanner->start_column = scanner->column;
  if (is_at_end(scanner)) return create_token(TOKEN_EOF, scanner);

  char c = advance(scanner);
  switch (c) {
    case '*': return create_token(TOKEN_MULTIPLY, scanner);
    case '(': return create_token(TOKEN_LPAREN, scanner);
    case ')': return create_token(TOKEN_RPAREN, scanner);
    case '%': return create_token(TOKEN_MODULO, scanner);
    case '~': return create_token(TOKEN_BITWISE_NOT, scanner);
    case '^': return create_token(TOKEN_BITWISE_XOR, scanner);
    case '\\': return create_token(TOKEN_ESCAPE_SEQUENCE, scanner);
    case '\n': return create_token(TOKEN_NEW_LINE, scanner);
    case '\r': return create_token(TOKEN_CARRIAGE_RETURN, scanner);
    case '\'': return create_token(TOKEN_SINGLE_QUOTE, scanner);
    case '"': return create_token(TOKEN_DOUBLE_QUOTE, scanner);
    case '=': return create_token(TOKEN_EQUAL, scanner);
    //  I think I'm mixing up ! and != because if it's != it should have match.
    case '!': return create_token(TOKEN_NOT_EQUAL, scanner);
    case '+': return create_token(TOKEN_ADD, scanner);
    case '-': return create_token(TOKEN_SUBTRACT, scanner);
    case '&': return create_token(TOKEN_BITWISE_AND, scanner);
    case '|':  return create_token(TOKEN_BITWISE_OR, scanner);
    case '.': return create_token(TOKEN_PERIOD, scanner);
    case '>':
      if (match(scanner, '=')) {
        return create_token(TOKEN_GREATER_EQUAL, scanner);
      } else {
        return create_token(TOKEN_GREATER, scanner);
      }
    case '<':
      if (match(scanner, '=')) {
        return create_token(TOKEN_LESS_EQUAL, scanner);
      } else {
        return create_token(TOKEN_LESS, scanner);
      }
    case '/':
      if (match(scanner, '/')) {
        // Single-line comment
        while (peek(scanner) != '\n' && !is_at_end(scanner)) {
            advance(scanner);
        }
      } else if (match(scanner, '*')) {
          // Multi-line comment
          while (!(peek(scanner) == '*' && peek_next(scanner) == '/') && !is_at_end(scanner)) {
              advance(scanner);
          }
          if (!is_at_end(scanner)) {
              advance(scanner); // Consume '*'
              advance(scanner); // Consume '/'
          }
      } else {
          return create_token(TOKEN_DIVIDE, scanner);
      }
    default:
      if (isdigit(c)) {
        while (isdigit(peek(scanner))) advance(scanner);
        return create_token(TOKEN_NUMBER, scanner);
      } else if (isalpha(c) || c == '_') {
        while (isalnum(peek(scanner)) || peek(scanner) == '_') advance(scanner);
        TokenType type = check_keyword(scanner);
        return create_token(type, scanner);
      } else {
        return create_token(TOKEN_UNKNOWN, scanner);
      }
  }
}

// void destroy_scanner(Scanner *scanner) 
// {
//     if (scanner == NULL) return;
//     free(scanner); 
// }

/*TODO: Remove this function.*/
void print_token(Token token, Scanner *scanner, int token_id)
{
  if (token.type == TOKEN_UNKNOWN) return;

  printf("ID: %-3d Type: ", token_id);

  switch (token.type) {
    case TOKEN_ADD: printf("%-6s", "+"); break;
    case TOKEN_SUBTRACT: printf("%-6s", "-"); break;
    case TOKEN_MULTIPLY: printf("%-6s", "*"); break;
    case TOKEN_LPAREN: printf("%-6s", "("); break;
    case TOKEN_RPAREN: printf("%-6s", ")"); break;
    case TOKEN_IDENTIFIER: printf("%-6s", "IDEN"); break;
    case TOKEN_NUMBER: printf("%-6s", "DIGIT"); break;
    case TOKEN_FOR: printf("%-6s", "for"); break;
    case TOKEN_WHILE: printf("%-6s", "while"); break;
    case TOKEN_ELSE: printf("%-6s", "else"); break;
    case TOKEN_CONST: printf("%-6s", "const"); break;
    case TOKEN_IF: printf("%-6s", "if"); break;
    case TOKEN_BREAK: printf("%-6s", "break"); break;
    case TOKEN_TRUE: printf("%-6s", "true"); break;
    case TOKEN_FALSE: printf("%-6s", "false"); break;
    case TOKEN_IN: printf("%-6s", "in"); break;
    case TOKEN_INT: printf("%-6s", "int"); break;
    case TOKEN_FLOAT: printf("%-6s", "float"); break;
    case TOKEN_BOOL: printf("%-6s", "bool"); break;
    case TOKEN_FUNC: printf("%-6s", "func"); break;
    case TOKEN_DUP: printf("%-6s", "dup"); break;
    case TOKEN_SWAP: printf("%-6s", "swap"); break;
    case TOKEN_ROLL: printf("%-6s", "roll"); break;
    case TOKEN_OVER: printf("%-6s", "over"); break;
    case TOKEN_PEEK: printf("%-6s", "peek"); break;
    case TOKEN_PTR: printf("%-6s", "ptr"); break;
    case TOKEN_ADDR: printf("%-6s", "addr"); break;
    case TOKEN_MEMORY: printf("%-6s", "memory"); break;
    case TOKEN_WRITE: printf("%-6s", "write"); break;
    case TOKEN_READ: printf("%-6s", "read"); break;
    case TOKEN_STORE: printf("%-6s", "store"); break;
    case TOKEN_DROP: printf("%-6s", "drop"); break;
    case TOKEN_LOAD: printf("%-6s", "load"); break;
    case TOKEN_SET: printf("%-6s", "set"); break;
    case TOKEN_DIVIDE: printf("%-6s", "/"); break;
    case TOKEN_MODULO: printf("%-6s", "%"); break;
    case TOKEN_EQUAL: printf("%-6s", "="); break;
    case TOKEN_NOT_EQUAL: printf("%-6s", "!="); break;
    case TOKEN_LESS: printf("%-6s", "<"); break;
    case TOKEN_LESS_EQUAL: printf("%-6s", "<="); break;
    case TOKEN_GREATER: printf("%-6s", ">"); break;
    case TOKEN_GREATER_EQUAL: printf("%-6s", ">="); break;
    case TOKEN_BITWISE_XOR: printf("%-6s", "^"); break;
    case TOKEN_BITWISE_AND: printf("%-6s", "&"); break;
    case TOKEN_BITWISE_OR: printf("%-6s", "|"); break;
    case TOKEN_BITWISE_NOT: printf("%-6s", "~"); break;
    case TOKEN_LEFT_SHIFT: printf("%-6s", "<<"); break;
    case TOKEN_RIGHT_SHIFT: printf("%-6s", ">>"); break;
    case TOKEN_ESCAPE_SEQUENCE: printf("%-6s", "ESC"); break;
    case TOKEN_SINGLE_QUOTE: printf("%-6s", "'"); break;
    case TOKEN_DOUBLE_QUOTE: printf("%-6s", "\""); break;
    case TOKEN_NEW_LINE: printf("%-6s", "NEW_LINE"); break;
    case TOKEN_CARRIAGE_RETURN: printf("%-6s", "CARRIAGE_RETURN"); break;
    case TOKEN_ROT: printf("%-6s", "rot"); break;
    case TOKEN_REQUIRE: printf("%-6s", "require"); break;
    case TOKEN_POP: printf("%-6s", "pop"); break;
    case TOKEN_EOF: printf("%-6s", "EOF"); break;
    case TOKEN_WHITESPACE: printf("%-6s", "WS"); break;
    case TOKEN_END: printf("%-6s", "end"); break;
    case TOKEN_THEN: printf("%-6s", "then"); break;
    case TOKEN_AS: printf("%-6s", "as"); break;
    case TOKEN_PERIOD: printf("%-6s", "."); break;
    case TOKEN_UNKNOWN: printf("%-6s", "UNKNOWN"); break;
  }

  printf("Line: %d[%-2d] Symbol: ", scanner->line, scanner->start_column);
  if (token.type == TOKEN_IDENTIFIER || token.type == TOKEN_NUMBER) {
    printf("%.*s", (int)(scanner->current - scanner->start), scanner->start);
  }
  printf("\n");
}

void scan_tokens(Scanner *scanner)
{
  Token *tokens = malloc(256 * sizeof(Token));
  int token_count = 0;

  while (!is_at_end(scanner)) {
    Token token = scan_token(scanner);
    if (token.type != TOKEN_UNKNOWN) {
      print_token(token, scanner, token_count);
      tokens[token_count++] = token;
    }
  }

  free(tokens);
}
