# Implementation

I dette afsnit er der beskrevet de tanker vi har gjort os omkring vores implementation af de fire opgaver. Bemærk at der ikke bliver beskrevet meget om testing, da det er i afsnittet Testing. 

## Sum(Sqrt)
Vi tager udgangspunkt i bogens implementation af et sum program, der benytter en tråd til at beregne summen fra 0 til et givent tal.
Programmet er ret simpel siden den kun benytter en tråd, men det viser hvordan en tråd starter med `pthread` til at udføre en given funktion.
Vores program er anderledes idet det skal benytte flere tråde, hvilket betyder at arbejdet skal opdeles.Desuden skal summen være af kvadratrod. 

\begin{center}
${\sum\limits_{i = 0}^{N} \sqrt i}$
\end{center}

Programmet skal tage imod to typer af input tallet der skal summeres op til og et tal der angiver hvor mange tråde der skal køres. Ud fra en antagelse vi godt må gøre os, at resultatet af ${N/t}$ er et heltal, hvor ${t}$ er antallet af tråde. Med denne antagelse kan vi ligeligt opdele arbejdet mellem trådene.

Vi har lavet en `struct` til at give som argument, da `pthread_create(pthread_t *tid, pthread_attr_t *attr, void *method, void *param)` kun tager et parameter og vi har behov for at give to parametre, `n` og `sqrtsum`. Summen er dog det eneste tal der ændres på, mens `n`, ${N/t}$, bliver sat før nogen tråde starter og derfor kunne man i retrospekt godt have ladet være med at lave en `struct`.

Programmet bruger desuden en `mutex`, som den låser når der skal lægges til `sqrtsum`. Dette sikre os, at flere tråde ikke opdatere summen samtidig.

### Programtid
Til at se program tiden har vi gjort brug af `<sys/time.h>` og dermed benytte de to `struct`, `timezone` og `timeval`, til at beregne tiden.

Vi har så kørt programmet med ${N = 100000000}$ og ${t = {1,4,8,16}}$. Derudover har vi kørt programmet i flere kerner, ved at bruge `parallel`, som skulle være hurtigere.

~~~
ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode$ ./sqrtsum 100000000 1
sqrtsum = 666666671666.567017
Total time(ms): 1690

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode$ ./sqrtsum 100000000 4
sqrtsum = 666666671666.513916
Total time(ms): 751

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode$ ./sqrtsum 100000000 8
sqrtsum = 666666671666.464111
Total time(ms): 731

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode$ ./sqrtsum 100000000 16
sqrtsum = 666666671666.476440
Total time(ms): 727

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode$ parallel ./sqrtsum ::: 100000000 ::: 1
sqrtsum = 666666671666.567017
Total time(ms): 1685

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode$ parallel ./sqrtsum ::: 100000000 ::: 4
sqrtsum = 666666671666.513916
Total time(ms): 716

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode$ parallel ./sqrtsum ::: 100000000 ::: 8
sqrtsum = 666666671666.463989
Total time(ms): 716

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode$ parallel ./sqrtsum ::: 100000000 ::: 16
sqrtsum = 666666671666.476562
Total time(ms): 716
~~~

Der er en klar forskel mellem at bruge èn tråd eller flere tråde, men man kan ikke sige, at flere tråde giver et hurtigere program. Det kommer an på opgaven trådene har og i det her tilfælde er det at beregne kvadratrods summen, hvor belastningen afhænger af størrelsen på ${N}$. Den værdi der blev testet med viser at der ikke er den store forskel ved at bruge 4 eller 16 tråde, men det kunne der være hvis tallet nu var 10 gange større.

Når programmet bliver kørt med `parallel` er der ikke den store forskel når man benytter en tråd, hvilket er meningen. Når der tilgengæld benyttes flere tråde er den hurtigere, som antaget, men der er ingen forskel mellem at bruge 4 eller 16 tråde, hvilket er lidt overraskende. Vi har ingen konkret forklaring på dette tilfælde, men antager, at det skyldes operationens simplicitet. 

## Linked List
Linked list er en datastruktur der er bestående af noder med to værdier, deres element værdi(kunne f.eks. være en `string`, `int` osv.) og den næste node i listen.
Selve listen kender til sin første og sidste node samt sin længde. Når listen er tom, dvs. længden er nul, er der kun en node i den og det er `first` som peger på `NULL` som den næste node i listen. `first` kendes som root og kan/må ikke fjernes. Listen som vi skal implementere er en FIFO(first-in-first-out), hvilket betyder at når vi tilføjer en node ryger den bagerest i kæden og bliver til den sidste node. Omvendt når vi fjerner en node skal den første ikke root node fjernes.

Vi er allerede blevet givet strukturen af noden og listen, samt funktioner til at oprette lister og noder. Opgaven er at implementere tilføj og fjern funktioner til listen.

### Tilføj og Fjern
Når man tilføjer en node skal man tage højde for om det er den første node efter root eller ej. Hvis dette er tilfældet, vil længden være 0 og dvs. at noden der tilføjes skal sættes som at være lig den root's næste node. Desuden er dette ikke blot den første node i listen, men også sidste node i listen så det skal den også sættes til. 

```c
if(l->len == 0)
{
	l->first->next = n;
	l->last = n;
}
``` 
Hvis det ikke er tilfældet er noden der skal tilføjes den nye sidste node i listen, så vi skal opdatere den sidste node.

```c
else
{
	l->last->next = n;
	l->last = n;
}
```

Til sidst skal længden incrementes med 1.

Når vi skal fjerne en node skal vi opdatere root's næste node, da det nu skal være den næste nodes næste node. Desuden skal der tjekkes for om listen ikke er tom.

```c
if(l->len > 0)
{
	n = l->first->next; 
	l->first->next = n->next;
	l->len -= 1;
}
```

### Problemer med flere tråde
Hvis flere tråde skal tilføje eller fjerne noder i listen kan der opstå problemer med integriteten af den data der er i listen. Listen behøver ikke nogen bestemt rækkefølge så vi kan godt tilføje noder tilfældigt, men hvis der bliver lavet et kald til tilføj via en tråd, så vil man gerne have at den node kommer ind i listen. Desuden hvis man lige har læst længden af listen, som ikke er tom, og man fjerner en node, er der ikke garanti for at listen ikke er tom på det tidspunkt denne tråd får lov til at udføre sin handling. Det resulterer i at du får en `NULL` node, og det kan i værste fald give en runtime error hvis programmet der kaldte på fjern afhang kraftigt af noden.


### Mutex og Linked List
En god måde at sikre at flere tråde kan arbejde på listen samtidig er ved at lave et låse system med en nøgle, hvilket mutex er. Det betyder at når nøglen er fri kan man godt foretage operationer i listen, og hvis den ikke er så venter du indtil den bliver det.

Det man så skal overveje er hvor man skal sætte sine låse, er det brugeren af programmet der skal gøre det i sin tråd funktion? Det kunne godt lade sig gøre, men er ikke særlig hensigtsmæssig, som i forhold til hvis listen selv kunne håndtere dette. Det betyder så, at listen skal have en nøgle. Vi har tilføjet mutex i listens `struct` i `list.h`, da dette gør at man er sikret at en liste har en speciel lås.

Forrige sektion afklarede at det kun var nødvendigt at tilføje en låsemekanisme når man tilføjer eller fjerner noder, idet det er de kristiske sektioner i listen.
Dette har vi gjort ved brug af `pthread_mutex_lock(l->mtx)` og `pthread_mutex_unlock(l->mtx)`.


## Producer-Consumer problemet
Producer-Consumer problemet er et velkendt problem, hvor producenter producerer varer, som consumers konsumerer. Problemet ligger i at stoppe consumers med at konsumerer vare når der ikke er flere varer og ligeledes at stoppe producenter med at producerer varer når lageret er fyldt.
Dette kan gøres ved brug af semaphores, da den netop har den funktionalitet der efterspørges, da vi kan waite og poste, som afklaret i teorien.
I programmet har vi implementeret en `struct PC`, der indeholder vores linked list, der fungerer som lager, og tre semaphores, `full` til at indikerer vare i lageret, `empty` til at indikerer plads i lageret og `mutex` der agerer som en mutex.

Programmet skal tage følgende fire input:

* Antallet af producers, `PRODUCERS`
* Antallet af consumers, `CONSUMERS`
* Størrelsen på lageret, `BUFFSIZE`
* Antallet af vare der skal produceres i alt, `PRODUCTS_IN_TOTAL`

Vi initialiserer vores semaphores således at `full` sættes til 0 og `empty` sættes til lagerets størrelse. 

~~~c
sem_init(&prodcons.full, 0, 0);
sem_init(&prodcons.empty, 0, BUFFSIZE);
sem_init(&prodcons.mutex, 0, 1);

~~~

Dette gøres grundet logikken i `sem_wait()` der formindsker værdien og `sem_post()` der forhøjer værdien. Dermed sætter vi producers til at holde øje med `empty` og sende et signal til `full` med post, og omvendt med consumers.

~~~c
void *producer(void *param)
{
	.
	sem_wait(&empty);
	.
	sem_post(&full);	
	.
}

void *consumer(void *param)
{
	.
	sem_wait(&full);
	.
	sem_post(&empty);	
	.
}
~~~

For at holde styr på hvor mange produkter der produceres og konsumeres har vi to counters `p` og `c`, der bliver tjekket op med produkter i alt i deres respektive funktioners while løkker. Mutex semaphoren anvendes til at opdatere counters.

## Banker's Algoritme
Til vores implementation af Banker's algoritme er vi blevet tildelt en fil med den grundliggende implementation af algoritmens struktur.
I den givet implementation er der en `struct State` som består af de elementer Banker's algoritme kræver forklaret i Teori afsnittet.
Det første der manglede at blive implementeret i filen var allokeringen af hukommelse til `State`. Til dette anvender vi `malloc()` og `sizeof()` funktionerne.
Med disse funktioner kan vi beregne det antal bytes i hukommelsen vi kommer til at anvende. Der hvor dette kan være problematisk er når der skal allokeres hukommelse til arrays og 2D-arrays. I et array skal man huske at allokere hukommelse i forhold til dens længde, så derfor ganges dette med `sizeof()` af typen.
For 2D-arrays kræver det to skridt, hvor i det første skridt tildeles hukommelse af det andet array og i det andet skridt ved brug af en loop allokeres hukommelse til det første array.

~~~c
s = (State *)malloc(sizeof(State));
s->resource = (int *)malloc(sizeof(int) * n);
s->available = (int *)malloc(sizeof(int) * n);
s->max = (int **)malloc(sizeof(int *) * n);
s->allocation = (int **)malloc(sizeof(int *) * n);
s->need = (int **)malloc(sizeof(int *) * n);

for(i = 0; i < m; i++)
{
	s->max[i] = (int *)malloc(sizeof(int) * m);
	s->allocation[i] = (int *)malloc(sizeof(int) * m);
	s->need[i] = (int *)malloc(sizeof(int) * m);
}
~~~

Det næste handlede om at tjekke om programmet var i en sikker tilstand eller en usikker tilstand.
Ved at følge teorien lavede vi en metode `checksafety()` der benytter sig af to arrays, `work` og `finish`. Implementationen følger teorien ved at tjekke om ${finish[i] == 0}$ og efterfølgende tjekke om ${need[i][j] \leq work[j]}$. 
Metoden simulerer kørsel af processer i en while-løkke, der tjekker de to nævnte kriterier. Hvis kriterierne er sande frigives ressourcerne til `work` og `finish` sættes til 1. Til sidst tjekkes der om der er nogle processer der stadig ikke kan køres og der returneres 0 hvis der er og 1 hvis der ikke er.

Med denne metode implementeret fokuseret vi på funktionen der skal benytte den mest, og det er `resource_request()`, som skal tage imod forespørgsler om ressourcer og afgøre ved brug af `checksafety()` om det er sikkert at allokere ressourcerne.
Denne metode tjekker først om forespørgslen er mindre eller lig behovet for processen og om der er nok ledige ressourcer på nuværende tidspunkt. Hvis disse to kriterier er opfyldt, laves der en backup af processens nuværende sikre tilstand, hvorefter ressourcerne allokeres. Efter dette benyttes `checksafety()` til at afgøre om den nye tilstand er sikker. Hvis den ikke er sikker, benyttes backup variablerne til at gå tilbage til den tidligere sikre tilstand.

Til sidst implementeret vi funktionen `resource_release()`, der skal frigive ressourcer fra en proces afhængig af en forespørgsel. Dette er ret ligetil da det eneste vi skal sikre er at forespørgslen ikke er større end de allokeret ressourcer for den givne process.

