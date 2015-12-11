# Teori

Virtuel hukommelse giver operativ systemet muligheder som fysisk hukommelse ikke kan give, såsom indikationen af mere hukommelse end der reel er, ved brug af sider.
En side er en blok af data med en given størrelse. Den virtuelle hukommelse benytter sider, således at den side et program efterspørger indlæses til den fysiske hukommelse via lagerenheden og derefter til den virtuelle hukommelses side. Dette giver operativ systemet mulighed for at lave en mængde sider og alt efter processers behov indlæse og skrive data til lagerenheden. Denne teknik er også kaldet "Demand Paging".

Den virtuelle hukommelse består typisk af en sidetabel $pt{d,f,b}$ med kollonerne, data for siden, sidens plads i den fysiske hukommelse(hvis den er indlæst) og et flag der indikerer om siden skal indlæses, skrives til eller eksekveres. Den fysiske hukommelses plads kaldes også for rammer i virtuel hukommelse.

Hvis der er mere fysisk hukommelse eller præcist den samme størrelse som den virtuelle hukommelse, er virtuel hukommelse ligeså hurtig som den fysiske hukommelse, da der ikke skal håndteres for side udskiftninger(bortset fra den første indlæsning af hver side). Dette er dog ikke altid tilfældet da der af flere grunde kan forekomme det som kaldes en "page fault", hvor en process tilgår en side, der ikke er i den fysiske hukommelse mere eller den fysiske hukommelse er nået sin grænse. 

Dette skal den virtuelle hukommelses sideudskiftnings algoritme håndtere, da der skal tages en beslutning om hvilken ramme skal frigives. Hypotetisk set burde antallet af page faults formindskes desto tættere antallet af sider og rammer er, men dette er ikke altid tilfældet som er blevet påvist af Belady's anomalitet.

Til denne opgave fokuseres der på en tilfældig algoritme, en FIFO(First-In-First-Out) algoritme og en custom algoritme af eget valg.

## Tilfældig udskiftning
Den tilfældige sideudskiftnings algoritme er en meget simpel algoritme, da den kræver at der generes et tal mellem 0 og antallet af rammer. Da det er tilfældigt givet ramme lokationer, kan antallet af page faults variere, da den ikke ved om den ramme bliver brugt eller skal til at bruges, hvilket i et senere tilfælde vil skabe endnu en page fault.

## FIFO udskiftning
Denne algoritme er også meget lige til, da man skal give den ramme der er blevet indlæst data i først. Dette kræver at der er behov for en tæller, der holder styr på hvilken ramme der skal frigives. Hver gang en ramme er frigivet forhøjes tælleren med en. Det skal dog huskes at for hver gang tælleren forhøjes skal den stadig være mellem 0 og antallet af rammer. Til dette kan modulo bruges.

## Custom udskiftning
Til custom udskiftnings algoritmen har vi valgt at se på to algoritmer, den ene som er en udvidet form af FIFO udskiftnings algoritmen, Second-Chance algoritmen(også kaldet Clock algoritmen) og den anden værende LRU, Least Recently Used algoritmen som skulle være den mest optimerede end alle førnævnte algoritmer. 

### Second-Chance
Second-Chance ser vi nærmere på pga. at den i værste tilfælde stadig vil have samme antal page faults som FIFO algoritmen og dette mener vi er en acceptabel præmise.
Udover dette er den også en approximation til LRU, så det ville være spændene at måle forskellen mellem disse.

Selve algoritmen gør brug af en reference bit til hver ramme, der sættes til 0 når et element indlæses i hukommelsen med læse flaget og 1 når et element indlæses med skrivnings flaget. Desuden bruger den også en tæller ligesom FIFO.

Når udskiftningsalgoritmen kaldes tjekkes der for et element med 0 som reference bit. Dette tjek startes fra tællerens position. Under gennemløbet sættes de reference bit der er 1 til 0, da dette er deres anden chance, idet da gennemløbet er cirkulært og det møder dette element igen vil den miste sin plads.

### LRU
Least Recently Used algoritmen går ud på at erstatte den side i den fysiske hukommelse der er blevet brugt mindst. Dette kan mindske antallet af page faults, indlæsninger og skrivninger, da man kan antage at det mindst anvendte side nok ikke bliver refereret igen foreløbigt. Teoretisk set så bliver denne algoritme bedre desto flere rammer der er og er en undtagelse for Belady's anomalitet.

LRU algoritmen består af en liste over rammer i den fysiske hukommelse. For hver gang en side opdateres eller tilføjes i rammen sættes denne rammes værdi i listen til en "tids" værdi, hvilket kunne være en tæller. For at finde den mindst anvendte element i den fysiske hukommelse findes det element med den mindste tidsværdi eller hvis man benytter en tæller så det element med den højeste tæller værdi.
