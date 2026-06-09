CC      := gcc
CFLAGS  := -std=c17 -Wall -Wextra -Wpedantic -Werror
CFLAGS  += -D_POSIX_C_SOURCE=200809L
CFLAGS  += -Iinclude -Itests/unit

SRC_DIR   := src
OBJ_DIR   := build/obj
BIN_DIR   := build
TEST_DIR  := tests

TARGET  := $(BIN_DIR)/algotri

SRCS    := $(shell find $(SRC_DIR) -name '*.c')
OBJS    := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Library objects: all src objects except main.o
LIB_OBJS  := $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# One binary per unit test file
UNIT_SRCS := $(wildcard $(TEST_DIR)/unit/test_*.c)
UNIT_BINS := $(patsubst $(TEST_DIR)/unit/%.c, $(BIN_DIR)/%, $(UNIT_SRCS))

# One binary per integration test file
INTG_SRCS := $(wildcard $(TEST_DIR)/integration/test_*.c)
INTG_BINS := $(patsubst $(TEST_DIR)/integration/%.c, $(BIN_DIR)/%, $(INTG_SRCS))

LDFLAGS := $(shell pkg-config --libs ncursesw)

.PHONY: all run test clean fclean re

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(TEST_DIR)/unit/%.c $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/%: $(TEST_DIR)/integration/%.c $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(UNIT_BINS) $(INTG_BINS)
	@for bin in $^; do echo "--- $$bin ---"; ./$$bin; done

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET) $(UNIT_BINS) $(INTG_BINS)

re: fclean all
