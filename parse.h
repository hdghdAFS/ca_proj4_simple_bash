#include <stdbool.h>
struct command{
	int cmd_num;
	char ** command;
	char *infile_name;
	char *outfile_name;
	bool isBackgroundJob;

};
struct command parseCommand(char *cmdLine);
#include "parse.c" 