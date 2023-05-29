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
  trie_insert(root, "continue", TOKEN_CONTINUE);
  trie_insert(root, "for", TOKEN_FOR);
  trie_insert(root, "while", TOKEN_WHILE);
  trie_insert(root, "else", TOKEN_ELSE);
  trie_insert(root, "elseif", TOKEN_ELIF);
  trie_insert(root, "try", TOKEN_TRY);
  trie_insert(root, "catch", TOKEN_CATCH);
  trie_insert(root, "throw", TOKEN_THROW);
  trie_insert(root, "const", TOKEN_CONST);
  trie_insert(root, "return", TOKEN_RETURN);
  trie_insert(root, "if", TOKEN_IF);
  trie_insert(root, "break", TOKEN_BREAK);
  trie_insert(root, "union", TOKEN_UNION);
  trie_insert(root, "static", TOKEN_STATIC);
  trie_insert(root, "true", TOKEN_TRUE);
  trie_insert(root, "false", TOKEN_FALSE);
  trie_insert(root, "alias", TOKEN_ALIAS);
  trie_insert(root, "foreach", TOKEN_FOREACH);
  trie_insert(root, "in", TOKEN_IN);
  trie_insert(root, "switch", TOKEN_SWITCH);
  trie_insert(root, "case", TOKEN_CASE);
  trie_insert(root, "default", TOKEN_DEFAULT);
  trie_insert(root, "Void", TOKEN_VOID);
  trie_insert(root, "I.8", TOKEN_I8);
  trie_insert(root, "U.8", TOKEN_U8);
  trie_insert(root, "I.16", TOKEN_I16);
  trie_insert(root, "U.16", TOKEN_U16);
  trie_insert(root, "I.32", TOKEN_I32);
  trie_insert(root, "U.32", TOKEN_U32);
  trie_insert(root, "I.64", TOKEN_I64);
  trie_insert(root, "U.64", TOKEN_U64);
  trie_insert(root, "F.32", TOKEN_F32);
  trie_insert(root, "F.64", TOKEN_F64);
  trie_insert(root, "{", TOKEN_LBRACE);
  trie_insert(root, "}", TOKEN_RBRACE);
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


TokenType check_keyword(Scanner *scanner) {
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
  skip_whitespace(scanner);
  scanner->start = scanner->current;
  scanner->start_column = scanner->column;
  if (is_at_end(scanner)) return create_token(TOKEN_EOF, scanner);

  char c = advance(scanner);
  switch (c) {
    case '*': return create_token(TOKEN_MULTIPLY, scanner);
    case '(': return create_token(TOKEN_LPAREN, scanner);
    case ')': return create_token(TOKEN_RPAREN, scanner);
    case ';': return create_token(TOKEN_SEMICOLON, scanner);
    case '%': return create_token(TOKEN_MODULO, scanner);
    case '~': return create_token(TOKEN_BITWISE_NOT, scanner);
    case '^': return create_token(TOKEN_BITWISE_XOR, scanner);
    case '\\': return create_token(TOKEN_ESCAPE_SEQUENCE, scanner);
    case '{': return create_token(TOKEN_LBRACE, scanner);
    case '}': return create_token(TOKEN_RBRACE, scanner);
    case '=':
      if (match(scanner, '=')) {
        return create_token(TOKEN_EQUAL, scanner);
      } else {
        return create_token(TOKEN_ASSIGN, scanner);
      }
    case '!':
      if (match(scanner, '=')) {
        return create_token(TOKEN_NOT_EQUAL, scanner);
      } else {
        return create_token(TOKEN_NOT, scanner);
      }
    case '.':
      if (match(scanner, '.')) {
        return create_token(TOKEN_RANGE, scanner);
      } else {
        return create_token(TOKEN_UNKNOWN, scanner);
      }
    case '-':
      if (match(scanner, '-')) {
        return create_token(TOKEN_DECREMENT, scanner);
      } else {
        return create_token(TOKEN_SUBTRACT, scanner);
      }
    case '+':
      if (match(scanner, '+')) {
        return create_token(TOKEN_INCREMENT, scanner);
      } else if (match(scanner, '=')) {
        return create_token(TOKEN_ASSIGN_ADD, scanner);
      } else {
        return create_token(TOKEN_ADD, scanner);
      }
    case '&':
      if (match(scanner, '&')) {
        return create_token(TOKEN_AND, scanner);
      } else {
        return create_token(TOKEN_BITWISE_AND, scanner);
      }
    case '|':
      if (match(scanner, '|')) {
        return create_token(TOKEN_OR, scanner);
      } else {
        return create_token(TOKEN_BITWISE_OR, scanner);
      }
    case '>':
      if (match(scanner, '=')) {
        return create_token(TOKEN_GREATER_EQUAL, scanner);
      } else if (match(scanner, '>')) {
        return create_token(TOKEN_RIGHT_SHIFT, scanner);
      } else {
        return create_token(TOKEN_GREATER, scanner);
      }
    case '<':
      if (match(scanner, '=')) {
        return create_token(TOKEN_LESS_EQUAL, scanner);
      } else if (match(scanner, '<')) {
        return create_token(TOKEN_LEFT_SHIFT, scanner);
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

void destroy_scanner(Scanner *scanner) {
    if (scanner == NULL) {
        return;
    }
    free(scanner);  // Free the memory allocated for the scanner
}

void print_token(Token token, Scanner *scanner, int token_id)
{
  if (token.type == TOKEN_UNKNOWN) return;

  printf("ID: %-3d Type: ", token_id);

  switch (token.type) {
    case TOKEN_ADD: printf("%-6s", "+"); break;
    case TOKEN_SUBTRACT: printf("%-6s", "-"); break;
    case TOKEN_ASSIGN_ADD: printf("%-6s", "+="); break;
    case TOKEN_MULTIPLY: printf("%-6s", "*"); break;
    case TOKEN_LPAREN: printf("%-6s", "("); break;
    case TOKEN_RPAREN: printf("%-6s", ")"); break;
    case TOKEN_SEMICOLON: printf("%-6s", ";"); break;
    case TOKEN_IDENTIFIER: printf("%-6s", "IDEN"); break;
    case TOKEN_NUMBER: printf("%-6s", "INT"); break;
    case TOKEN_UNKNOWN: printf("%-6s", "UNKNOWN"); break;
    case TOKEN_ASSIGN: printf("%-6s", "="); break;
    case TOKEN_CONTINUE: printf("%-6s", "continue"); break;
    case TOKEN_FOR: printf("%-6s", "for"); break;
    case TOKEN_WHILE: printf("%-6s", "while"); break;
    case TOKEN_ELSE: printf("%-6s", "else"); break;
    case TOKEN_ELIF: printf("%-6s", "elseif"); break;
    case TOKEN_TRY: printf("%-6s", "try"); break;
    case TOKEN_CATCH: printf("%-6s", "catch"); break;
    case TOKEN_THROW: printf("%-6s", "throw"); break;
    case TOKEN_CONST: printf("%-6s", "const"); break;
    case TOKEN_RETURN: printf("%-6s", "return"); break;
    case TOKEN_IF: printf("%-6s", "if"); break;
    case TOKEN_BREAK: printf("%-6s", "break"); break;
    case TOKEN_UNION: printf("%-6s", "union"); break;
    case TOKEN_STATIC: printf("%-6s", "static"); break;
    case TOKEN_TRUE: printf("%-6s", "true"); break;
    case TOKEN_FALSE: printf("%-6s", "false"); break;
    case TOKEN_ALIAS: printf("%-6s", "alias"); break;
    case TOKEN_VOID: printf("%-6s", "Void"); break;
    case TOKEN_FOREACH: printf("%-6s", "foreach"); break;
    case TOKEN_RANGE: printf("%-6s", ".."); break;
    case TOKEN_IN: printf("%-6s", "in"); break;
    case TOKEN_SWITCH: printf("%-6s", "switch"); break;
    case TOKEN_CASE: printf("%-6s", "case"); break;
    case TOKEN_DEFAULT: printf("%-6s", "default"); break;
    case TOKEN_GOTO: printf("%-6s", "goto"); break;
    case TOKEN_I8: printf("%-6s", "I.8"); break;
    case TOKEN_U8: printf("%-6s", "U.8"); break;
    case TOKEN_I16: printf("%-6s", "I.16"); break;
    case TOKEN_U16: printf("%-6s", "U.16"); break;
    case TOKEN_I32: printf("%-6s", "I.32"); break;
    case TOKEN_U32: printf("%-6s", "U.32"); break;
    case TOKEN_I64: printf("%-6s", "I.64"); break;
    case TOKEN_U64: printf("%-6s", "U.64"); break;
    case TOKEN_F32: printf("%-6s", "F.32"); break;
    case TOKEN_F64: printf("%-6s", "F.64"); break;
    case TOKEN_DIVIDE: printf("%-6s", "/"); break;
    case TOKEN_MODULO: printf("%-6s", "%"); break;
    case TOKEN_EQUAL: printf("%-6s", "=="); break;
    case TOKEN_NOT_EQUAL: printf("%-6s", "!="); break;
    case TOKEN_LESS: printf("%-6s", "<"); break;
    case TOKEN_LESS_EQUAL: printf("%-6s", "<="); break;
    case TOKEN_GREATER: printf("%-6s", ">"); break;
    case TOKEN_GREATER_EQUAL: printf("%-6s", ">="); break;
    case TOKEN_AND: printf("%-6s", "&&"); break;
    case TOKEN_OR: printf("%-6s", "||"); break;
    case TOKEN_NOT: printf("%-6s", "!"); break;
    case TOKEN_BITWISE_XOR: printf("%-6s", "^"); break;
    case TOKEN_INCREMENT: printf("%-6s", "++"); break;
    case TOKEN_DECREMENT: printf("%-6s", "--"); break;
    case TOKEN_BITWISE_AND: printf("%-6s", "&"); break;
    case TOKEN_BITWISE_OR: printf("%-6s", "|"); break;
    case TOKEN_BITWISE_NOT: printf("%-6s", "~"); break;
    case TOKEN_LEFT_SHIFT: printf("%-6s", "<<"); break;
    case TOKEN_RIGHT_SHIFT: printf("%-6s", ">>"); break;
    case TOKEN_ESCAPE_SEQUENCE: printf("%-6s", "ESC"); break;
    case TOKEN_EOF: printf("%-6s", "EOF"); break;
    case TOKEN_LBRACE: printf("%-6s", "{"); break;
    case TOKEN_RBRACE: printf("%-6s", "}"); break;
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
