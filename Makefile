CFLAGS := -Isrc -std=c11 -g3 -O2 \
          -Wall -Wextra -Wpedantic \
          -Wconversion -Wdouble-promotion \
          -Wformat=2 -Wshadow -Wwrite-strings \
          -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs \
          -Wmissing-include-dirs \
          -Wno-unused-function -Wno-unused-parameter -Wno-sign-conversion \
          -fsanitize=undefined -fsanitize-trap
# -DDEBUG

ifeq ($(CC),gcc)
  CFLAGS += -Wjump-misses-init -Wlogical-op
endif

SRC=src
BIN=bin
OBJ=obj

SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

EXEC := $(BIN)/aurum

.PHONY: all clean mkdirs
all: $(EXEC)

$(EXEC): $(OBJS) | mkdirs
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c | mkdirs
	$(CC) $(CFLAGS) -c $< -o $@

mkdirs:
	@mkdir -p $(BIN) $(OBJ)

clean:
	@rm -rf $(BIN)/* core* *~ $(SRC)/*~ docs/* *.dSYM $(OBJ)/*

