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
Banker's algoritme er testet med de to inputfiler der er blevet givet. Ved input.txt ender vi i en uendelig lykke, da `generate_request()` ikke laver brugbare ressurser, andet end 0 0 0. Den stopper tilgengæld input2.txt filen efter den er blevet tjekket via `checksafety()`.

Vi kan ikke se om programmet virker korrekt, andet end at vi kan delvis bekræfte `checksafety()` funktionen da den stopper input2.txt filen.


