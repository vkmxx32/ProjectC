#include "sudoku.h"


// Partie affichage

int afficher_tableau(int t[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d ",t[i][j]);
        }
        printf("\n");
    }
    return 1;
}

int afficher_sudoku(Sudoku * psudoku){
	afficher_tableau(psudoku->t);
	if (psudoku->m){
		printf("Methode: bactracking\n");
	}else{
		printf("Methode: brute-force\n");
	}
    printf("\n");
	return 1;
}

// Partie manipulation fichier
int build_fname(char buffer[LENGTHMAX],int numero, int mode){
	memset(buffer, 0, LENGTHMAX);
	if (mode){
    	strcat(buffer,"solutions/solution");

	}
    else{
    	strcat(buffer,"sudoku/sudoku");

    }

    char fname1[] = ".txt";
    char numero_str[LENGTHMAX];

    sprintf(numero_str,"%d", numero); // copy integer in buffer

    strcat(buffer,numero_str);
    strcat(buffer,fname1);

    return 1;
}

int test_fichier(int numero){
    char fname[LENGTHMAX];
    build_fname(fname, numero,0);
    FILE * fp;
    fp = fopen(fname, "r");

    if (fp == NULL)
        return 0;

    return 1;
} 

int extraire_fichier(int numero,Sudoku * psudoku){
	char fname[LENGTHMAX];
	build_fname(fname, numero,0);

    FILE * fp;
    fp = fopen(fname, "r");


    if(fp == NULL){
        //printf("Fichier inexistant: %s\n",fname);
        psudoku->r = -1;
        return 0;
    }

    char * line = NULL; //String alloue dynamiquement par getline()
    size_t len = 0; //Longueur de la ligne
    int c_g = 0;
    int l_g = 0;
    size_t read; //signed size_t taille bloc memoire

    for (int i = 0;i<14;i++){
        read = getline(&line, &len, fp);

        if (read==20){
            c_g = 0;

            for (int j=0;j<20;j++){

               if ( isdigit(line[j]) ){
                    psudoku->t[l_g][c_g]= line[j]- '0';
                    c_g++;
               }

               if (line[j]=='_'){
                    psudoku->t[l_g][c_g]= 0;
                    c_g++;
               }

            }
            l_g++;

        }

    }


    char brute[] ="METHOD: BRUTE-FORCE";
    int n = strcmp(brute,line);

    psudoku->numero = numero;
    psudoku->m =n>0;
    psudoku->r =0;

    free(line);
    fclose(fp);
    return 1;
}

int ecrire_sol(Sudoku * psudoku){
	char fname[LENGTHMAX];

	build_fname(fname,psudoku->numero,1);
    FILE *fp;

    fp = fopen(fname, "w");
    if (fp == NULL){
        printf("Fichier inexistant: %s\n",fname);
        return 0;
    }

    for (int i =0;i<9;i++){
        for (int j =0;j<9;j++){
            fprintf(fp,"%d",psudoku->t[i][j]);
            fprintf(fp," ");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 1;
}

//Partie sudoku
int test_lig(int t[9][9],int lig, int num){
    for (int i =0;i<9;i++)
        if (t[lig][i]==num)
            return 0;
    return 1;
}

int test_col(int t[9][9],int col, int num){
    for (int i =0;i<9;i++)
        if (t[i][col]==num)
            return 0;
    return 1;
}

int test_box(int t[9][9],int lig,int col,int num){
    int debutl_box = lig - lig % 3;
    int debutc_box = col - col % 3;
    for (int i =0;i<3;i++)
            for (int j =0;j<3;j++)
                if (t[i+debutl_box][j+debutc_box]==num)
                    return 0;
    return 1;
}

int possible(int t[9][9],int lig, int col, int num){
    int n = (test_box(t,lig,col,num) & test_lig(t,lig,num) & test_col(t,col,num));
    return n;
}

int backtrack_aux(int t[9][9],int lig,int col){
    if (lig == 8 & col == 9)
        return 1;

    if (col==9){
        lig++;
        col = 0;
    }

    if (t[lig][col]>0){
        return backtrack_aux(t,lig,col+1);
    }

    for (int num =1;num<=9;num++){
        int possible_bool = possible(t,lig,col,num);

            if (possible_bool){
                t[lig][col]=num; //on affecte une valeur
                if (backtrack_aux(t,lig,col+1))
                    return 1;
            }
            t[lig][col]=0; //on enleve une valeur
    }
    return 0;
}

int sudoku_nzeros(int n,int t[9][9]){
    int u = 0;
    while(u!=n){
        int i = rand() % 9 + 1;
        int j = rand() % 9 + 1;
        if (t[i][j]>0){
            t[i][j]=0;
            u++;
        }
    }
    return 1;
}

int valider(int t[9][9]){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            int num = t[i][j];
            t[i][j]=0;
            if (!(possible(t,i,j,num))) //on teste si chaque coefficient est legal
                return 0;
            t[i][j]= num;

        }
    }
        return 1;
}

int copier_sudoku(int src[9][9],int dest[9][9]){
    for (int i =0;i<9;i++)
        for (int j =0;j<9;j++)
            dest[i][j]= src[i][j];
    return 1;
}

int tester_solution(char * buffer,int src[9][9]){
    int t[9][9];
    copier_sudoku(src,t);
    int nb = 0;
    for (int i =0;i<9;i++)
        for (int j =0;j<9;j++)
            if (t[i][j]==0){
                t[i][j]=buffer[nb]-'0';
                    nb++;
                }

    int valide = valider(t);
    return valide;
}

int remplir_solution(char * buffer,int t[9][9]){
    int nb = 0;
    for (int i =0;i<9;i++)
        for (int j =0;j<9;j++)
            if (t[i][j]==0){
                t[i][j]=buffer[nb]-'0';
                nb++;
            }
    return 1;
}

int nb_zeros(int t[9][9]){
    int n = 0;
    for (int i =0;i<9;i++)
        for (int j =0;j<9;j++)
            if (t[i][j]==0)
                n++;
    return n;
}

int brute_aux(int t[9][9],char * buffer,int position, int nbz){

    if (position==nbz){
        int bool_sol =(tester_solution(buffer,t));
        if(bool_sol){
            remplir_solution(buffer,t);
            return 1;
        }
        return 0;
    }

    for (int num = 1;num<=9;num++){
        buffer[position] = num + '0' ;
        if (brute_aux(t,buffer,position+1,nbz))
            return 1;
    }
    return 0;
}

void * resoudrev(void * preceived){
    Sudoku * psudoku = (Sudoku *) preceived;
    if (!(psudoku->m)){
    	int nbz = nb_zeros(psudoku->t);
        if (nbz>=REDUIT){
            nbz = REDUIT;//
    		backtrack_aux(psudoku->t,0,0);
            sudoku_nzeros(nbz,psudoku->t);
        }
		char buf[nbz];
		brute_aux(psudoku->t,buf,0,nbz);
    }else {
    	backtrack_aux(psudoku->t,0,0);
    }
    pthread_mutex_lock(psudoku->pmutex);
    psudoku->r = 1;
    pthread_mutex_unlock(psudoku->pmutex);
    return NULL;
}

int print_line(){
	for (int i =0;i<70;i++)
		printf("─");
	printf("\n");
    return 0;
}

int standalone(){
	print_line();
	int numero =0;
    printf("Veuillez rentrer le numero du sudoku (1-21) dans la BDD:  \n");
    scanf("%d",&numero);
    int methode;
    printf("Choix de la methode de resolution : (0)Brute force (1)Backtracking\n");
    scanf("%d",&methode);


    Sudoku * psudoku = malloc(sizeof(Sudoku));
    extraire_fichier(numero,psudoku);
    psudoku->m = methode;
    if (psudoku->r==-1){
    	printf("Numero de fichier invalide\n");
    	return 0;
    }
    printf("Voici le sudoku numero %d :\n",numero);
    afficher_sudoku(psudoku);
    int nbz = nb_zeros(psudoku->t);
    printf("Nombre de zeros: %d\n",nbz);
    if (!(psudoku->m))
		printf("Pour limiter le temps de calcul, on a reduit le puzzle a %d chiffres manquants\n",REDUIT);


    resoudrev((void *)psudoku);
    printf("Sudoku resolu:\n");
    afficher_tableau(psudoku->t);
    ecrire_sol(psudoku);

    free(psudoku);
    print_line();
    return 1;
}

int multithread(){

    printf("Donner le nombre de sudokus a resoudre:");
    int nb;
    scanf("%d",&nb);


    pthread_mutex_t mutex;
    pthread_mutex_init( &mutex, NULL);
    pthread_t * thread = malloc(sizeof(pthread_t)*nb);
    int * numeros = malloc(nb* sizeof(int) );

    Sudoku ** psudoku_array = malloc(nb * sizeof(Sudoku*));
    for (int i=0;i<nb;i++){
        psudoku_array[i] = malloc( sizeof(Sudoku) );
        psudoku_array[i]->pmutex= &mutex;

    }

    for (int i=0;i<nb;i++){
        printf("Veuillez rentrer le numero du sudoku (1-21) dans la BDD:  \n");
        scanf("%d",numeros+i);
        if (!(test_fichier(numeros[i]))){
            printf("Fichier non existant \n");
            printf("Veuillez rentrer un autre numero\n");
            i--;
        }else{
            printf("Fichier existant \n");

        }
    }


    for (int i=0;i<nb;i++){
        printf("%d ",numeros[i]);
        extraire_fichier(numeros[i],psudoku_array[i]);
        if (pthread_create ( &thread[i], NULL, resoudrev, (void*) psudoku_array[i]) != 0){
                   printf("Impossible de creer un thread\n");
                   exit(EXIT_FAILURE);
        }
    }

 
    int thread_resolu = 0;
    while (thread_resolu<nb){
        for (int i=0;i<nb;i++){
            pthread_mutex_lock(&mutex);
            if (psudoku_array[i]->r == 1){
                ecrire_sol(psudoku_array[i]);
                psudoku_array[i]->r =0; 
                printf("%d\n",psudoku_array[i]->m);
                printf("%d\n",psudoku_array[i]->numero);
                printf("Ecriture solution puzzle %d:",psudoku_array[i]->numero);
                if (psudoku_array[i]->m){
                	printf(" bactracking\n");
                } else{
                	printf(" brute force\n");
                }

                thread_resolu++;
            }
            pthread_mutex_unlock(&mutex);
        }
    }


    for (int i=0;i<nb;i++){
        pthread_join(thread[i],NULL);
    }
    for (int i=0;i<nb;i++)
        free(psudoku_array[i]);
    free(thread);  
    free(psudoku_array);
    free(numeros);

    return 1;
}


