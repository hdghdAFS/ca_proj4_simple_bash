#include "parse.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
void printPrompt(){
	char hostname[1000];
	char buf[1000];
	char *dir,*t;
	struct passwd *info;  
	info=getpwuid(getuid()); 
	getcwd(buf, 1000); 
	gethostname(hostname,1000);
	t=buf+(int) strlen(info->pw_dir);
	dir=malloc(1000);
	strcpy(dir,t);
	printf("%s@%s:~%s$ ",info->pw_name,hostname,dir);
	free(dir);
}

void executeBuiltInCommand(char* command,char* argument){

	if (strcmp(command,"cd") == 0)
	{
		if (argument == NULL)
		{

			struct passwd *info;  
			info=getpwuid(getuid());
			chdir(info->pw_dir);
		}
		else chdir(argument);
	}
	else if (strcmp(command,"exit") == 0){
		if (argument == NULL) exit(0);
		else exit (atoi(argument));
	}

}
int 
main (int argc, char **argv)
{
	char command[1000];
	char argument[1000];
	while (1){
	int childPid;

	struct command cmd;
	printPrompt();
	char *cmdLine=readline("");

		
	cmd = parseCommand(cmdLine);
	for (int i = 0; i < cmd.cmd_num; ++i)
	{
	char *t;
	t = strtok(cmd.command[i]," ");
	strcpy(command,t);
	t= strtok(NULL," ");
	if (t != NULL) strcpy(argument,t);
	executeBuiltInCommand(command,argument);
/*	

	if ( isBuiltInCommand(command)){
		executeBuiltInCommand(command,argument);
	} else {		
		childPid = fork();
		if (childPid == 0){
		executeCommand(cmd); //calls execvp  

		} else {
			if (cmd.isBackgroundJob){
	        	record in list of background jobs
			} else {
				waitpid (childPid);

			}		
		}*/
	}
	free(cmdLine);
	}
}