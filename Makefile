ifeq ($(CC),)
CC=clang
endif

OBJS=tmp/sudokusolv.o

all: bin/sudokusolv


tmp/sudokusolv.o: src/sudokusolv.c src/sudokusolve.h

	$(CC) -c -ansi -pedantic -std=c99 -Werror -Wall -o tmp/sudokusolv.o src/sudokusolv.c

bin/sudokusolv: $(OBJS)

	$(CC) -o bin/sudokusolv tmp/sudokusolv.o

clean:

	-rm tmp/*.o
	-rm bin/sudokusolv

.PHONY: all clean
