\newpage

# Implementation

I dette afsnit beskrives hvorledes implementationen af en virtuel hukommelses side håndtering og udskiftnings algoritmerne beskrevet i teorien.

## Page Fault Håndtering

Til at starte med er det vigtig at implementere basis page fault håndtering, altså hvordan der skal indlæses data fra disken og skrives til disken.

Dette gøres i `page_fault_handler()` metoden. Vi husker fra teorien at en side i en sidetabel har et flag, der kan benyttes til at afgøre hvad sidens behov er. Dette implementere vi med en `switch` erklæring med tre sager. 

Den første sag er 0, altså et flag der hverken har læse eller skrive rettigheder, denne indikerer at denne side ikke er indlæst i hukommelsen. For at indlæse data fra disken benyttes metoderne `page_table_set_entry()`, som sætter sidens rettigheder og ramme, og `disk_read()`, der indlæser data fra disken til den tildelte ramme.
For at finde ud af hvilken ramme siden skal til, tjekkes listen `loaded_pages`, der er en liste over indlæste sider i rammerne, om der er en ledig plads, som indikeres ved -1. 

Hvis det ikke er muligt at finde en ledig ramme, skal en sideudskiftnings algoritme afgøre om hvilken ramme der skal tildeles. Efter en ramme er tildelt, er det nødvendigt at se om det har `PROT_READ|PROT_WRITE` flaget sat, da disse skal skrives til disken med `disk_write()` før frigivelse. Desuden skal den udskiftede side opdateres i sidetabellen med `page_table_set_entry()`.
 
Den anden sag i `switch` erklæringen er `PROT_READ`, der er læse flaget, når dette er tilfældet skal denne side blot have læse samt skrive rettigheder, men ikke decideret skrives til disken med det samme.

## Udskiftning af sider

Når der skal sider fra den fysiske hukommelse benyttes metoden `get_swap_frame()`, der afgør hvilken udskiftningsalgoritme brugeren ville benytte med variablen `pageswap`. Når denne er 0 skal den tilfældige udskiftning foretages, 1 for FIFO udskiftning og 2 for custom(Second-Chance).

### Tilfældig udskiftning
Den tilfældige algoritme gør brug af metoden `lrand48()` for at genere et tilfældigt tal hvorefter rammen findes ved brug af modulo med `nframes`, det maksimalt antal af rammer.

### FIFO udskiftning
FIFO er implementeret præcist efter teorien med en tæller `fifo_counter`, der forhøjes med en efter hver ramme tildeling og derefter sættes til at være mellem 0 og `nframes` ved brug af modulo.

### Custom udskiftning
Denne udskiftningsalgoritme kræver en lidt mere i sin implementering da den har behov for en reference bit til hver ramme, som er implementeret i from af en liste `clock`. Det der skal tages højde for ved implementeringen af denne algoritme er hvordan gennemløbet skal være og hvornår skal reference bitten sættes til 0 og 1.

Ved initialisering af `clock`, sættes alle bit til 0. I `page_fault_handler()` under indlæsning af data efter udskiftning at sidens ramme skal sættes til 0, da side udskiftning foretages i `switch` erklæringens sag 0, hvilket foroven er der hvor siden får et læse flag. I `switch` erklæringens `PROT_READ` sag, skal `clock` sættes til 1 da denne sides flag nu er et læse og skrive flag.

Under selve udskiftningen skal der gennemløbes cirkulært gennem listen.

