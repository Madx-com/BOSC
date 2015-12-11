# Baggrund
Ifølge opgavebeskrivelsen skal programmet kunne opfylde følgende specifikationer: 

* Uafhængighed: Programmet skal kunne virke uden brug af andre shells f.eks. ved at benytte et systemkald ​system() til at starte en bash. 
* Når programmet kører skal det vise navnet på den host der er logget ind. 
* Udskrive en “Command not found” meddelelse når man skriver en kommando der ikke findes i systemet. 
* Kommandoer skal kunne køres i baggrunden, ved brug af ​‘&’. ​Dvs. at man skal kunne fortsætte med at benytte ​bosh. ​
* Man skal kunne lave redirection af ​stdin og stdout ved at benytte, ​‘<’ ​og ‘>’. 
* Det skal være muligt at anvende pipes. Dvs. at det skal være muligt at tage flere 
kommandoer med ​‘|’ som separator. Den venstre specificeret kommando skal benyttes som input til den højre specificeret kommando. 
* Man skal kunne lukke shellen ved at bruge kommandoen ​‘exit’.
* Ctrl+C skal ikke lukke shellen, men afslutte det kørende program i ​bosh. ​
 
 
De udleverede filer indeholder tre C ­filer: 

* bosh.c 
* parser.c 
* print.c 

For at løse opgaverne specificeret er der ikke behov for at ændre i parser.c og print.c. 
I filen parser.h er der specificeret de to struct som bliver benyttet i programmet, ​Cmd og Shellcmd.
 
Cmd ​har en hægtet liste datastruktur, der består af en kommando streng og peger på den næste kommando i rækken. Når der er flere kommandoer skal der anvendes pipe. 

Shellcmd indeholder felter relateret til de andre specificeret opgaver, som hvis brugeren har specificeret at programmet skal køres i baggrunden, eller specificeret en form af redirect.
 
\newpage
