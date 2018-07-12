#ifndef SHELL_H_
#define SHELL_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define READ  0
#define WRITE 1

char* args[512];                        //Array che contiente gli argomenti del comando scritto dall'utente
pid_t pid;			
char line[1024];                        //Comando inserito da utente */
int n;                                  // Numero di chiamate alla funzione 'comando' 
char commandToPrint[1024];              // Comando stampato in cmdAppend
int index1;                             //indice dell'ultimo carattere di commandToPrint 
char out_path[256];
char err_path[256];
int status;


void controllo_argomenti(int argc, char **argv);
void controllo_input(char* line, int len);
int processa_comando(char* cmd, int input, int first, int last, int* pipe_err_fd_read);
void dividi(char* cmd);
char* spazio(char* s);
int esegui(int input, int first, int last, int* pipe_err_fd_read);
void cmdAppend(char* cmd, int first);
void wait_pr();
//void stampasufile(char* file,int nbytes, char* buf);
void stampa_output(FILE* out, int nbytes, char* buf);
void stampa_errore(FILE* err, int nbytes, char* buf_err);
void printHelp();



#endif
