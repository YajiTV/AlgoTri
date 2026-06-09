CC      := gcc
CFLAGS  := -std=c17 -Wall -Wextra -Wpedantic -Werror
CFLAGS  += -Iinclude

SRC_DIR   := src
OBJ_DIR   := build/obj
BIN_DIR   := build
TEST_DIR  := tests

TARGET  := $(BIN_DIR)/algotri
TEST_BIN := $(BIN_DIR)/algotri_tests

SRCS    := $(shell find $(SRC_DIR) -name '*.c')
OBJS    := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TEST_SRCS := $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/tests/%.o, $(TEST_SRCS))
# Main objects without main.c for test linking
LIB_OBJS  := $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

LDFLAGS := -lncurses

.PHONY: all run test clean fclean re

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(LIB_OBJS) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/tests/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET) $(TEST_BIN)

re: fclean all
