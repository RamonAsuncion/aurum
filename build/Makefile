CC = gcc
CFLAGS = -g -c
LDFLAGS = -g
INCLUDE_DIRS = -I../src/lexer -I../src/parser -I../src/ast -I../src/runtime
EXES = lexer_tests parser_tests ast_tests runtime_tests

all: $(EXES)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o $@

lexer_tests: ../tests/lexer_tests.o ../src/lexer/lexer.o
	$(CC) $(LDFLAGS) $^ -o $@

parser_tests: ../tests/parser_tests.o ../src/parser/parser.o ../src/lexer/lexer.o ../src/ast/ast.o
	$(CC) $(LDFLAGS) $^ -o $@

ast_tests: ../tests/ast_tests.o ../src/ast/ast.o
	$(CC) $(LDFLAGS) $^ -o $@

runtime_tests: ../tests/runtime_tests.o ../src/runtime/runtime.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *~ $(EXES)
	rm -f ../tests/*.o
	rm -f ../src/lexer/*.o
	rm -f ../src/parser/*.o
	rm -f ../src/ast/*.o
	rm -f ../src/runtime/*.o
	rm -f ast.dot ast.png
