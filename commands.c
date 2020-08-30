#include "smash.h"
#include "history.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void executeCommand(char *str)
{
  char *token;
  char *args[4096];
  char *hist = malloc(sizeof(str) + 1);
  int counter = 0;
  int retval;

  strcpy(hist, str); //Adds input command to history ignoring whether or not the command is valid
  token = strtok(str, " "); //Grabs commands from input to be processed
  while (token != NULL) //Loop to process all input strings into array for command iteration
  {
    args[counter] = token;
    token = strtok(NULL, " ");
    counter++;
  }
  args[counter] = NULL; //Null identifier to signal end of processing

    //Internal commands to be handled by smash include "exit", "cd", and "history". All other commands will be processed through the executeExternalCommand function
    if (strcmp(args[0], "exit") == 0)
    {
      clear_history();
      exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
      if (counter > 0)
      {
        retval = chdir(args[1]);
        if (retval == 0)
        {
          fprintf(stdout, "%s\n", args[1]);
        }
        else if (retval == -1)
        {
          perror(args[1]);       
        }
      }
    }
    else if (strcmp(args[0], "history") == 0)
    {
      print_history(1);
    }
    else
    {
      retval = executeExternalCommand(args);
    }
  add_history(hist, retval); //function to append history array with the command and its return value
}

int executeExternalCommand(char **cmd)
{
  pid_t pid = fork(); //Create child for external command processing
  int status;
  int exitStatus;

  if (pid == 0) //If child then process the command calling execvp
  {
    if (execvp(cmd[0], cmd) < 0)
    {
      fprintf(stderr, "-smash: %s: command not found\n", cmd[0]);
      exit(1);
    }
    fclose(stdout);
  }
  else if (pid > 0) //If parent wait on child exit status to be received before processing continues
  {
    int childPid = wait(&status);
    exitStatus = childPid;
    exitStatus = WEXITSTATUS(status);
  }
  return exitStatus;
}
