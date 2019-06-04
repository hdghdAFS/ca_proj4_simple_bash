#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct command parseCommand(char *cmdLine)
{
  char *t;
  struct command cmd; 
  cmd.isBackgroundJob=false;
  cmd.infile_name = NULL;
  cmd.outfile_name = NULL; 
  int i = 0;
  int k=0;
  int m=0;
  int j = 0; //num of pipe
  char **command = malloc(sizeof(char *) * 10000);
  t = strtok(cmdLine, " \n");
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
  cmd.command=malloc(sizeof(char *) * (j+1));
  cmd.cmd_num= j+1;
  cmd.command[0]=malloc(sizeof(char) * (strlen(command[0])+1));
  strcpy(cmd.command[0],command[0]);
  free(command[0]);
  k=1;
  while (k < i)
  {
    if (strcmp(command[k],"|")==0){
      free(command[k]);
      m++;
      cmd.command[m]=malloc(sizeof(char) * (strlen(command[k+1])+1));
      strcpy(cmd.command[m],command[k+1]);
      free(command[k+1]);
      k=k+2;
    }
    else if (strcmp(command[k],"<")==0){
      free(command[k]);
      cmd.infile_name=malloc(sizeof(char) * (strlen(command[k+1])+1));
      strcpy(cmd.infile_name,command[k+1]);
      free(command[k+1]);
      k=k+2;
    }    
    else if (strcmp(command[k],">>")==0){
      free(command[k]);
      cmd.outfile_name=malloc(sizeof(char) * (strlen(command[k+1])+1));
      strcpy(cmd.outfile_name,command[k+1]);
      free(command[k+1]);
      k=k+2;
    }
    else if (strcmp(command[k],">")==0){
      free(command[k]);
      cmd.outfile_name=malloc(sizeof(char) * (strlen(command[k+1])+1));
      strcpy(cmd.outfile_name,command[k+1]);
      free(command[k+1]);
      k=k+2;
    }
    else if (strcmp(command[k],"&")==0){
      cmd.isBackgroundJob=true;
      free(command[k]);
      k++;
    }
    else if (k != 0){
      cmd.command[m] = realloc (cmd.command[m],sizeof(char)*(strlen(cmd.command[m])+strlen(command[k])+2));
      strcat(cmd.command[m]," ");
      strcat(cmd.command[m],command[k]);
      free(command[k]);
      k++;
    }

  }
  free(command);
  return cmd;
}
