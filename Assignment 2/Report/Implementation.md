# Implementation

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


## Producer-Consumer


## Banker's Algorithm


\newpage
