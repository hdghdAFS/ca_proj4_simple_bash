#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct command parseCommand(char *cmdLine)
{
  char *t;
  struct command cmd;  
  int i = 0;
  cmd.command = malloc(sizeof(char *) * 10000);
  t = strtok(cmdLine, " ");
  cmd.command[0] = malloc(sizeof(char)*(strlen(t)+1));
  strcpy(cmd.command[0], t);
  while(t != NULL)
  {
    i += 1;
    t = strtok(NULL, " ");
    cmd.command[i] = malloc(sizeof(char)*(strlen(t)+1));
    strcpy(cmd.command[i], t);

  }
  cmd.argc= i+1;
  return cmd;
}
