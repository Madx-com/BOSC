#include <stdio.h>
#include <stdbool.h>
#include "redirect.h"

#define MAXBUFF 512

int main(int argc, char *argv[])
{
  /*
  FILE *inputfile;
  char answer[MAXBUFF];
  char line[MAXBUFF];

  printf("In testIn.c size of argv is: %d \n", argc);
  printf("In testIn.c argv[1] is:  %s \n", argv[1]);

  inputfile = fopen(argv[2],"r");

  while(fgets(line,MAXBUFF,inputfile))
  {
    if(sscanf(line,"%s",answer))
    {
     printf("%s",answer);
    }
  }
  */
  printf("testIn running\n");
  int b = 0;
  while(b == 0)
  {
    int read;
    size_t nbytes;
    nbytes = 100;
    char *line;

    read = getline(&line,&nbytes,stdin);
    /*printf("%d\n",read);*/
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
