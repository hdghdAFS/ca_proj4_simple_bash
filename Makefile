CC = gcc
CFLAGS = -g 


all: parse.c parse.h shell.c
	$(CC) $(CFLAGS) -o shell shell.c -lreadline

