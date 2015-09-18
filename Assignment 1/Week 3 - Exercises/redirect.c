/* 

   Opgave 2

   redirect.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "redirect.h"

int main(int argc, char *argv[])
{
  redirect_stdincmd(argv[1], argv, argv[3]);
  return 0;
}

/* start the program specified by filename with the arguments in argv
   in a new process that has its stdin redirected to infilename and
   wait for termination */
int redirect_stdincmd(char *filename, char *argv[], char *infilename)
{
  pid_t pID = fork();
  if(pID == 0)
  {
    int fid = open(infilename, O_RDONLY);

    close(0);

    dup(fid);

    close(fid);

    execvp(filename, argv);
  }
  return 0;
}

/* start the program specified by filename with the arguments in argv
   in a new process that has its stdout redirected to outfilename and
   wait for termination */
int redirect_stdoutcmd(char *filename, char *argv[], char *outfilename)
{
  pid_t pID = fork();
  if(pID == 0)
  {
    int fid = creat(outfilename, O_APPEND);

    close(1);

    dup(fid);

    close(fid);

    execvp(filename, argv);
  }
  return 0;
}
