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

## Bankers Algorithm
Banker's algoritme er en resource alokerings algoritme som bruges til hovedsageligt til at undgå deadlock situationer. 

Algoritmen benytter matriser til at allokere ressoourcer til processer og holder styr på hvor mange ressourcer af typen R, en process har fået. Den benytter to vektorer, ${available}$ med længen m som angiver den maksimale mængde ressourcer af en given type R der er tilgængelig, og ${resource}$ der angiver de maksimalt tilgængelige ressourcer af en type R.

I algoritmen er der 3 matriser i alt, med størrelsen ${n \times m}$, hvor n er antallet af processer og m er antallet af ressourse typer R. ${max[n \times m]}$ er en matrise som holder styr på hvor mange resourser R en process kan modtage. ${need[n \times m]}$ er en matrise som angiver algoritmen, hvor mange resourcer en given process mangler for de specifikke typer R. ${allocated[n \times m]}$ er en matrise som håndtere allokeringen af ressourcer på processerne på et givent tidspunkt. 

Safe state og unsafe state er to stadier som er kernen i denne algoritme. Det er dette der afgør om der er nok resourcer til en proces kan udføre sit arbejde og afslutte, uden at de andre processer kommer til at deadlock. En safe state er når en process kan få alle ressourcer den har brug for samtidig med at der er nok til at en anden process kan få ressourcer. 

Hvis man antager at der er 5 resourcer af typen B og der er tre processer 1, 2, 3. Proces 1 kommer med en forespørgsel om at få 3B ressourcer. For at tilfredse dette behov så vil Banker's algoritme tjekke safe states, ved at se om den kan allokere de ressourcer. Hvis ja, kigger den på tilstanden efter resourcen er blevet tildelt og ser om der er en proces der stadig kan få tilfredsstillet sit behov for rescourcer, dette gentages så den kan se at alle processer stadig kan køre. 

Med denne metode undgås deadlocks fordi der på intet tidspunkt er processer der venter på rescourcer som de aldrig kan få, mindst en process kan altid få nok rescourcer til at afslutte. Denne algoritme har dog nogle downsides som er ret markante. En af dem er, at man er nød til at vide på forhand hvilke og hvor mange ressourcer en process maksimum skal bruge, hvilket er et sjældent scenarie. Udover dette så at antage, at en proces skal frigive alle sine resourcer når den terminere er i sig selv et vigtigt for algoritmen, men da man ikke kan sige levetiden på en given proces, kan man måske vente flere minutter, timer eller dage på, at de ressourcer tilbage. Dette er ikke praktisk for et realistisk system. I vores verden i dag, er det ikke logisk at have et statisk antal processor, da verden er begyndt at bruge mange flere tråde som bliver oprettet og lukket igen og igen i løbet af et programs levetid.
 
\newpage
