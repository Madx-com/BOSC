#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "parser.h"

/* execute commands with pipe */
int pipecmd(Cmd *cmds)
{
  int pfd[2];
  pid_t pid;
  int status;

  char **cmd = cmds->cmd;
  Cmd *nextcmds = cmds->next;

  int exe;

  if(cmds != NULL)
  {
    pipe(pfd); /* Create the pipe */

    if((pid = fork()) == 0)
    {
      dup2(pfd[0],0); /* Replace stdin */

      close(pfd[1]);
      
      exe = execvp(*cmd, cmd);
    }
    else
    {
      dup2(pfd[1],1); /* Replace stdout */

      close(pfd[0]);

      pipecmd(nextcmds);
    }
  }
  
  wait(&status);

  close(pfd[0]);
  close(pfd[1]);

  if(exe)
  {
	return exe;
  }

  return 0;
}
