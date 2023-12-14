# Makefile for compiling the C program

SRC_DIR = src/
# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRC = ./src/main.c

# Executable name
EXEC = main

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to clean up
clean:
	rm -f $(EXEC)