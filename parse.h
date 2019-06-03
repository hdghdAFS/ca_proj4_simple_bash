struct command
{
	char **command = malloc(sizeof(char *) * 10000);
	int argc;  /*the num of argument in the cmdline*/
};



struct command parseCommand(char *cmdLine);


#include "parse.c"