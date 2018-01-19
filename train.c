// creé par anis tairi
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>


/* Les stations */
#define A 0
#define B 1
#define C 2
#define D 3




/* les threads */
void* train_de_A_vers_C(void* arg);
void* train_de_A_vers_D(void* arg);

pthread_mutex_t mut_AB;
pthread_mutex_t mut_BC;
pthread_mutex_t mut_CD;
//pthread_mutex_t mut_DB;

/* Gestion des chemins A B C D */
void utiliser_le_segment(int ville_depart, int ville_arrivee)
{
	if  ( (ville_depart == A) && (ville_arrivee == B) )
		pthread_mutex_lock(&mut_AB);
	else
		if  ( (ville_depart == A) && (ville_arrivee == C) )
			pthread_mutex_lock(&mut_AB);
			pthread_mutex_lock(&mut_BC);

		else if  ( (ville_depart == A) && (ville_arrivee == D) )
			pthread_mutex_lock(&mut_AB);
			pthread_mutex_lock(&mut_BC);
			pthread_mutex_lock(&mut_CD);
}


void liberer_le_segment(int ville_depart, int ville_arrivee)
{
	if  ( (ville_depart == A) && (ville_arrivee == B) )
		pthread_mutex_unlock(&mut_AB);
	else
		if  ( (ville_depart == B) && (ville_arrivee == C) )
			pthread_mutex_unlock(&mut_BC);
		else
			pthread_mutex_unlock(&mut_DB);
}


int main(int argc, char* argv[])
{
	int i;
	pthread_t id;

	/* Creer autant de trains que 
		necessaire */

	pthread_mutex_init(&mut_AB,0);
	pthread_mutex_init(&mut_BC,0);
	pthread_mutex_init(&mut_DB,0);
	
	for(i =0; i<3; i++)
		{
		pthread_create(&id, NULL, train_de_A_vers_C, NULL);
		pthread_create(&id, NULL, train_de_A_vers_D, NULL);
		}

	pthread_exit(NULL);
}


void* train_de_A_vers_C(void* arg)
{
	utiliser_le_segment(A, B);
	printf("Train %d : utilise raille AB \n",pthread_self());
	attendre(6);
	liberer_le_segment(A, B);

	utiliser_le_segment(B, C);
	printf("Train %d : utilise raille BC \n",pthread_self());
	attendre(6);
	liberer_le_segment(B, C);
        
        utiliser_le_segment(C, B);
        printf("Train %d : utilise raille CB \n",pthread_self());
	attendre(6);
        liberer_le_segment(C, B);
   
        utiliser_le_segment(B, A);
        printf("Train %d : utilise raille BA \n",pthread_self());
	attendre(6);
        liberer_le_segment(B, A);

	pthread_exit(NULL);
}

void* train_de_A_vers_D(void* arg)
{
	utiliser_le_segment(A, B);
	printf("Train %d : utilise raille AB \n",&id);
	attendre(6);
	liberer_le_segment(A, B);

	utiliser_le_segment(C, D);
	printf("Train %d : utilise raille CD \n",pthread_self());
	attendre(5);
	liberer_le_segment(C, D);

	utiliser_le_segment(D, C);
	printf("Train %d : utilise raille DC \n",pthread_self());
	attendre(5);
	liberer_le_segment(D, C);
	

	utiliser_le_segment(C, B);
	printf("Train %d : utilise raille DC \n",pthread_self());
	attendre(5);
	liberer_le_segment(C, B);

	utiliser_le_segment(B, A);
	printf("Train %d : utilise  BA \n",pthread_self());
	attendre(5);
	liberer_le_segment(B, A);

	pthread_exit(NULL);
}





int tirage_aleatoire(double max)
{
        int j=(int) (max*rand()/(RAND_MAX+1.0));
        if(j<1)
                j=1;
        return j;
}



void attendre(double max)
{
        struct timespec delai;

        delai.tv_sec=tirage_aleatoire(max);
        delai.tv_nsec=0;
        nanosleep(&delai,NULL);
}


