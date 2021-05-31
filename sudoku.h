#ifndef HEADER_INCLUDED

#define HEADER_INCLUDED
#define N 9
#define LENGTHMAX 255
#define REDUIT 7

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

//$$$$$$$$$$$ajouts pour TP socket UDP
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<strings.h>
#include <unistd.h>
#define LEPORT 5677
//struct sockaddr_in *padin; //pointeur adresse internet locale
struct sockaddr_in *p_exp; //pointeur adresse internet de l'expediteur (depuis UDP recu)
/* RAPPEL de sur cette structure
struct sockaddr_in
{
    short      sin_family;
    unsigned short   sin_port;
    struct   in_addr   sin_addr;
    char   sin_zero[8];
};
*/
//$$$$$$$$$$FIN d'ajouts TP socket UDP




typedef struct Sudoku {
  int m;
  int t[9][9];
  int r;
  int numero;
  pthread_mutex_t * pmutex;
} Sudoku;

//Affichage
int afficher_sudoku(Sudoku * sudoku);
int afficher_tableau(int t[N][N]);

// Manipulation fichier
int build_fname(char buffer[LENGTHMAX],int numero, int mode);
int test_fichier(int numero);
int extraire_fichier(int numero,Sudoku * sudoku);
int ecrire_sol(Sudoku * psudoku);

//Test sudoku
int test_lig(int t[9][9],int lig, int num);
int test_col(int t[9][9],int col, int num);
int test_box(int t[9][9],int lig,int col,int num);
int possible(int t[9][9],int lig, int col, int num);
int valider(int t[9][9]);

//Fonctions utilitaires pour brute force
int sudoku_nzeros(int n,int t[9][9]);
int copier_sudoku(int src[9][9],int dest[9][9]);
int tester_solution(char * buffer,int src[9][9]);
int remplir_solution(char * buffer,int t[9][9]);
int nb_zeros(int t[9][9]);

//Esthetique
int print_line();

//Algorithme sudoku
int backtrack_aux(int t[9][9],int lig,int col);
int brute_aux(int t[9][9],char * buffer,int position, int nbz);
void * resoudrev(void * preceived);//


int standalone();
int multithread();

#endif // HEADER_INCLUDED
