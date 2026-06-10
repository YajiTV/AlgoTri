CC      := gcc
CFLAGS  := -std=c17 -Wall -Wextra -Wpedantic -Werror
CFLAGS  += -D_POSIX_C_SOURCE=200809L
CFLAGS  += -Iinclude

SRC_DIR   := src
OBJ_DIR   := build/obj
BIN_DIR   := build

TARGET  := $(BIN_DIR)/algotri

SRCS    := $(shell find $(SRC_DIR) -name '*.c')
OBJS    := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

LDFLAGS := $(shell pkg-config --libs ncursesw)

.PHONY: all run clean fclean re

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all
