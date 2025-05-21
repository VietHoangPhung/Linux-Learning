# Project name = current folder
PROJECT := $(notdir $(CURDIR))

# Folders
SRC_DIR := src
INC_DIR := inc
BUILD_DIR := build

# Tools and flags
CC := gcc
CFLAGS := -Wall -I$(INC_DIR)

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Final binary
TARGET := $(PROJECT)

# Default target
all: $(TARGET)

# Link objects to create final executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# Compile .c into build/*.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Make sure build/ exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

new-%:
	@echo "Creating new folder: $*"
	@mkdir -p $*
	@cp Makefile $*/Makefile
	@mkdir $*/src $*/inc
	@touch $*/src/main.c
	@echo "Folder $* created with a Makefile inside :D"

# 

.PHONY: all clean
