#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"
#include "lexer.h"

int main(void)
{
  int tokens = 5;
  HashMap* map = hashmap_create();

  Token* macros = malloc(tokens * sizeof(Token));
  macros[0].type = TOKEN_NUMBER;
  macros[0].lexeme = "1";
  macros[1].type = TOKEN_NUMBER;
  macros[1].lexeme = "1";
  macros[2].type = TOKEN_NUMBER;
  macros[2].lexeme = "3";
  macros[3].type = TOKEN_SYSCALL;
  macros[3].lexeme = "systemcall";
  macros[4].type = TOKEN_END;
  macros[4].lexeme = "end";
  hashmap_insert(map, "print", macros, tokens);

  Macro* macro = hashmap_get(map, "print");
  if (macro != NULL) {
    printf("%s:\n", macro->key);
    for (int i = 0; i < macro->numTokens; ++i) {
      printf("%6d %-15s\n",
             macro->tokens[i].type,
             macro->tokens[i].lexeme);
    }
  }

  hashmap_free(map);

  return 0;
}
