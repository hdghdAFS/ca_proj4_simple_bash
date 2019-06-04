#include "parse.h"
sa#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <sys/types.h> 
#include <sys/wait.h>
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


bool isBuiltInCommand(char *command){
	if ((strcmp(command,"cd") == 0) || (strcmp(command,"kill") == 0) || (strcmp(command,"kill") == 0) ||  (strcmp(command,"exit") == 0)) return true;
	return false;
}

void executeCommand(char* command,char* argument){
	printf("123");
	if (argument == NULL){
		char* c[2]={command,NULL};
		execvp(command,c);
	}
	else{
		char* c[3]={command,argument,NULL};
		execvp(command,c);
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
	char *t,*p;
	t = strtok(cmd.command[i]," ");
	strcpy(command,t);
	strcpy(argument,"");
	t= strtok(NULL," ");
	if (t != NULL) {

		p=strtok(t,"\"");
		while (p != NULL){
			strcat(argument,p);
			p=strtok(NULL,"\"");
		}
	}
	if ( isBuiltInCommand(command)){
		executeBuiltInCommand(command,argument);
	} else {	

		childPid = fork();
		
		if (childPid == 0){
		
		executeCommand(command,argument); //calls execvp  

		} else {/*
			if (cmd.isBackgroundJob){
	        	record in list of background jobs
			} else {*/
				waitpid (childPid,NULL,WUNTRACED);

			}		
		}
	free(cmd.command[i]);
	}
	free(cmdLine);
	if (cmd.infile_name != NULL) free(cmd.infile_name);
	if (cmd.outfile_name != NULL) free(cmd.outfile_name);
	}
}