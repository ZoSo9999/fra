#include "random.h"
#include "statistic.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>




#define UNMEZZO 0.5                      //per non fare tutte le volte la divisione per 2    POSSO FARE ANCHE 1/STEPS 


/****************************************/


int main(int argc, char *argv[]){

  if(argc < 3){
    printf("Wrong imput! --> DIMESION STEPS STORIES\n");
    exit(-1);
  }

  

  seed = time(NULL);
  set_Ecuyer2();
  
  int DIM = atoi(argv[1]);
  int STEPS = atoi(argv[2]);
  int STORIE = atoi(argv[3]);
  int *X = (int *) calloc (STORIE*DIM, sizeof(int));            //ARRAY CHE CONTIENE LE COORDINATE DI TUTTE LE PARTICELLE AD UN CERTO TEMPO t


  if(X==NULL){
    printf("Something went wrong during the allocation!  :(\n");
    exit(-1);
  }

  

#ifdef MSD
  double *MEAN  = (double *)  calloc (DIM, sizeof(double));         //ARRAY DELLE MEDIE
  double *MEAN2  = (double *)  calloc (DIM, sizeof(double));        //ARRAY DELLE MEDIE^2
  
  if(MEAN == NULL || MEAN2 == NULL){
    printf("Something went wrong during the allocation!  :(\n");
    exit(-1);
  } 
#endif
  
  
  
  
#ifdef HISTO
  //int *HIST = (int *)  calloc ((2*STEPS + 1) * (2 * STEPS + 1) , sizeof(int));                                 //MATRICE ISTOGRAMMA altro modo di fare istogramma

  
 double *x1 = (double*) calloc(STORIE, sizeof(double));            //ARRAY PER L'ISTOGRAMMA DI UNA VARIABILE E PER LE DISTANZE
 double *x2 = (double*) calloc(STORIE, sizeof(double));            //ARRAY PER L'ISTOGRAMMA DI UNA VARIABILE E PER LE DISTANZE
 double *DISTANCE = (double*) calloc(STORIE, sizeof(double));
 
 
 
 if(/*HIST == NULL ||*/ x1 == NULL || x2 == NULL || DISTANCE == NULL){
   printf("Something went wrong during the allocation!  :(\n");
   exit(-1);
 }
#endif
 
  







#ifdef PLOT  

  for(int j=0; j<STORIE; j++){
    printf("T %d S %d  ", 0, j+1);                      //STAMPA INIZIALE t=0 
    for(int k=0; k<DIM; k++){
      printf("%d ", 0);
    }
    printf("\n");
  }

  printf("\n\n");
#endif




  
  
#ifdef MSD
  
  printf("#A %16s", "t");                                 //STAMPO LA LEGENDA
  for(int k=0; k<DIM; k++)
    printf("%16s%d%s", "<x_", k+1, ">");
  printf("%16s\n", "MSD");
  
  printf("A %16d", 0);                                    //FACCIO LA PRIMA STAMPA t=0
  for(int k=0; k<DIM; k++)
    printf("%16d", 0);
  printf("%16d\n", 0);
#endif    



  

/***************************SCORRO GLI STEPS *********************************/
  
  for(int t=0; t<STEPS; t++){                            

#ifdef MSD    
    double msd = 0;                                      //MID SQUARE DISPLACEMENT LO INIZIALIZZO A 0
#endif 

/****************************SCORRO LE STORIE ********************************/
    
    for(int j=0; j<STORIE; j++){                         


      
      
      int dir = get_intero (2 * DIM - 1);
      int spostamento = (dir % 2) ? 1 : -1 ;            //capisco se va a destra o sinistra
      dir = (int) dir * UNMEZZO;                        //determino la componente da incrementare

      X[dir + j * DIM] += spostamento;                        //faccio lo spostamento


      
#ifdef MSD   
      MEAN[dir] += X[dir + j * DIM];                          //faccio le somme e le somme quadre
      MEAN2[dir] += X[dir + j * DIM] * X[dir + j * DIM];
#endif


      

      

      
#ifdef PLOT
      printf("T %d S %d  ", t+1, j+1);                      //STAMPA DELLE POSIIZONI AL TEMPO T E ALLA STORIA S
      for(int k=0; k<DIM; k++){
	printf("%d ", X[k + j * DIM]);
      }
      printf("\n");
#endif

      
      
    }

    
#ifdef PLOT
    printf("\n\n");
#endif
        

    

#ifdef MSD         
    printf("A %16d", t + 1);                            //STAMPA DELLA STATISTICA
    for(int k=0; k<DIM; k++){ 
      MEAN[k] /= STORIE;
      MEAN2[k] /= STORIE;
      msd += MEAN2[k];   
      printf("%16g", MEAN[k]);
      MEAN[k]=0;
      MEAN2[k]=0;
    }   
    printf("%16g\n", msd);  
    msd = 0;
#endif


  }



  
  
  
#ifdef HISTO 
  //int L = 2 * STEPS + 1;
  int Xbins;
  int Ybins;
  double Xstep;
  double Ystep;
  double M;
  int *HIST;


  for(int j=0; j<STORIE; j++){
    x1[j] = X[j * DIM];
    x2[j] = X[j * DIM + 1];
  }
  
  stat statY = get_statistic(x2, STORIE);
  print_statistic( statY );                                      //FACCIO L'ISTOGRAMMA PER UN'UNICA VARIABILE
  
  stat statX = get_statistic(x1, STORIE);
  print_statistic( statX );                                      //FACCIO L'ISTOGRAMMA PER UN'UNICA VARIABILE


  
  if(DIM == 2){
    Xbins = statX.bins;                                               //SCELTA DI QUANTI BINS
    Ybins = statY.bins;
    
    Xstep = (statX.max - statX.min) / (double) Xbins;
    Ystep = (statY.max - statY.min) / (double) Ybins;
    M = 1. / ((double) STORIE * Xstep * Ystep);              //COSTANTE DI NORMALIZZAZIONE

  
    HIST = (int *)  calloc ( (Xbins * Ybins) , sizeof(int));         //MATRICE ISTOGRAMMA
    if(HIST == NULL){
      printf("Something went wrong during the allocation!  :(\n");
      exit(-1);
    }
  }


  

for(int j=0; j<STORIE; j++){


  if(DIM == 2){                                           //SE LA DIMENSIONE È 2 FACCIO L'ISTOGRAMMA 3D
    int binx = (x1[j] - statX.min) / Xstep;
    int biny = (x2[j] - statY.min) / Ystep;
    

    int pos = binx + Xbins * biny;
    HIST[pos] ++;
  }
    
  double sum = 0;
  for(int i=0; i<DIM; i++)
    sum += X[j * DIM + i] * X[j * DIM + i];
  DISTANCE[j] = pow(sum, 0.5);               //riempio l'array
    
 }



  /* for(int j=0; j<STORIE; j++){                  //ALTRO METODO DI FARE L'ISTOGRAMMA MENO OTTIMALE

    int x = X[j * DIM];
    int y = X[j * DIM + 1];
    
    
    if (DIM==2){                                    //SE LA DIMENSIONE È 2 FACCIO L'ISTOGRAMMA 3D

      x = x + STEPS;
      y = STEPS - y;
      
      int pos = x + L * y;
      HIST [pos] ++;
    }
    
    

    double sum = 0;
    for(int i=0; i<DIM; i++)
      sum += X[j * DIM + i] * X[j * DIM + i];
    DISTANCE[j] = pow(sum, 0.5);               //riempio l'array
    }*/
  
  
  if(DIM == 2){

    for(int i=0; i<Ybins; i++){
      for(int j=0; j<Xbins; j++){

	double x = (statX.min + Xstep/2)+ j*Xstep;
	double y = (statY.min + Ystep/2)+ i*Ystep;
	double P = (double)  HIST[j + i * Xbins] * M;

	if(P != 0)
	  printf("%16g %16g %16g\n", x, y, P);
      }
    }
  }

  printf("\n\n");
  
  
    
  
    
  print_statistic( get_statistic( DISTANCE, STORIE));

  

  
  if(DIM == 2)
    free(HIST);
  free(x1);
  free(x2);
#endif



  
#ifdef MSD
  free(MEAN);
  free(MEAN2);
#endif


  free(X);
    
  return 0;
}





/*******************************************************************/
/*
Prossimo passo implementare i camminatori speciali.  
*/
