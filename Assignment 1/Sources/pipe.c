#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "pipe.h"
#include "forback.h"

/* create a pipe between two new processes, executing the programs
   specified by filename1 and filename2 with the arguments in argv1
   and argv2 and wait for termination */
int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[])
{
  int pfd[2];
  pipe(pfd); /* Create the pipe */

  pid_t pid = fork();
  int status;

  if(pid == 0)
  {
    dup2(pfd[0],0); /* Replace stdin */

    close(pfd[1]);

    execvp(filename1, argv1);
  }
  else
  {
    dup2(pfd[1],1); /* Replace stdout */

    close(pfd[0]);

    execvp(filename2, argv2);
  }

  waitpid(pid,&status,0);

  close(pfd[0]); 
  
  close(pfd[1]);

  return 0;
}
