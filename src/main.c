#include "shell.h"

int main(int argc, char **argv)
{
    
  if (argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) 
  {
    printHelp();
    exit(0);
  }
    
  //int stampa=0; //flag utilizzato per la ricerca di un carattere speciale ">"
  //char* maggiore; //ciò che viene dopo il ">" quando il comando viene inserito

    controllo_argomenti(argc,argv);

    while (1) {
        
        printf("My own terminal$ > "); //il mio terminale
        fflush(NULL);

        /* Leggo da command line e prendo in input la stringa inserita dall'utente*/
        if (!fgets(line, 1024, stdin))
        {
            return 0;
        }

        int len=strlen(line); //salvo la lunghezza della stringa inserita

        controllo_input(line, len); //controllo che la stringa non abbia spazi o tabulazioni finali

        int input = 0; // Variabile che dice se il comando che sto andando ad eseguire ha come input l'output del comando precendente 
        int first = 1;    //Variabile che definisce se è il primo comando della linea di comando
        char cmd_s[1024]; //variabile di appoggio
        strcpy(cmd_s, line); 
        char* cmd = cmd_s;
        char* next = strchr(cmd, '|');   //ricerco il carattere "|", a cui puntera' next


        /*if(strchr(cmd,'>')) //gestione dell'operatore ">"
        {
            stampa=1; //ho trovato un ">"
            maggiore=strchr(cmd,'>')+2; 
        }*/

        int pipe_err_fd_read; //Variabile a cui verrà assegnato il file descriptor della pipe per la gestione degli errori
        
        /*Analizzo tutto il resto del comando */
        while (next != NULL) 
        {
            *next = '\0';     // Tolgo '|' da "next" sostituendolo con \0, facendo rimanere il primo comando in cmd
            input = processa_comando(cmd, input, first, 0, &pipe_err_fd_read);        //Inizio a processare il primo comando
            cmd = next + 1;
            next = strchr(cmd, '|');                 //Trovo la '|' successiva
            first = 0;                       // Non è più il primo comando
            
            wait(&status);                                    // Aspetto che gli altri comandi terminino
            

            if (status != 0)
            {
                FILE*  devnull_fd;
                devnull_fd = fopen(err_path, "a");
                char error_buf[4096];
                //Leggo quello che c'è dentro la pipe (error processo precedente)
                int error_len = read(pipe_err_fd_read, error_buf, sizeof(error_buf));
                
                stampa_errore(devnull_fd, error_len, error_buf);
                write(STDERR_FILENO, error_buf, error_len);
            }
            close(pipe_err_fd_read);
        }

        input = processa_comando(cmd, input, first, 1, &pipe_err_fd_read);       // Eseguo l'ultimo comando
        
        wait(&status);                                    // Aspetto che gli altri comandi terminino
        
        //Gestione degli errori
        if (status != 0)
        {
            FILE*  devnull_fd;
            devnull_fd = fopen(err_path, "a");
            char error_buf[4096];
            //Leggo quello che c'è dentro la pipe (error processo precedente)
            int error_len = read(pipe_err_fd_read, error_buf, sizeof(error_buf));
            
            stampa_errore(devnull_fd, error_len, error_buf);
            write(STDERR_FILENO, error_buf, error_len);
        }

        close(pipe_err_fd_read);
        
        n = 0;                                        // Rinizializzo n per la prossima linea di comando

        commandToPrint[0]= '\0';
        index1 = 0;
        //stampa=0;   

    }
    return 0;
}
