#include <assert.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "parser.h"

int main() {
    const char *source = "a + b / c";
    Scanner scanner;
    init_scanner(&scanner, source);
    ASTNode *root = parse(&scanner);

    FILE *dot_file = fopen("ast.dot", "w");
    if (dot_file) {
        generate_dot(root, dot_file);
        fclose(dot_file);
    } else {
        fprintf(stderr, "Could not open 'ast.dot' for writing.\n");
    }
    system("dot -Tpng ast.dot -o ast.png");
    return 0;
}

