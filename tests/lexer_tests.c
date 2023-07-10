#include <assert.h>

#include "lexer.h"
#include "scanner.h"

int main(void) 
{
  const char *source = "2 while dup 1 >= then 4 while dup 0 >= then dup . 1 -  end 1 - end";
  Scanner scanner;
  init_scanner(&scanner, source);
  scan_tokens(&scanner);
  return 0;
}
