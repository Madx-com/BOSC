#include <stdio.h>

#include "pipe.h"

int main(int argc, char *argv[])
{
  char *cmd1[] = {"/usr/bin/tr","a-z","A-Z",0};
  char *cmd2[] = {"/bin/ls","-al",0};
  pipecmd(cmd1[0], cmd1, cmd2[0], cmd2);
  return 0;
}
