#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "forback.h"

int main(int argc, char *argv[])
{
  if(*argv[2] == 'a') {
    printf("A\n");
    foregroundcmd(argv[1], argv);
  } else {
    printf("B\n");
    backgroundcmd(argv[1], argv);
  }
  return 0;
}
