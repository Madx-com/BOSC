\newpage

#Reflektion

Denne opgave har givet os et større indblik i hvordan et virtuelt hukommelses system kan implementeres i praksis, samt udfordret os med hensyn til valget af en udskiftnings algoritme, da der netop er så mange måder at gøre det på. Vi valgte to ekstra algoritmer, da det ville være spændene at se forskellen mellem dem.

Testene viste dog en meget større forskel end vi havde regnet med, men vi mener at dette skyldes at programmerne danner page faults sekventielt hvilket i sidste ende vil få Second-Chance til at være ligesom FIFO. Det kunne være interessant at se et program med tilfældige siders page fault og sammenligne Second-Chance og LRU.

Vi har også gjort overvejelser om hvorvidt vi skulle implementere et tjek under `page_fault_handler()`'s `PROT_READ` sag, for at se om siden der skal ænderes til et læse og skrive flag, er i rammetabellen. For at implementere dette kræver det blot en `if` erklæring omkring den nuværende metode for at lave tjekket, og blot bruge metoderne `get_swap_frame()` og `page_fault_helper()`, på samme måde som de benyttes i `0` sagen. Grunden til at vi ikke har implementeret dette er på baggrund af opgavebeskrivelsens illustrations eksempel, hvor vi fortolker en side for en `PROT_READ` sag som allerede værende i rammetabellen.

