#include <assert.h>

#include "lexer.h"
#include "scanner.h"
#include "print_tokens.h"

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

int main(void) 
{
  const char *source = "\"Hello, World!\n\"\n";
  Scanner scanner;
  init_scanner(&scanner, source);
  scan_tokens(&scanner);
  return 0;
}
