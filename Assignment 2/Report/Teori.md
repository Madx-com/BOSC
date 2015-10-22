# Teori
## Pthreads
Pthread er et standardiseret trådbibliotek som bruges til at oprette tråde sådan at man kan køre funktioner parrallelt, samle resultater fra barne tråde til forældretråden, og destruere tråde som blev skabt når man er færdig med at bruge dem.
 
## Mutex
Mutex bruges når man har kritiske sectioner i sin kode og vil synkronisere sine processertråde. Hvis flere tråde bruger en værdi i den kritiske section, og ændre på den parrallelt, vides det ikke hvornår trådene ændre på værdien, dette kan skabe problemer og give forkerte læsninger også kaldet dirty reads. Ved brug af mutex kan man låse disse sectioner af så andre processer/tråde ikke kan tilgå samme section, når processen med låsen er færdig i den section vil mutex frigive låsen og lade andre processer/tråde tilgå den kritiske section. 

## Semaphores
Semaphores er en anden variation til at låse kritiske sectioner af med, de følger dog et andet princip de har en variabel med sig som betegner hvor mange pladser der er til processer/tråde kan køre simultant med hinanden. Når alle pladser er i brug vil semaphoren blocke adgangen til sectionen, og først give adgang når der er plads igen. Følgende kan forståes ved semaphorens metoder: wait og post Hvad de gør har en virkning på den variable som semaphoren har med sig, wait vil sænke variablen med 1 -- reducere antallet a ledige pladser, og post vil hæve variablen med 1 -- øge antallet af ledige pladser. Når variablen når 0 vil wait vente på et post.

## Pthread_cond vs semaphores
Hvor semaphores blockerer kritiske sectioner. Vil Pthread_cond blockere på værdier den har brug for andet steds fra hvis man har 2 tråde hvor at tråd A gør brug af en global variable X som tråd B ændre kan man i tråd A vente på en at B har ændret præcis denne variable x. så foreskellen mellem pthread_cond og semaphore er at pthread_cond blocker på værdier fremfor sectioner, og derved kun blockere når det er højst nødvendigt for hvis tråd B er færdig før A så vil den bare fortsætte fordi variablen allerede er blevet ændret.


## Concurenxy Control:

## Bankers Algorithm:
\newpage
