#include "hashmap.h"

int main(void) 
{
  HashMap* map = hashmap_create();

  Token* macros = malloc(2 * sizeof(Token));
  macros[0].type = TOKEN_IDENTIFIER;
  macros[0].lexeme = "Hello";
  macros[1].type = TOKEN_IDENTIFIER;
  macros[1].lexeme = "World";
  int i = 2;

  hashmap_insert(map, "print", macros, i);

  // Access macro by name
  Macro* macro = hashmap_get(map, "print");
  if (macro != NULL) {
    printf("Macro Name: %s\n", macro->key);
    for (int i = 0; i < macro->numTokens; i++) {
      printf("Token type: %d, Value: %s\n", macro->tokens[i].type, macro->tokens[i].lexeme);
    }
  }

  hashmap_free(map);

  return 0;
}
