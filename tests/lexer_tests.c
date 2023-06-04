#include <assert.h>

#include "lexer.h"
#include "scanner.h"

int main() 
{
  const char *source = "2 3 + 5 -";
  Scanner scanner;
  init_scanner(&scanner, source);
  scan_tokens(&scanner);
  return 0;
}
