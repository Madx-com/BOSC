# Testing

## Sum(Sqrt)
For at teste dette program har vi udregnet nogle små sum af sqrt's som vi har tjekket op mod det endelige resultat.

~~~
./sqrtsum 4 2
sqrtsum = 6.146264

./sqrtsum 6 3
sqrtsum = 10.831822
~~~

Da disse har vist sig at være korrekte har vi antaget at beregningen er korrekt. Desuden har vi haft `printf` statements til at se hvor meget enkelte tråde har lagt til summen, men disse er fjernet nu da vi ved at den arbejder med flere tråde uden problemer.
Det er dog værd at bemærke, at de sidste få decimaltal er varierernde i forhold til antallet af tråde man benytter, hvilket giver mening idet desto flere opdelinger af tråde desto mindre præcist bliver beregningen når der lægges til.

Da vi har gjort os antagelsen for input er to tal og de kan divideres til et heltal, har vi ikke gjort meget ud af, at tjekke om brugeren giver det rigtige input.

## Linked List
For at teste om vores implementation af `list.c`'s tilføj og fjern funktioner virkede brugte vi den `main.c` fil der blev givet. Den samme fil er nu blevet til testprogrammet for at tjekke om listen kan håndtere flere tråde.

Programmet tager to inputs. Antallet af noder man vil sætte ind i listen og antallet man vil fjerne. Dette giver os muligheden for at tjekke flere scenarier.
Vi har testet fire scenarier:

* Tilføj uden at fjerne
* Fjern uden at tilføje
* Tilføj 20 og Fjern 30
* Tilføj 25 og Fjern 10

~~~
ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode/list$ ./fifo 4 0
Success! List is correct. Diff:4 Length:4

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode/list$ ./fifo 0 10
Success! List is correct. Diff:-10 Length:0

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode/list$ ./fifo 20 30
Success! List is correct. Diff:-10 Length:0

ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode/list$ ./fifo 25 10
Success! List is correct. Diff:15 Length:15
~~~

Igen som i den tidligere opgave med summen, har vi haft `printf` statements til at angive produkter der kommer ind og ud, men efter vi fik bekræftet at dette lykkedes i forhold til de tre scenarier, fjernede vi dem og lavede en mere ren succes og fejl besked. Scenarierne viser differencen mellem tilføj og fjern og hvad listens længde er til sidst. Rækkefølgen af elementerne er tilfældig da vi ikke har lavet nogen restriktion for det.

## Producer-Consumer
Dette program er testet på baggrund af sine counters. Hvis de begge er lig `PRODUCTS_IN_TOTAL`, betyder det at alle produkter er produceret og konsumeret.
Derudover printer den alle produkter der bliver produceret og konsumeret ligesom i opgavebeskrivelsen. Under testing fandt vi dog ud af at der i visse tilfælde, efter det sidste produkt er konsumeret, sidder programmet fast. Dette antager vi skyldes en af semaphorerne har fejlet eller at en af trådene gik tabt og derfor venter programmet.

~~~c
./prodcons 20 20 10 300
.
.
10. Consumed Item 298: P298. Items in buffer 2 (out of 10)
8. Consumed Item 299: P299. Items in buffer 1 (out of 10)
4. Consumed Item 300: P300. Items in buffer 0 (out of 10)
Success! All products produced and consumed.
~~~

Ved testning af dette program blev vi overrasket over, at man ikke får nogen warnings fra compileren hvis man benytter `wait()` med en semaphore, da vi havde overset at vi på det tidspunkt ikke brugte `sem_wait()` og derfor fik en buffer der blev større end tilladt.

## Banker's Algoritme
Banker's algoritme har vi testet ved brug af tre inputfiler, fil 1 har en sikker start tilstand mens fil 2 og 3 har usikre start tilstande.
For at tjekke om `checksafety()` fungerer efter hensigten og finder usikre start tilstande brugte vi inputfil 2 og 3.
Input fil 1 er ret ligetil, siden intet er allokeret fra starten vil alle processer være i stand til at køre. Dette kan ses i transriptet forneden, hvordan programmet tjekker alle processer og når den kommer tilbage til process 0 afgør den at programmet er i en sikker tilstand.

~~~c
ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode/banker$ ./banker < input.txt
Number of processes: Number of resources: Resource vector: Enter max matrix: Enter allocation matrix: 
Need matrix:
R1 R2 R3 
3  2  2
6  1  3
3  1  4
4  2  2
Availability vector:
R1 R2 R3 
9  3  6
Checking safety of process 0
Checking safety of process 1
Checking safety of process 2
Checking safety of process 3
Checking safety of process 0
Initial state safe!
..
~~~

Input fil 2 vil resulterer i en usikker tilstand da der på intet tidspunkt kan køre en process, grundet need vil altid være større end available, hvilket programmet også vurdere efter at have simuleret alle processers kørsel.

~~~c
ok@ok:~/Dokumenter/BOSC/Assignment 2/Sourcecode/banker$ ./banker < input2.txt
ok@ok:~/Desktop/git-repos/BOSC/Assignment 2/Sourcecode/banker$ ./banker < input2.txt
Number of processes: Number of resources: Resource vector: Enter max matrix: Enter allocation matrix: 
Need matrix:
R1 R2 R3 
1  2  0
1  0  2
1  0  3
4  2  0
Availability vector:
R1 R2 R3 
0  1  0
Checking safety of process 0
Checking safety of process 1
Checking safety of process 2
Checking safety of process 3
Initial state unsafe!
~~~

Kørsel af input fil 3 vil også resulterer i en usikker tilstand, men den bekræfter at simuleringen af processerne er som forventet, da process 0 vil i det første tjek vil være sand og fortælle algoritmen at den minimum skal simulerer en runde af alle processerne en gang til. Dette kan ses forneden.

~~~
ok@ok:~/Desktop/git-repos/BOSC/Assignment 2/Sourcecode/banker$ ./banker < input3.txt
Number of processes: Number of resources: Resource vector: Enter max matrix: Enter allocation matrix: 
Need matrix:
R1 R2 R3 
0  0  0
2  2  2
2  2  2
3  3  3
Availability vector:
R1 R2 R3 
0  0  0
Checking safety of process 0
Checking safety of process 1
Checking safety of process 2
Checking safety of process 3
Checking safety of process 0
Checking safety of process 1
Checking safety of process 2
Checking safety of process 3
Initial state unsafe!
~~~

For at teste resten af funktionaliteten med hensyn til forespørgsler om allokering af ressourcer eller frigivelse af ressourcer kørte vi programmet med inputfil 1.
Vi er dog opmærksomme på at der er nogle problemer med hensyn til den udleveret kode således at der kan forekomme en uendelig løkke grundet en deadlock forudsaget af en starvation, men denne har vi ikke gjort noget ved.

Forneden ses det at process 1 anmoder om ressourcerne 0 0 1, og får dem tildelt. Dette viser at tildeling af ressourcerne går efter hensigten, men det interessante er at se når der kommer forespørgsler der ikke bliver godkendt.

~~~c
..
Process 1: Requesting resources.
Process 1 request vector: 0 0 1 
..
Request leads to safe state. Request Granted!
Vector changed: Availability vector:
R1 R2 R3 
9  3  5
..
~~~

I tilfælde af at en forespørgsel bliver anset som usikker skal den spørge igen efter noget tid som defineret i teorien.

~~~c
..
Process 3 request vector: 1 0 0 
..
Request leads to unsafe state. Request Denied!
Process 3 request vector: 1 0 0 
..
Request leads to unsafe state. Request Denied!
Process 3 request vector: 1 0 0 
..
Request leads to unsafe state. Request Denied!
..
~~~

Froven ses det at der er en forespørgsel fra process 3 på ressourcerne 1 0 0, som bliver anset som usikker i flere tilfælde. Men i senere tilfælde er det meget sandsynligt at der vil forekomme et scenarie hvor den kan betragtes som sikker og får ressourcerne tildelt, hvilket kan ses i det følgende transcript.

~~~c
Process 3: Requesting resources.
Process 3 request vector: 1 0 0 
..
Request leads to safe state. Request Granted!
Vector changed: Availability vector:
R1 R2 R3 
4  1  3
..
~~~

Den sidste funktion i programmet er frigivelsen af ressourcer, hvilket er ret lige til, da den eneste fejl der kan forekomme er en forespørgsel der er større end det der er allokeret for processen, men det er der allerede taget forbehold for og burde ikke fremkomme. Transcriptet forneden viser en release fra process  der gennemføres.

~~~c
..
Process 3: Releasing resources.
Process 3 release vector: 2 0 0 
Released resources
Availability vector:
R1 R2 R3 
5  2  3 
..
~~~

På baggrund af ovenstående transcripts fra programkørsel konkludere vi at algoritmen opføre sig efter teorien.



