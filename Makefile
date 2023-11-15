# Compiler and flags
CC=gcc -I ./include
CFLAGS=-std=gnu99 -O2 -Wall -g

# Directories
OBJ=./obj
BIN=./bin
SRC=./src
INC=./include

# Source files and object files
SOURCES=$(wildcard $(SRC)/*.c)
OBJECTS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

# Executable
EXEC=$(BIN)/aurum

# Default target
all: mkdirs $(EXEC)

# Create the directories
mkdirs:
	mkdir -p $(OBJ) $(BIN)

# Link the object files to create the executable
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Compile the source files into object files
$(OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(BIN)/$(EXEC) $(OBJECTS) ./obj/* 

# Distclean target
distclean: 
	rm -rf $(OBJ) 

# Phony targets
.PHONY: all clean distclean
