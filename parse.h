#include <stdbool.h>
struct command{
	int pipe_num;
	char * command;
	char *infile_name;
	char *outfile_name;
	bool isBackgroundJob;
	bool isBuiltInCommand;
	bool append;
};
struct jobs
{
	int pid;
	int i; //i is the max num  in head
	char* command;
	struct jobs* next;
	char c[1];
};
struct command* parseCommand(char *cmdLine);
struct jobs print_jobs(struct jobs head);

struct jobs insert(struct jobs head,int p,struct command* cmd);

int find_pid(struct jobs head,int num);
#include "parse.c" 