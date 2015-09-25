#include <stdio.h>

#include "pipe.h"

int main(int argc, char *argv[])
{
  char *cmd[] = {"ls","-al","tr","a-z A-Z"};
  pipecmd(cmd[0], *cmd[1], cmd[2], *cmd[3]);
  return 0;
}
