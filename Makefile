# Compiler and flags
CC=gcc -I./include
CFLAGS=-std=gnu99 -O2 -Wall -g

# Directories
OBJ=./obj
BIN=./bin
TEST_BIN=./test_bin
SRC=./src
INC=./include
TEST_SRC=./tests

# Source files and object files
SOURCES=$(wildcard $(SRC)/*.c)
TEST_SOURCES=$(wildcard $(TEST_SRC)/*.c)
OBJECTS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
TEST_OBJECTS=$(patsubst $(TEST_SRC)/%.c, $(OBJ)/%.o, $(TEST_SOURCES))

# Executables
EXEC=$(BIN)/aurum
TEST_EXECUTABLES=$(foreach t,$(notdir $(TEST_SOURCES:.c=)), $(TEST_BIN)/$(t))

# Targets
all: mkdirs $(EXEC)
test: $(TEST_EXECUTABLES)

# Create the directories
mkdirs:
	@mkdir -p $(OBJ) $(BIN) $(TEST_BIN)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Compile the source files into object files
$(OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files into object files
$(OBJ)/%.o : $(TEST_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link test files to create individual executables
$(TEST_BIN)/hashmap_tests: $(OBJ)/hashmap_tests.o $(OBJ)/hashmap.o $(OBJ)/lexer.o
	$(CC) $(CFLAGS) $(OBJ)/hashmap_tests.o $(OBJ)/hashmap.o $(OBJ)/lexer.o -o $@

$(TEST_BIN)/lexer_tests: $(OBJ)/lexer_tests.o $(OBJ)/lexer.o $(OBJ)/print_tokens.o $(OBJ)/hashmap.o
	$(CC) $(CFLAGS) $(OBJ)/lexer_tests.o $(OBJ)/lexer.o $(OBJ)/print_tokens.o $(OBJ)/hashmap.o -o $@

$(TEST_BIN)/stack_tests: $(OBJ)/stack_tests.o $(OBJ)/stack.o
	$(CC) $(CFLAGS) $(OBJ)/stack_tests.o $(OBJ)/stack.o -o $@

# Clean target
clean:
	@echo "Removed build files."
	@rm -rf $(EXEC) $(OBJECTS) $(TEST_BIN) ./obj/ ./bin/

# Install target (optional)
install:
	cp $(EXEC) /usr/local/bin/

# Phony targets
.PHONY: all clean distclean install

