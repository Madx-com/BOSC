#Reflektion
Til sum opgaven valgte vi at gøre brug af en struct til et forholdsvis simpelt program dette kunne have været undgået da der kun er en variabel der bliver ændret og der ikke er anden data struktur indvolveret er structen overkill til en variabel der skal summes. 

Til List opgaven brugte vi meget tid og fik mange interessante variationer af en thread safe liste en af vores lister kaldet mlist.c hvor vi kom frem til en løsning som helt selv stod for at holde styr på threads og brugeren skulle kun kalde en metdode med to params til add skulle er det første arg Node og det andet Mlist listen som skulle tilføjes til, denne løsning viste sig at være interessant men meget komplekst og ud fra hvad vi kan forstå af det hele så vides det ikke om den er 100% safe. Vi endte med at beholde mlist da vi syntes at denne implementation var den bedste iteration vi havde vi kaldte den nu for list.c og det vil også være den implementering vi bruger i prodcons. 
\newpage
