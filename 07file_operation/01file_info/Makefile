# project & executable name
PROJECT := $(notdir $(CURDIR))

# directories
SRC_DIR := src
INC_DIR := inc
BUILD_DIR := build

# CC and flag
CC := gcc
CFLAGS := -Wall -I$(INC_DIR)

# sources
SRCS := $(wildcard $(SRC_DIR)/*.c)

# objects
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# executable
TARGET := $(PROJECT).out

# all
all: $(TARGET)

# link
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# run executable
run: 
	@./$(TARGET)

# clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# create new project folder with a root makefile template
new-%:
	@echo "Creating new folder: $*"
	@mkdir -p $*
	@cp Makefile $*/Makefile
	@mkdir -p $*/src $*/inc
	@touch $*/src/main.c
	@echo "Folder $* created with a Makefile inside :D"

# 

.PHONY: all clean
