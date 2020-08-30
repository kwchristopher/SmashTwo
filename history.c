#define _GNU_SOURCE
#include "history.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Cmd *history[MAXHISTORY];
int current;
int oldest;

void init_history() //Function to initialize the history array keeping track of where the most current point is in the array and the oldest point for printing purposes
{
  current = 0;
  oldest = 0;
  history[0] = malloc(sizeof(struct Cmd));
}

void add_history(char *cmd, int exitStatus)
{
  //History will only store and print the 10 most recent commands, if the total number of commands is less than 10 the history array is simply appended with the input command.
  //If the total number of commands is greater than 10 the array will loop back on itself and store the new command in the current % 10 position.
  if (current < 10)
  {
    if (current != 0)
    {
      history[current] = malloc(sizeof(struct Cmd));
    }
    history[current] -> cmd = strdup(cmd);
    history[current] -> exitStatus = exitStatus;
  }
  else
  {
    history[current % 10] -> cmd = strdup(cmd);
    history[current % 10] -> exitStatus = exitStatus;
    oldest = (current % 10) + 1;
  }
  current++;
}

void clear_history(void)
{
  int i;
  for (i = 0; i < current % 10; i++)
  {
    history[i] -> cmd = NULL;
    history[i] -> exitStatus = 10;
    free(history[i]);
  }
}

void print_history(int firstSequenceNumber)
{
  int i;
  if (current < 10)
  {
    for (i = 0; i < current; i++)
    {
      fprintf(stdout, "%d [%d] %s\n", i + 1, history[i] -> exitStatus, history[i] -> cmd);
    }
  }
  else
  {
    for (i = 0; i < 10; i++)
    {
      fprintf(stdout, "%d [%d] %s\n", current - 10 + i + 1, history[(oldest + i) % 10] -> exitStatus, history[(oldest + i) % 10] -> cmd);
    }
  }
}
