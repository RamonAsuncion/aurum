CC = gcc
CFLAGS = -g -c
LDFLAGS = -g
INCLUDE_DIRS = -I../src/lexer -I../src/parser -I../src/ast -I../src/runtime -I../src/stack -I../src/interpreter
EXES = lexer_tests parser_tests ast_tests  stack_tests interpreter_tests
INTERPRETER = aurum

all: $(EXES) $(INTERPRETER)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o $@

lexer_tests: ../tests/lexer_tests.o ../src/lexer/lexer.o
	$(CC) $(LDFLAGS) $^ -o $@

parser_tests: ../tests/parser_tests.o ../src/parser/parser.o ../src/lexer/lexer.o ../src/ast/ast.o
	$(CC) $(LDFLAGS) $^ -o $@

ast_tests: ../tests/ast_tests.o ../src/ast/ast.o
	$(CC) $(LDFLAGS) $^ -o $@

stack_tests: ../tests/stack_tests.o ../src/stack/stack.o
	$(CC) $(LDFLAGS) $^ -o $@

interpreter_tests: ../tests/interpreter_tests.o ../src/interpreter/interpreter.o ../src/stack/stack.o  ../src/lexer/lexer.o
	$(CC) $(LDFLAGS) $^ -o $@

aurum: ../src/interpreter/main.o ../src/interpreter/interpreter.o  ../src/lexer/lexer.o ../src/stack/stack.o
	$(CC) $(LDFLAGS) $^ -o ../bin/$@

clean:
	rm -f *.o *~ $(EXES)
	rm -f ../tests/*.o
	rm -f ../src/parser/*.o
	rm -f ../src/lexer/*.o
	rm -f ../src/stack/*.o
	rm -f ../src/interpreter/*.o

