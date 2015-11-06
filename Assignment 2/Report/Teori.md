# Teori
## Pthreads
Pthread er et standardiseret trådbibliotek som bruges til at oprette tråde. Dette gør at man kan køre funktioner parrallelt, samle resultater fra barne tråde til forældretråden, og destruere tråde som blev skabt når man er færdig med at bruge dem.
 
## Mutex
Mutex bruges når man har kritiske sektioner i sin kode og vil synkronisere sine processertråde. Hvis flere tråde bruger en værdi i den kritiske section, og ændre på den parrallelt, vides det ikke hvornår trådene ændre på værdien. Dette kan skabe problemer og giver forkerte læsninger også kaldet dirty reads. 

Ved brug af mutex kan man låse disse sektioner af, så andre processer/tråde ikke kan tilgå sektionen, når processen med låsen er færdig i sektionen vil mutex frigive låsen og lade andre processer/tråde tilgå den kritiske sektion. 

## Semaphores
Semaphores er en anden variation til at låse kritiske sektioner af med. De følger et andet princip med, at de har en variabel med sig som betegner hvor mange pladser der er til processer/tråde kan køre simultant med hinanden. Når alle pladser er i brug vil semaphoren blokerer adgangen til sectionen, og først give adgang når der er plads igen. 


Semaphorens metoder,`sem_wait()` og `sem_post()`, har en virkning på den variabel som semaphoren har med sig. `sem_wait()` vil sænke variablen med 1 -- reducere antallet a ledige pladser, og post vil hæve variablen med 1 -- øge antallet af ledige pladser. Når variablen når 0 vil wait vente på et post.

### Pthread condition vs semaphores
Hvor semaphores blokerer kritiske sektioner vil pthread condition blokere på værdier den har brug fra et andet sted. Hvis man har 2 tråde, hvor tråd A gør brug af en global variable ${x}$ som tråd B ændrer, kan man i tråd A vente på at B har ændret præcis denne variabel ${x}$. 

Forskellen mellem pthread condition og semaphore er, at pthread condition blokerer på værdier fremfor sektioner, og derved kun blokerer når det er højst nødvendigt.

## Concurenxy Control
Når man har noget data som mange skal bruge på samme tid, hvordan opnås dette uden at ændringerne der bliver foretaget ikke ender med at være forkerte, når andre skal bruge dem? 

Dette er grundlaget for concurrency control, at sikre at samtidige ændringer håndteres korrekt og efter hensigten. I concurency control har man transaktioner med processer som udfører nogle handlinger der generelt betegnes som læse- og skrivehandlinger. Hvis man har to transaktioner som begge har adgang til ressource ${x}$, så kan en handling se sådan ud:

* ${read(x), write(x,value)}$

Hvis begge nu skulle lave en ${write(x, 34)}$ og ${write(x,1000)}$, hvilken skulle så være den der får lov til at ændre den variabel?

Besvarelsen af det spørgsmål afgør om hvorvidt den næste process der laver en ${read(x)}$ ender med et resultat der er brugbart eller ej. For at undgå dette skal man sørge for at ens transaktioner er seriel ækvivalens. Dette kan gøres på mange forskellige måder. En af dem er som mutex, hvor man låser den sektion der er data sensitiv af, indtil man er sikker på at ændringerne har taget efekt. Dette gør at man ikke får dirty reads. Det er dog ikke nok til at kalde det en seriel ækvivalent transaktion. For at de kan være det så kræver det, at dataen ser ud som den ville hvis en transaktion havde kørt den isoleret.

## Banker's Algoritme
Banker's algoritme er en ressource alokerings algoritme som bruges til hovedsageligt til at undgå deadlock situationer. 

Algoritmen benytter matriser til at allokere ressourcer, ${R}$, til processer, ${P}$ og holde styr på hvor mange ressourcer af en ressource type en process har. Den benytter to vektorer, ${available}$ med længen ${n}$ som angiver mængden ressourcer af en given type ${R}$ der er tilgængelige på et givent tidspunkt, og en vektor ${ressource}$ der angiver de maksimalt tilgængelige ressourcer af en type ${R}$.

I algoritmen er der 3 matriser i alt, med størrelsen ${m \times n}$, hvor ${m}$ er antallet af processer og ${n}$ er antallet af resourse typer ${R}$. ${max[m \times n]}$ er en matrise som holder styr på hvor mange resourser en process kan modtage. ${need[m \times n]}$ er en matrise som angiver hvor mange ressourcer en given process mangler for de specifikke ressourcetyper ${R}$. ${allocated[m \times n]}$ er en matrise som håndtere allokeringen af ressourcer på processerne på et givent tidspunkt. Matrisen ${need}$ er beregnet ud fra ${max - allocated}$.

Måden hvorpå algoritmen kan afgøre om det er sikkert at allokere ressourcer til en proces er ved at tjekke om tilstanden efter ressourcerne er allokeret, er en sikker tilstand. En sikker tilstand opnås når alle processer kan færdiggøres i en sikker sekvens. For at opnå en sikker tilstand kan man bruge en metode der benytter sig af to variabler, en vektor ${Work}$, der afspejler ${available}$ vektoren og en bool liste ${Finish[m]}$ med længden ${m-1}$, altså antallet af processer.
Disse variabler bliver brugt til at afgøre om en tilstand er sikker således:

1. Hvis ${Finish[i] == false}$ og ${need[i] \leq Work}$ er sandt forsæt ellers gå til trin 3
2. Sæt ${Work = Work + allocation[i]}$ og ${Finish[i] = true}$ gå til trin 1
3. Hvis ${Finish[i] == true}$ for alle i, hvor ${0 \leq i < m}$.

Hvis de ovenstående trin kan lade sig gøre, betyder dette, at der findes minimum en sikker sekvens hvorledes processerne kan få allokeret ressourcer til at udføre deres arbejde.

Teorien bag håndtering af en forespørgsel er at validere forespørgslen ved at tjekke at ${request[i] \leq need[i]}$ og ${request[i] \leq available[i]}$. Men det er kke nok med blot at validere forespørgslen, da det kan være forespørgslen invalidere andre processers kørsel og skaber en usikker tilstand, så man simulerer allokeringen af ressourcer og benytter ovenstående metode til at se om systemet stadig er i en sikker tilstand, hvis ikke annulleres forespørgslen således at de må vente og prøve senere. 
 
\newpage
