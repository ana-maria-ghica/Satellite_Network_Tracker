CC=gcc
CFLAGS=-Wall -Wextra
SRC = src/main.c src/utils.c
OBJ = $(SRC:.c=.o)

.PHONY: build, test, clean

build: main

main: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@

test: main
	checker/run_tests.sh

clean:
	rm -f *.o main