CC = gcc
CFLAGS = -g  -Wall 


all: parse.c parse.h shell.c
	$(CC) $(CFLAGS) -o shell shell.c -lreadline

clean:
	rm -f shell
.PHONY: clean