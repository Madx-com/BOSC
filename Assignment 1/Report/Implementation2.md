# Implementation

## Hostname

I bosh.c er der en metode signatur, `​gethostname()`, til at hente brugernavnet. For at finde lokationen af hostname, benyttes det virtuelle filsystem ​`/proc`. ​Den fulde sti til hostname filen er `proc/sys/kernel/hostname`. Vi har lavet den antagelse, at vi kun skal hente den første linje i filen for at få hostname. Dette har vi gjort ved at benytte en `FILE` og metoden ​`fopen()`.

~~~c
FILE *fp;
	char ​*line = ​NULL;
	size_t ​len = ​0;
	ssize_t ​read;
	
	fp = fopen(hostfile, "r");
	if((read = getline(&line, &len, fp)) != -1)) /* get the hostname from line 1 */
	{
		strtok(line, ​"\n"); /* remove newline token */
		*hostname = line;
	}
~~~

## Baggrundsprocesser & Redirection

Implementering af baggrundsprocesser og redirection er gjort i metoden `executecmds()` i filen execmds.c.
Metoden tager imod en kommando, `Cmd`, et filnavn til ​`stdin`, et filnavn til ​`stdout`, og en binær boolean for at vide om programmet skal køre i baggrunden. Den observante læser har bemærket, at metodens parametre er svarende til `Shellcmd`’s felter. 

Metoden bliver kaldt fra bosh.c’s ​`executeshellcmd()`, 
​
~~~c
executecmds(cmds, shellcmd->rd_stdin, shellcmd->rd_stdout, shellcmd)
~~~

Metoden skaber en ny process hvori den tjekker om parametrene er instantieret.

~~~c
...
	if(pid == ​0)
	{
		if(infilename != ​NULL) { ... }
		
		if(outfilename != ​NULL) { ... }
		...
	}
	else if(bg != ​1)
	{
		waitpid(pid, &status, ​0);
	}
...
~~~

Til redirection er der gjort brug af metoderne ​`open()`, ​`close()` og ​`dup()`. Hvis programmet skal køres i baggrunden ventes der ikke på processen.

## Pipe

I metoden ​`executecmds()` tjekkes ​`Cmd` struct’en om der er flere kommandoer og hvis dette er tilfældet kaldes metoden ​`pipecmd()` i filen pipe.c. Metoden tager en kommando som argument. Metoden er rekursiv og derfor startes der med at tjekke om kommandoen er `NULL`. Hvis det ikke er tilfældet oprettes en ​`pipe()` og der startes en ny proces.

~~~c
...
if(cmds != ​NULL)
{
	pipe(pfd); /* Create the pipe */

​	if((pid = fork()) == ​0) /* Child */
	{
		...
		execvp(*cmd, cmd);
	}
	else ​ /* Parent */
	{
		...
		pipecmd(nextcmds);
	}
}
...
~~~

I den nye proces bliver pipe’s ​`stdin` udskiftet med processens `stdin` ved brug af `dup2()`. Tilbage i den gamle proces udskiftes `stdout` ​på samme vis. 

## CTRL+C

Når en bruger trykker Ctrl+C sendes der et signal som kan fanges af det program der bliver kørt.
Helt specifikt er signalet for Ctrl+C en ​`SIGINT`. Når et signal fanges med metoden `signal()`, skal man i dens andet argument angive en metode der skal kaldes og i dette tilfælde er det sat til en tom metode,​ `sig_handler()`. Dette giver den ønskede funktionalitet. 

~~~c
	#include
	<signal.h>
	...
	void ​ sig_handler(int ​ signo) { }
	int ​ main(int ​ argc, ​char ​ *argv[])
	{
		...
		signal(SIGINT, sig_handler);
		...
~~~

Funktioner som “exit” kommandoen og “Command not found” beskeden er også implementeret i bosh.c.

