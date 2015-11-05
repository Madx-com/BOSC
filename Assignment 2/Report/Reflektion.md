#Reflektion
I dette afsnit ser vi retrospekt på programmerne vi har implementeret og hvad vi kan bruge i fremtiden.

Ved at lave alle disse opgaver er vi blevet klogere på hvordan tråde arbejder i et operativ system, samt hvor og hvornår det er relevant at benytte semaphore eller mutexes.

I opgaven med sum valgte vi at gøre brug af en struct til et forholdsvis simpelt program, hvilket som nævnt under Testing, kunne være undgået, da der kun er en variabel der bliver ændret. Hvis det havde været tilfældet at n var anderledes for bestemte processer ville det give mening at beholde structen. 

Med hensyn til implementeringen af linked list, brugte vi meget tid og fik mange interessante variationer af en thread safe liste. En af vores lister kaldet `mlist.c`(ikke med i sourcecode), kom vi frem til en løsning som helt selv stod for at holde styr på threads og brugeren skulle kun kalde `list_add()` og `list_remove()` på samme måde som i opgave 2.1. Denne løsning viste sig at være interessant, men meget komplekst og ud fra hvad vi kan forstå af det hele så vides det ikke om den er 100% safe. I sidste ende gik vi dog tilbage til at ændre i `list.c`, da det var mere overskueligt. I fremtiden bør vi nok holde os til de simple løsninger så der kan spares tid når man møder de svære opgaver. 

Producer-Consumer opgaven var en god opgave til at lære brugen af semaphores, men grundet en overset `wait()`, der ikke var `sem_wait()` røg der et par timer i debugging.

Fra Banker's algoritme opgave tager vi det med os at forstå den udleveret kode og teorien fuldstændig før vi går i gang med at implementere det, da det var grunden til tiden løb fra os.

\newpage
