#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"

int parseCommand(char *cmdLine)
{
  char *t;
  char **command = malloc(sizeof(char *) * 10000);
  int i = 1;
  t = strtok(cmdLine, " ");
  command[0] = malloc(sizeof(char)*(strlen(t)+1));
  strcpy(command[0], t);
  while(t != NULL)
  {
    t = strtok(NULL, " ");
    command[i] = malloc(sizeof(char)*(strlen(t)+1));
    strcpy(command[1], t);
    i += 1;
  }
  return command;
}
