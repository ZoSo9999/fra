#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void riempi_elenco(double*, int);
void stampa_numeri(double*, int);//ho messo puntatore **************

int main(int argc, char** argv) {

    unsigned int seed;
    int Nran;
    double* numeri;

    if (argc!=3) {
	fprintf(stderr,"%s <seed> <quanti numeri>\n",argv[0]);
	exit(EXIT_FAILURE);
    }
    seed=atoi(argv[1]); // atoi() trasforma una stringa nel valore intero rappresentato
    Nran=atoi(argv[2]);

    srand(seed);

    numeri = (double*)calloc(Nran, sizeof(double));//ho messo double ****************
    if (numeri == NULL) {
      fprintf(stderr,"Errore di allocazione.\n");
      exit(EXIT_FAILURE);
    }
    riempi_elenco(numeri,Nran);

    stampa_numeri(numeri,Nran);
    free(numeri);

    return EXIT_SUCCESS;
}

void riempi_elenco(double* punt, int quanti) {
  int i;
  double r;
  double f;
  for (i=0;i<quanti;i++) {
    //r=1.-rand()/(RAND_MAX+1);
    r= (double)rand()/(RAND_MAX);//prima era la riga sopra *****************
    f= (double)rand()/(RAND_MAX);//prima c'era un +1 *******************
    punt[i]=sqrt(-2.0*log(r))*cos(2.*M_PI*f);//ho messo il 2* invece di /2 ***********
  }
}

void stampa_numeri(double* punt, int quanti) {//ho messo puntatore **************
  int i;
  double p;
  double mean=0;
  double mean2=0;
  for(i=0;i<quanti;i++){// ho messo la parentesi ************
    p = punt[i];
    mean+=p;
    mean2+=p*p;
    fprintf(stdout,"%.14e\n",p);
  }// ho messo la parentesi ************

  mean/=quanti;
  mean2 /= quanti;
  printf("\n\n");
  printf("#mean=%g   var=%g\n", mean, mean2-(mean*mean));
}
