/* 

   Opgave 3

   pipe.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "pipe.h"

/* create a pipe between two new processes, executing the programs 
   specified by filename1 and filename2 with the arguments in argv1
   and argv2 and wait for termination */
int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[])
{
  int fd[2];
  pipe(fd); /* Create the pipe */

  close(fd[0]); /* Close read end of the pipe file descriptor */
  close(fd[1]); /* Close write end of the pipe file descriptor */

  pid_t pid1 = fork(); /* Child process 1 */
  pid_t pid2 = fork(); /* Child process 2 */
  int status;

  if(pid1 == 0)
  {
    dup2(fd[0],0); /* Replace stdin */
    close(fd[1]);  /* No need for the other */
    execvp(filename1, argv1);
  }

  if(pid2 == 0)
  {
    dup2(fd[1],1); /* Replace stdout */
    close(fd[0]); /* No need for the other */
    execvp(filename2, argv2);
  }

  wait(&status);

  return 0;
}
