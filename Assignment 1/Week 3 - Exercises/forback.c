/* 

   Opgave 1

   forback.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "forback.h"

/* start the program specified by filename with the arguments in argv 
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[])
{
  pid_t pID = fork();
  int status;
  if(pID == 0) 
  {
    printf("Executing child process with foregroundcmd\n");
    execvp(filename, argv);
  } 
  pid_t w = waitpid(pID, &status, 0);
  if(WIFEXITED(status)) 
  {
    printf("Parent: Child process terminated. Exiting...\n");
  } 
  return 0;
}

/* start the program specified by filename with the arguments in argv 
   in a new process */
int backgroundcmd(char *filename, char *argv[])
{
  pid_t pID = fork();
  if(pID == 0)
  {
    printf("Executing child process with backgroundcmd");
    execvp(filename, argv);
  }
  return 0;
}
