CC = gcc
CFLAGS = -g -std=c89 -Wall


all: parse.c parse.h shell.c
	$(CC) $(CFLAGS) -o shell shell.c

clean:
	rm -f shell
.PHONY: clean