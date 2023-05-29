#include <assert.h>

#include "lexer.h"
#include "scanner.h"

int main() 
{
  const char *source = "a = 5;\na += 10 * 9;\nprintln( a );\n";
  Scanner scanner;
  init_scanner(&scanner, source);
  scan_tokens(&scanner);
  return 0;
}
