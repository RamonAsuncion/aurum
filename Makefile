CC=gcc -I ./include
CFLAGS=-std=gnu99 -O2 -Wall -g

SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst src/%.c, obj/%.o, $(SOURCES))

OBJ=./obj
BIN=./bin
SRC=./src
INC=./include
TEST=./tests

vpath %.c $(SRC)
vpath %.h $(INC)

EXEC=./bin/aurum

all: mkpaths $(EXEC)

mkpaths:
	@mkdir -p $(OBJ) $(BIN)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create the test executables (remove the main.o object file from the list of objects)
$(BIN)/%: $(TEST)/%.c $(filter-out obj/main.o,$(OBJECTS))
	@if [ "$@" != "main.o" ]; then \
		$(CC) $(CFLAGS) $< $(filter-out obj/main.o,$(OBJECTS)) -o $@; \
	fi

tests: $(patsubst $(TEST)/%.c, $(BIN)/%, $(filter-out $(TEST)/main.c, $(TEST_FILES)))

clean:
	rm -rf $(BIN)/$(EXEC) $(OBJECTS) ./obj/* 
	find . -name "*~" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;

distclean: 
	rm -rf $(OBJ) 

.PHONY: all clean distclean
