#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"

int parseCommand(char *cmdLine)
{
  char *t;
  struct command cmd;
  int i = 0;
  t = strtok(cmdLine, " ");
  cmd->command[0] = malloc(sizeof(char)*(strlen(t)+1));
  strcpy(cmd->command[0], t);
  while(t != NULL)
  {
    i += 1;
    t = strtok(NULL, " ");
    cmd->command[i] = malloc(sizeof(char)*(strlen(t)+1));
    strcpy(cmd->command[i], t);
    
  }
  argc = i+1 ;
  return cmd;
}
