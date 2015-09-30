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
#include "forback.h"
#include "pipe.h"

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
  printshellcmd(shellcmd);
  char **cmd0;
  Cmd *cmds;

  cmds = shellcmd->the_cmds;
  cmd0 = cmds->cmd;

  if(strcmp(*cmd0,"exit") == 0)
  {
    return 1;
  }

  if(cmds->next != NULL)
  {
    Cmd *cmds1 = cmds->next;
    char **cmd1 = cmd0;
    char **cmd2 = cmds1->cmd;
    
    do
    {
      pipecmd(*cmd1, cmd1, *cmd2, cmd2);
      if(cmds1->next != NULL)
      {
	cmd1 = cmd2;
        Cmd *tempcmd = cmds1->next;
        cmds1 = tempcmd;
        cmd2 = cmds1->cmd;
      }
    }
    while(cmds1->next != NULL);
  }

  if(shellcmd->background == 1)
  {
    backgroundcmd(*cmd0, cmd0, shellcmd->rd_stdin, shellcmd->rd_stdout);
  }
  else
  {
    foregroundcmd(*cmd0, cmd0, shellcmd->rd_stdin, shellcmd->rd_stdout);
  }

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

