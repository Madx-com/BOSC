/* 

   Opgave 1

   forback.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "forback.h"

/* start the program specified by filename with the arguments in argv 
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[], char *infilename, char *outfilename)
{
  pid_t pID = fork();
  int status;
  if(pID == 0) 
  {
    if(infilename != NULL)
    {
      int fid = open(infilename, O_APPEND,0);

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

    execvp(filename, argv);
  }
 
  pid_t w = waitpid(pID, &status, 0);

  if(WIFEXITED(status)) { } 

  return 0;
}

/* start the program specified by filename with the arguments in argv 
   in a new process */
int backgroundcmd(char *filename, char *argv[], char *infilename, char *outfilename)
{
  pid_t pID = fork();
 
  if(pID == 0)
  {
    if(infilename != NULL)
    {
      int fid = open(infilename, O_APPEND,0);

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

    printf("Executing child process with backgroundcmd");

    execvp(filename, argv);
  }

  return 0;
}
