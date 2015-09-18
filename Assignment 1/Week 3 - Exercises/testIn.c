#include <stdio.h>
#include <stdbool.h>
#include "redirect.h"

#define MAXBUFF 512

int main(int argc, char *argv[])
{
  FILE *inputfile;
  char answer[MAXBUFF];
  char line[MAXBUFF];
  
  printf("In testIn.c size of argv is: %d \n", argc);
  printf("In testIn.c argv[1] is:  %s \n", argv[1]);

  inputfile = fopen(argv[1],"r");

  while(fgets(line,MAXBUFF,inputfile))
  {
    if(sscanf(line,"%s",answer))
    {
     printf("%s",answer);
    }
  }

/**
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
  }**/
  return 0;
}
