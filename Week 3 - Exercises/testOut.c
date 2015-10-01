#include <stdio.h>
#include "redirect.h"

int main(int argc, char *argv[])
{
  printf("testOut running\n");
  int b = 0;
  while(b == 0)
  {
    int read;
    size_t nbytes;
    nbytes = 100;
    char *line;

    read = getline(&line,&nbytes,stdin);
    printf("%s", line);

    if(*line == 'e')
    {
      printf("Changing b to 1\n");
      b = 1;
    }
  }
  printf("All done\n");
  return 0;
}
