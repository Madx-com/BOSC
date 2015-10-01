#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "redirect.h"

int main(int argc, char *argv[])
{
  if(argc == 3)
  {
    redirect_stdincmd("./testInOut",argv,argv[1]);
  }
  else
  {
    redirect_stdoutcmd("./testInOut",argv,argv[1]);
  }
}

/* start the program specified by filename with the arguments in argv
   in a new process that has its stdin redirected to infilename and
   wait for termination */
int redirect_stdincmd(char *filename, char *argv[], char *infilename)
{
  int fd[2];
  pid_t pID;
  int status;
  if((pID = fork()) == 0)
  {
    printf("In child process started...\n");
    int fid = open(infilename, O_APPEND,0);

    close(0);

    dup(fid);

    close(fid);

    execvp(filename, argv);
  }
  else
  {
    waitpid(pID,&status,0);
    if(WIFEXITED(status)) {
      printf("Parent is back\n");
    }
  }
  return 0;
}

/* start the program specified by filename with the arguments in argv
   in a new process that has its stdout redirected to outfilename and
   wait for termination */
int redirect_stdoutcmd(char *filename, char *argv[], char *outfilename)
{
  pid_t pID = fork();
  int status;
  if(pID == 0)
  {
    printf("Out child process started...\n");
    int fid = creat(outfilename,S_IRWXU|S_IRWXG|S_IRWXO);

    close(1);

    dup(fid);

    close(fid);

    execvp(filename, argv);
  }
  else
  {
    printf("Parent is waiting...\n");
    waitpid(pID,&status,0);
    if(WIFEXITED(status))
    {
      printf("Parent: Child process terminated. Exiting...\n");
    }
  }

  return 0;
}
