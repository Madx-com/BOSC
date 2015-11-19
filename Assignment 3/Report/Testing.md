\newpage

# Testing

For at teste implementationen af udskiftnings algoritmerne er der implementeret tre variabler `fault_counter`, `write_count` og `read_count`, der angiver henholdsvis antallet af page faults, disk læsninger og disk skrivninger. Bemærk at når man første gang laver programmet med `make` vil der forekomme to advarsler, som kommer fra page_table.c og program.c, hvilket er filer der ikke er foretaget ændringer.

Programmet køres på følgende måde:

`./virtmem npages nframes <rand|fifo|custom> <sort|scan|focus>`

Generelt er der testet ved brug af udskrifter af variabler ved brug af `page_table_print_entry()` og `print_second_chance`, men for at afgøre og måle de forskellige algoritmer mod hinanden har vi kørt hver algoritme igennem hvert program med 100 sider i alt med varierende rammer og aflæst de tre variabler der udskrives til sidst i programmet. Med denne information opstilles tabeller og der udarbejdes diagrammer for hver af de forskellige programmer med de forskellige algoritmer.
Resultaterne af disse test kan ses i Appendix A.

## Diskussion
Ud fra resultaterne kan man se at den tilfældige algoritme er meget hurtigere end de to andre. Det skal dog bemærkes, at selvom denne er hurtigere med disse programmer, så kunne det blive værre hvis `lrand48()` ikke genereret et uniform tilfældigt tal og før hver kørsel blev seedet et tal, da dette kunne forudsage et værste tilfælde hvor det tilfældige tal er det samme i alle tilfælde. Der er dog tilfælde med mindre sider og rammer hvor Second-Chance kan have færre page faults, indlæsninger og skrivninger.

Desuden er det værd at bemærke at Second-Chance kun er bedre end FIFO med 1 page fault i de fleste tilfælde. Nærmere udforskning med mindre tal har dog vist at der kan være større forskel, som f.eks. at køre Second-Chance og FIFO med 4 sider og 3 rammer med sort. Dette passer efter teorien at Second-Chance vil i værste tilfælde have lige så mange page faults som FIFO. Skrivninger og indlæsninger er også meget sammenligneligt med FIFO.


