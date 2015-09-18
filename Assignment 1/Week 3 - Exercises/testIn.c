#include <stdio.h>
#include <stdbool.h>
#include "redirect.h"

int main(int argc, char *argv[])
{
  bool b = false;
  while(!b)
  {

    char *p;

    scanf('%c', &p);
    printf('%c', *p);
    if('e' == *p)
    {
      b = true;
    }
  }
  return 0;
}
