# OS1
Project for Operating System 1

Progetto “SHELL CUSTOM”

Gruppo: ASV Componenti:

-Callaioli Sara
-Salvaterra Amanda Asia 
-Carrer Vanes

Il nostro progetto simula molte delle funzioni di una Shell Ubuntu.
All’avvio da makefile (usando il comando “make build”) vengono compilati i file nella cartella src, ovvero il main.c, il file shell.c e l’header file shell.h.

Per eseguire la nostra applicazione bisogna entrare nella cartella “bin” e digitare il comando ./shell.
Oltre ai comandi singoli come “ls” oppure “ls -la” gestiamo i casi di piping e piping multiplo, con annessa stampa su file di output.
Per la gestione dei comandi utilizziamo le Pipe come mezzo di comunicazione.
La nostra shell permette l’utilizzo di editor come pico e nano, così come il comando “cat” per stampare il contenuto di un file.
Nel caso in cui un input termini con “|” abbiamo optato per non dare output, in quanto il comando risulta incompleto. Nel caso invece termini con uno spazio o una tabulazione, queste non vengono considerate nell’esecuzione del comando, che avviene con successo.

Alcune decisioni Implementative:
- Abbiamo optato per l’utilizzo della funzione execvp per l’esecuzione delle funzioni del programma; 
- In molte occasioni utilizziamo funzioni di modifiche delle stringhe presenti nella libreria string.h;
- Per ottenere la data da inserire nell’output abbiamo utilizzato le funzioni della libreria time.h;
