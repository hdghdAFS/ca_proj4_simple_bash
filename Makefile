CC = gcc
CFLAGS = -g  -Wall


all: parse.c parse.h shell.c
	$(CC) $(CFLAGS) -o shell shell.c

clean:
	rm -f shell
.PHONY: clean