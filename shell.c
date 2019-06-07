#include "parse.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <fcntl.h>                   
#include <sys/types.h>                       
#include <sys/stat.h>                       
struct jobs executeBuiltInCommand(struct command* cmd,struct jobs head){
	char *t;
	char *command=NULL;
	char *argument=NULL;
	char e_command[1000];
	strcpy(e_command,cmd->command);
	t = strtok(e_command," ");
	command =malloc(sizeof(char)*(strlen(t)+1));
	strcpy(command,t);
	t= strtok(NULL," ");
	if (t != NULL) {
        argument =malloc(sizeof(char)*(strlen(t)+1));
        strcpy(argument,t);
	}
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
	else if (strcmp(command,"kill") == 0){
		t=strtok(argument,"%%");
		if (t == NULL){
			kill(atoi(argument),15);
		}
		else{
			kill(find_pid(head,atoi(t)),15);
		}
	}
	free(command);
	free(argument);
	return head;
}



void excute(char* command,char * argument,struct command* cmd){
	if (argument == NULL){
		char* c[2]={command,NULL};
		execvp(command,c);
	}
	else{

		char* c[3]={command,argument,NULL};
		execvp(command,c);
		free(argument);
	}	
}

void executeCommand(struct command* cmd){
	int current_pipe=1;
	char e_command[1000];
	strcpy(e_command,cmd->command);
	int t0,t1,t2,fi,fo;
	if (cmd->pipe_num == 0)
	{

		char *t;
		char *command=NULL;
		char *argument=NULL;
		int t1_fd,t2_fd;

		t = strtok(e_command," ");

		command =malloc(sizeof(char)*(strlen(t)+1));
		strcpy(command,t);
		//printf("%s\n",command);
		t= strtok(NULL," ");
		if (t != NULL){
			argument = malloc(sizeof(char)*(strlen(t)+1));
			strcpy(argument,t);
		}
		if (cmd->infile_name != NULL){
			fi = open(cmd->infile_name,O_RDONLY);
			t1=dup(STDIN_FILENO);
			dup2(fi,STDIN_FILENO);

		}
		if (cmd->outfile_name != NULL)
		{

			if (cmd->append){
				fo = open(cmd->outfile_name,O_WRONLY|O_APPEND| O_CREAT,0644);
			}
			else{
				fo = open(cmd->outfile_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

			}
			t2=dup(STDOUT_FILENO);
			dup2(fo,STDOUT_FILENO);
			
			
		}
    	

		excute(command,argument,cmd);

		if (cmd->infile_name != NULL){
			
			dup2(t1,STDIN_FILENO);
			close(fi);
			close(t1);
			free(cmd->infile_name);
		}
		if (cmd->outfile_name != NULL)
		{
			
			dup2(t2,STDOUT_FILENO);
			close(fo);
			close(t2);
			free(cmd->outfile_name);			
		}
		

    	free(command);
	}
	else{
		int pipes[2],childPid;
		char *first_commmand,*command_remain;
		command_remain=cmd->command;
		while (current_pipe <= cmd->pipe_num+1){			
			pipe(pipes);
			
			char *t = strsep(&command_remain,"|");
			first_commmand = malloc(sizeof(char)*(strlen(t)+1));
			strcpy(first_commmand,t);

			char *command=NULL;
			char *argument=NULL;
			t = strtok(first_commmand," ");

			command =malloc(sizeof(char)*(strlen(t)+1));
			strcpy(command,t);
			t = strtok(NULL," ");
			
			if (t != NULL){
				argument = malloc(sizeof(char)*(strlen(t)+1));
				strcpy(argument,t);
			}
			free(first_commmand);
			childPid = fork();
			if (childPid == 0){
				if (current_pipe == 1)
				{
					//in re
					if (cmd->infile_name != NULL){
						fi = open(cmd->infile_name,O_RDONLY);
						t1=dup(STDIN_FILENO);
						dup2(fi,STDIN_FILENO);

					}
				}
				else if (current_pipe > 1){
					t1=dup(STDIN_FILENO);
					dup2(t0,STDIN_FILENO);
				}
				if (current_pipe != cmd->pipe_num+1)
				{
					t2=dup(STDOUT_FILENO);
					dup2(pipes[1],STDOUT_FILENO);
				}
				else if (current_pipe == cmd->pipe_num+1) {
					//out re
					if (cmd->outfile_name != NULL)
					{

						if (cmd->append){
							fo = open(cmd->outfile_name,O_WRONLY|O_APPEND| O_CREAT,0644);
						}
						else{
							fo = open(cmd->outfile_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

						}
						t2=dup(STDOUT_FILENO);
						dup2(fo,STDOUT_FILENO);
						
						
					}
				}	
				close(pipes[0]);
				excute(command,argument,cmd);

				free(command);
				dup2(t1,STDIN_FILENO);
				dup2(t2,STDOUT_FILENO);
				if ((cmd->infile_name != NULL) && (current_pipe == 1)){

					dup2(t1,STDIN_FILENO);
					close(fi);
					free(cmd->infile_name);
				}
				if ((cmd->outfile_name != NULL) && (current_pipe == cmd->pipe_num+1))
				{
					dup2(t2,STDOUT_FILENO);
					close(fo);
					close(t2);
					free(cmd->outfile_name);			
				}				
				exit(0);
			}
			else {
				waitpid (childPid,NULL,WUNTRACED);
				
				
				current_pipe++;
				close(pipes[1]);
				t0=pipes[0];
			}
		}
		
	}

}
int 
main (int argc, char **argv)
{

	char cmdLine[1000];
	FILE *fi = fopen(argv[1], "r");
	struct jobs head;
	head.i=0;
	head.next=NULL;
	while (1){
	//printf("a\n");
	int childPid,fatherpid;

	struct command *cmd;
	/*printPrompt();*/
	
	if (fgets(cmdLine,1000,fi) == NULL) break;
	cmd = parseCommand(cmdLine);
	//printf("%s\n",cmd->command);
	if ( cmd->isBuiltInCommand){
		head=executeBuiltInCommand(cmd,head);
	} else {	

		childPid = fork();
		
		if (childPid == 0){
		
		executeCommand(cmd); //calls execvp  
		exit(0);
		} else {
			if (cmd->isBackgroundJob){
	        	head=insert(head,childPid,cmd);
			} else {
				waitpid (childPid,NULL,WUNTRACED);

			}		
		}


	}

	}
	fclose(fi);
}