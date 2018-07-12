#include "shell.h"

//Funzione che controlla il formato deli argomenti assegnati 
void controllo_argomenti(int argc, char **argv){  
    strcpy(out_path, "../log/log");
    strcpy(err_path, "../log/err");
    if(argc >= 2)
    {
        char option_out[256];
        char option_err[256];
        if(argc == 2)
        {
            if(!strncmp("--outfile=", argv[1] , 10 ))
            {
                strcpy(out_path, "../log/");
                printf("--outfile= \n");
                strcpy(option_out, &argv[1][10]);
                strcat(out_path, option_out);
                printf("option: %s \n", out_path);
            }
            else if(!strncmp("--errfile=", argv[1] , 10 ))
            {
                strcpy(err_path, "../log/");
                printf("--errfile= \n");
                strcpy(option_err, &argv[1][10]);
                strcat(err_path, option_err);
                printf("option: %s \n", err_path);
            }
            else
            {
                printf("Formato di comando errato. Eseguire scrivendo: \n");
                printf("'--outfile=' + nome_file \nOppure\n");
                printf("'-o ' + nome_file\n");
                exit(0);
            }
        }
        else if(argc>=3 && argc<=5 )
        {
            for(int i = 1; i < argc; i++)
            {
                if(!strcmp("-o",argv[i]) && i!=argc-1)
                {
                    strcpy(out_path, "../log/");
                    strcpy(option_out, argv[i+1]);
                    strcat(out_path, option_out);
                    printf("option: %s \n", out_path);
                    i++;
                }
                else if(!strcmp("-e",argv[i]) && i != argc-1)
                {
                    strcpy(err_path, "../log/");
                    strcpy(option_err, argv[i+1]);
                    strcat(err_path, option_err);
                    printf("option: %s \n", err_path);
                    i++;
                }
                else if(!strncmp("--outfile=", argv[i] , 10 ) )
                {
                    strcpy(out_path, "../log/");
                    strcpy(option_out, &argv[i][10]);
                    strcat(out_path, option_out);
                    printf("option: %s \n", out_path);
                }
                else if(!strncmp("--errfile=", argv[i] , 10 ))
                {
                    strcpy(err_path, "../log/");
                    strcpy(option_err, &argv[i][10]);
                    strcat(err_path, option_err);
                    printf("option: %s \n", err_path);
                }
                else
                {
                    printf("Formato di comando errato. Eseguire scrivendo: \n");
                    printf("'--outfile=' + nome_file \nOppure\n");
                    printf("'-o ' + nome_file\n");
                    exit(0);
                }
            }
        }
        else
        {
            printf("Formato di comando errato. Eseguire scrivendo: \n");
            printf("'--outfile=' + nome_file \nOppure\n");
            printf("'-o ' + nome_file\n");
            exit(0);
        }
    }
}


// controllo se alla fine di un comando sono presenti spazi, tabulazioni o caratteri che non fanno parte del comando
void controllo_input(char* line, int len) 
{
        int j=len;
        int i=len;
        while( (line[j-2])<33)
        {
            if( ( (line[i-2]) < 33) ) //se la coda della stringa e' minore dei primi 33 caratteri della tabella ASCII allora li elimino
            {
                line[i-2] = line[i-1];
                line[i-1] = line[i];
            }
            i--;
            j--;
        }
}

/** Funzione che nel caso l'utente digiti:
 *  - exit : esce da my own terminal
 *  - altrimenti esegue il comando
 */ 
int processa_comando(char* cmd, int input, int first, int last, int* pipe_err_fd_read) //funzione che processa il comando da eseguire
{
    dividi(cmd); //parsing di cmd su "|"
    if (args[0] != NULL) 
    {
        if (strcmp(args[0], "exit") == 0) // se il comando e' "exit" esco da tutto
        {
            exit(0);
        }
        n+= 1;
        cmdAppend(args[0], first); //servira' per stampare sottocomandi
        return esegui(input, first, last, pipe_err_fd_read);
    }
    return 0;
}


//Funzione che divide l'insieme di comandi tra una '|' e l'altra in singoli comandi
void dividi(char* cmd) 
{
    cmd = spazio(cmd);
    char* next = strchr(cmd, ' ');        //Trovo il primo comando 
    int i = 0;                            //Posizione del singolo comando
    
    while(next != NULL)     
    {
        next[0] = '\0';                    //Termina la stringa con dentro il singolo comando 
        args[i] = cmd;
        ++i;
        cmd = spazio(next + 1);      //Tolgo tutti gli spazi fino a che non trovo il prossimo comando 
        next = strchr(cmd, ' ');
    }

    if (cmd[0] != '\0') 
    {
        args[i] = cmd;
        next = strchr(cmd, '\n');
        if(next!=NULL) //if che fa in modo di considerare anche i casi in cui tra comandi e pipe non ci siano spazi
        {
          next[0] = '\0';
        }
        ++i;
    }
    args[i] = NULL;
}


//Funzione per togliere gli spazi
char* spazio(char* cmd)  
{
    while (*cmd == ' ') 
    {
        ++cmd;
    }
    return cmd;
}


int esegui(int input, int first, int last, int* pipe_err_fd_read) //Funzione che gestisce l'esecuzione
{
    int pipe_t[2];   //Pipe iniziale
    int pipe_err_fd[2];
    char buf[4096];  //Buffer in cui ci sara' output del comando  
//    char error_buf[4096];  //Buffer utilizzato per la gestione degli errori
    pipe( pipe_t ); //inizializzazione pipe
    pipe(pipe_err_fd);
    *pipe_err_fd_read = pipe_err_fd[READ];
    
    pid = fork();

    int duperr_fd;      // Puntatore al STDERR_FILENO, che riprestinerò dopo
    FILE*  devnull_fd;  // File contenente gli errori
    FILE*  out;  //File contenente output

    devnull_fd = fopen(err_path, "a");  //Apro il file d'errore in modalità append
    duperr_fd= dup(STDERR_FILENO);

    out = fopen(out_path, "a");     //Apro il file out in modalità append

    if(dup2(pipe_err_fd[WRITE], STDERR_FILENO) < 0)  //Al posto di scrivere il messaggio di errore su STDERR_FILENO lo vado a scrivere sul file err 
    {           
        perror("dup");
    }

    if (pid == 0) 
    {
        if (first == 1 && last == 0 && input == 0) 
        {
            //Sono nel primo comando, scrivo nella pipe
            dup2( pipe_t[WRITE], STDOUT_FILENO );
        } 
        else if (first == 0 && last == 0 && input != 0) 
        {
            //Sono nel comando di mezzo, invece che leggere da STDIN leggo da input, che e' output del processo precedente
            dup2(input, STDIN_FILENO);
            dup2(pipe_t[WRITE], STDOUT_FILENO);
        } 
        else 
        {
            //Sono nell'ultimo comando, procedo come nel comando di mezzo leggendo dall'input corrispondente all'output precedente
            dup2( input, STDIN_FILENO );
            if (((strcmp(args[0],"nano") != 0) && (strcmp(args[0],"pico") != 0)) || (strcmp(args[1],"-h") == 0 && strcmp(args[1],"-?") == 0 && strcmp(args[1],"--help") == 0 && strcmp(args[1],"-V") == 0 && strcmp(args[1],"--version") == 0))
            {
                //Se non trovo un comando che crea un nuovo processo scrivo nella pipe invece che su STDOUT
                dup2(pipe_t[WRITE], STDOUT_FILENO);
            }
        }

        if (execvp( args[0], args) == -1)
        { //In caso di fallimento del figlio
            _exit(EXIT_FAILURE); 
        }
    }
    
    close(pipe_err_fd[WRITE]);
    
    dup2(duperr_fd, STDERR_FILENO);

    if (input != 0) //chiudo in lettura quando finisco di leggere da input
    {
        close(input);
    }

    close(pipe_t[WRITE]); //chiudo anche in scrittura perche' non ho piu' nulla da scrivere 
 
    int nbytes= read(pipe_t[READ], buf, sizeof(buf)); //Leggo quello che c'è dentro la pipe (output processo precedente)

    int newPipe[2]; //Creo nuova pipe perche leggendo cancello il contenuto
    pipe(newPipe);
    stampa_output(out, nbytes, buf); 

    /*if(stampa==1) //Nel caso in cui ci sia un ">" nella stringa di input
    {
      stampasufile(maggiore,nbytes,buf);
    }*/

    if(last == 1)
    {
        printf("%.*s", nbytes, buf);
    }

    write(newPipe[WRITE], buf, nbytes); //Scrivo nella nuova pipe l'output del processo precedente
    close(newPipe[WRITE]);
    close(pipe_t[READ]);

    if (last == 1) //Se e' l'ultimo comando non ho nulla da leggere, chiudo quindi la pipe
    {
        close(newPipe[READ]); 
    }
    
    fclose(out);
    fclose(devnull_fd);
    
    return newPipe[READ]; //ritorno il valore nella pipe in lettura 
}


//Funzione che gestisce la stampa dei sottocomandi
void cmdAppend(char* cmd, int first) 
{
    if(first!= 1)
    {
        commandToPrint[index1] = '|';
        index1++;
    }

    int startingIndex = index1;

    for (int i = 0; i < strlen(cmd) ; i++)
    {
        commandToPrint[startingIndex+i] = cmd[i];
        index1++;
    }

    commandToPrint[index1] = '\0';
}


//Funzione che gestisce la stampa sul file di output
void stampa_output(FILE* out, int nbytes, char* buf) 
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf(out, "\n------------------------------------------------------------------\n");
    fprintf(out,"COMANDO: \t\t\n%s", line);
    fprintf(out, "SUBCOMMANDO:\n%s\n", commandToPrint);
    fprintf(out,"DATA: \t\t\t%s",asctime(timeinfo));
    fprintf(out, "OUTPUT:\n\t%.*s   \n", nbytes, buf);

}


//Funzione che gestisce la stampa sul file di errore
void stampa_errore(FILE* err, int nbytes, char* buf_err) 
{
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf(err, "\n------------------------------------------------------------------\n");
    fprintf(err,"COMANDO: \t\t%s", line);
    fprintf(err, "SUBCOMMANDO:\n%s\n", commandToPrint);
    fprintf(err,"DATA: \t\t\t%s",asctime(timeinfo));
    fprintf(err, "ERROR:\n\t%.*s   \n", nbytes, buf_err);
}


//Funzione che reindirizza la stampa ad un file specifico 
/*void stampasufile(char* file,int nbytes, char* buf) 
{
  FILE* out;
  out= fopen(file, "w+" );
  fprintf(out, "%.*s", nbytes, buf);
}*/

void printHelp(){
    printf("Usage: ./shell [OPTIONS]\n");
    printf("Option\t\tlong option\t\tMeaning\n");
    printf("-e <file>\t--errfile=<file>\tSet name of error log file\n");
    printf("-h\t\t--help\t\t\tShow usage help\n");
    printf("-o <file>\t--outfile=<file>\tSet name of log file\n");
}

