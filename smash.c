#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "smash.h"
#include "history.h"

#define MAXLINE 4096

int main(void)
{
  setvbuf(stdout,NULL,_IONBF,0);
  char bfr[MAXLINE];
  init_history();
  fputs("$ ",stderr); //Output the first prompt

  //Loop reading commands until EOF or error
  while (fgets(bfr, MAXLINE, stdin) != NULL)
  {
    if (strchr(bfr, '\n') == NULL)
    {
        fprintf(stderr, "Invalid input, line length > %lu\n", sizeof(bfr));
        while(strchr(bfr, '\n') == NULL)
        {
          fgets(bfr, MAXLINE, stdin);
        }
    }
    else if (!isspace(bfr[0]))
    {
      bfr[strlen(bfr) - 1] = '\0';
      executeCommand(bfr);
    }
    fputs("$ ",stderr);
  }
  clear_history();
  return 0;
}
