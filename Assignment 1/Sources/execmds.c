#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "parser.h"

int executecmds(Cmd *cmds, char *infilename, char *outfilename, int bg)
{
  char **cmd0 = cmds->cmd;
  Cmd *nextcmds = cmds->next;

  pid_t pid = fork();
  int status;
  
  if(pid == 0)
  {
    if(infilename != NULL)
    {
      int fid = open(infilename, O_RDONLY,0);

      close(0);

      dup(fid);

      close(fid);
    }

    if(outfilename != NULL)
    {
      int fid = creat(outfilename,S_IRWXU|S_IRWXG|S_IRWXO);

      close(1);

      dup(fid);

      close(fid);
    }

    if(nextcmds != NULL)
    {
      pipecmd(cmds);
    }
    else
    {
      execvp(*cmd0, cmd0);
    }
  }
  else if(bg != 1)
  {
    waitpid(pid, &status, 0);
  }
  return 0;
}
