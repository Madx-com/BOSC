#include <string.h>
#include <stdio.h>

#include "redirect.h"

int main(int argc, char *argv[])
{
  if(argc == 3)
  {
    char str[1000];
    printf("Enter words:\n");
    scanf("%s\n",str);
    printf("%s\n",str);
  }
  else
  {
    printf("Bla\n");
    printf("LoL another Bla\n");
  }
  return 0;
}
