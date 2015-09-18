#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "forback.h"

int main(int argc, char *argv[])
{
  foregroundcmd(argv[1], argv);
  return 0;
}
