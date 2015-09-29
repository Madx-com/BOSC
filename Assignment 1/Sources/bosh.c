/* 

   bosh.c : BOSC shell 

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "print.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname(char **hostname)
{
  char hostfile[] = "/proc/sys/kernel/hostname"; /* hostname file */

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read; 

  fp = fopen(hostfile,"r"); 
  if((read = getline(&line, &len, fp)) != -1) /* get the hostname from line 1 */
  { 
    strtok(line,"\n"); /* remove newline token */
    *hostname = line;
  }
  return *hostname;
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  int fid;
  int pd[2];
  char **cmd;
  Cmd *cmd0;

  cmd0 = shellcmd->the_cmds;
  cmd = cmd0->cmd;

  if(shellcmd->rd_stdin != NULL)
  {
    
  }

  if(shellcmd->rd_stdout != NULL)
  {
    
  }

  if(shellcmd->background)
  {
    
  }

  if(cmd0->next)
  {
    
  }

  execvp(*cmd,cmd);

  if(strcmp(*cmd,"exit") == 0)
  {
    return 1;
  }
  printshellcmd(shellcmd);

  return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

  /* initialize the shell */
  char *cmdline;
  char *hostname[HOSTNAMEMAX]; /* changed to a pointer */
  int terminate = 0;
  Shellcmd shellcmd;

  if (gethostname(hostname)) {
    /* parse commands until exit or ctrl-c */
    while (!terminate) {
      printf("%s", *hostname);
      if (cmdline = readline(":$ ")) {
	if(*cmdline) {
	  add_history(cmdline);
	  if (parsecommand(cmdline, &shellcmd)) {
	    terminate = executeshellcmd(&shellcmd);
	  }
	}
	free(cmdline);
      } else terminate = 1;
    }
    free(*hostname);
    printf("Exiting bosh.\n");
  }

  return EXIT_SUCCESS;
}

