#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct command* parseCommand(char *cmdLine)
{
  char *t;
  struct command* cmd=malloc(sizeof(struct command)); 
  cmd->isBackgroundJob=false;
  cmd->infile_name = NULL;
  cmd->outfile_name = NULL;
  cmd->append = false; 
  int i = 0;
  int k=0;
  int j = 0; //num of pipe
  char **command = malloc(sizeof(char *) * 10000);
  t = strtok(cmdLine, " \n");
  if ((strcmp(t,"cd") == 0) || (strcmp(t,"kill") == 0) || (strcmp(t,"kill") == 0) ||  (strcmp(t,"exit") == 0)) {
  	cmd->isBuiltInCommand = true;
  }
  else{
  	cmd->isBuiltInCommand = false;
  }
  while(t != NULL)
  {
    command[i] = malloc(sizeof(char)*(strlen(t)+1));
    strcpy(command[i], t);
    //read the next 
    t = strtok(NULL, " \n");
    i += 1;
    if (t == NULL) break;
    if (strcmp(t,"|")==0){
      j++;
    }       
  }
  //printf("%s %s\n",command[0],command[1]);
  cmd->pipe_num= j;
  cmd->command=malloc(sizeof(char) * (strlen(command[0])+1));
  strcpy(cmd->command,command[0]);
  free(command[0]);
  k=1;
  while (k < i)
  {
    if (strcmp(command[k],"<")==0){
      free(command[k]);
      cmd->infile_name=malloc(sizeof(char) * (strlen(command[k+1])+1));
      strcpy(cmd->infile_name,command[k+1]);
      free(command[k+1]);
      k=k+2;
    }    
    else if (strcmp(command[k],">>")==0){
      free(command[k]);
      cmd->append = true;
      cmd->outfile_name=malloc(sizeof(char) * (strlen(command[k+1])+1));
      strcpy(cmd->outfile_name,command[k+1]);
      free(command[k+1]);
      k=k+2;
    }
    else if (strcmp(command[k],">")==0){
      free(command[k]);
      cmd->outfile_name=malloc(sizeof(char) * (strlen(command[k+1])+1));
      strcpy(cmd->outfile_name,command[k+1]);
      free(command[k+1]);
      k=k+2;
    }
    else if (strcmp(command[k],"&")==0){
      cmd->isBackgroundJob=true;
      free(command[k]);
      k++;
    }
    else if (k != 0){
      cmd->command = realloc (cmd->command,sizeof(char)*(strlen(cmd->command)+strlen(command[k])+2));
      strcat(cmd->command," ");
      strcat(cmd->command,command[k]);
      free(command[k]);
      k++;
    }

  }
  //("%s\n",cmd->command);
  free(command);
  return cmd;
}



struct jobs insert(struct jobs head,int pid,struct command* cmd){
	struct jobs* j = malloc(sizeof(struct jobs));
	struct jobs* p= &head;

	while (p->next != NULL) {
		p=p->next;
	}
	p->next=j;
	j->next=NULL;
	j->i= (head.i)+1;
	j->pid = pid;
	j->command =cmd->command;

	return head; 
}


int find_pid(struct jobs head,int num){
	struct jobs* p= &head;
	p=p->next;
	while(p->i != num){
		p=p->next;
	}
	return p->pid;
}