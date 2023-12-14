
CC = gcc

SRC = ./src/main.c

EXEC = main

all: clean $(EXEC)

$(EXEC): $(SRC)
	$(CC) -o $@ $^

clean:
	rm -f $(EXEC)