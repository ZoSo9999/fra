#include "random.h"
#include <time.h>
#include <stdio.h>
#include <mcheck.h>


#define MY_EMPTY 0
#define DIM 2
#define UNMEZZO 0.5




void shuffle(int *V, int N){
  for(int i=0; i<N; i++)
    V[i]=i;
  
  for(int i=0; i<N; i++){
    int r = get_intero(i, N-1);
    int aux =  V[i];
    V[i] = V[r];
    V[r] = aux;
  }
}



int periodic(int a, int b){

  int module = a % b;
  if(module < 0)
    module += b;

  return module;
}


void spostamento(int *spazio, int *Pos, int *realPos, int *name, int N, int L){
  
  shuffle(name, N);

#ifdef DEBUG
  for(int i=0; i<N; i++){
    printf("%d\n", name[i]+1);
  }

  for(int i=0; i<N; i++)
    printf("%d  %d\n", Pos[i*DIM], Pos[i*DIM +1]);
#endif
  
  
  for(int i=0; i<N; i++){


    
    int oldPlace = Pos[DIM * name[i]] + L * Pos[DIM * name[i] + 1];

#ifdef DEBUG    
    printf("oldPlace=%d\n", oldPlace);
#endif

    
    int indice = get_intero (0, 2 * DIM - 1);
    int spostamento = (indice % 2) ? 1 : -1 ;                                //determino il verso
    indice *= UNMEZZO;                                                  //determino la direzione  (questo è già pronto per più dimensioni)
 
    indice += DIM * name[i];
    Pos[ indice ] +=  spostamento;

    

    Pos[ indice ] = periodic(Pos[ indice ], L);


    
    int newPlace = Pos[DIM * name[i]] + L * Pos[DIM * name[i] + 1];

#ifdef DEBUG    
    printf("newPlace=%d\n", newPlace);
#endif
    

    if(spazio[ newPlace ] == MY_EMPTY){
      
      spazio[ newPlace ] = name[i]+1;
      spazio[ oldPlace ] = MY_EMPTY;

      realPos[ indice ] += spostamento;
      
    }
    
    else{
      Pos [ indice ] -= spostamento;
      Pos[ indice ] = periodic(Pos[ indice ], L);
    }


#ifdef DEBUG    
        for(int i=0; i<L*L; i++){
      if((i%(L)) == 0)
	printf("\n");
      printf("%d ", spazio[i]);
    }
    printf("\n\n");
#endif
    
  }
  

}





double distance2(int *A, int *B, int i){

  double d2 = 0;
  
  for(int j=0; j<DIM; j++){
    int index = i * DIM + j;
    d2 += (A[ index ] - B[ index ]) * (A[ index ] - B[ index ]);
  }

  return d2;
}




double measure(int *Pos, int *zeroPos, int N){
  
  double deltaR2 = 0;
  
  for(int i=0; i<N; i++)
    deltaR2 += distance2(Pos, zeroPos, i);

  return deltaR2 / N;  
}




void DA_i_A_xy(int *V, int i, int L){V[1] = i / L; V[0] = i % L;}



void initializing(int *SPACE, int *zeroPos, int *Pos, int *realPos, int *name, int N, int L){

  int V = L*L;
  
  

    for(int i=0; i<V; i++)
      SPACE[i] = MY_EMPTY;

    
    shuffle(name, V);
    
    
    for(int i=0; i<N; i++){
      
      SPACE[name[i]] = i+1;

      
      
      int coordinate[2];
      DA_i_A_xy(coordinate, name[i], L);

      zeroPos[i * DIM] = coordinate[0];                            //qua fare ciclo for sulle dimensioni per generalizzare
      zeroPos[i * DIM + 1] = coordinate[1];
    
      Pos[i * DIM] = coordinate[0];
      Pos[i * DIM + 1] = coordinate[1];

      realPos[i * DIM] = coordinate[0];
      realPos[i * DIM + 1] = coordinate[1];

#ifdef DEBUG
      printf("%d  %d\n", Pos[i*DIM], Pos[i*DIM +1]);
#endif
    }

}



void plot( FILE *gnuplotPipe, int *Pos, int N, int L){
  
  fprintf(gnuplotPipe, "plot [0:%d][0:%d] '-' u 1:2 w p pt 7 ps 2\n", L, L);

  for(int i=0; i<N; i++)
    fprintf(gnuplotPipe, "%16d %16d\n", Pos[i * DIM], Pos[i * DIM + 1]);

  fprintf(gnuplotPipe, "e\n");
  fflush(gnuplotPipe);
}





/***********************MAIN***************************/

int main(int argc, char *argv[]){

  clock_t begin = clock();

  
  if(argc < 4){
    printf("Wrong imput -> RHO(volumetric density) LENGTH NUM_STEPS STORIES\n");
    exit(-1);
  }
  
#ifdef PLOT  
  FILE *gnuplotPipe = popen("gnuplot -p", "w");
#endif

  FILE *pf = fopen("clock.dat", "a");
  
  seed = time(NULL);
  set_Ecuyer2();
  
  

  double rho = atof(argv[1]);
  int L = atoi(argv[2]);
  int STEPS = atoi(argv[3]);
  int STORIES = atoi(argv[4]);
    
  int V = L*L;
  int N =  rho * V;
  double A = 0.;

  int *SPACE = (int *) calloc(V, sizeof(int));
  int *Pos = (int *) calloc(N * DIM, sizeof(int));
  int *zeroPos = (int *) calloc(N * DIM, sizeof(int));
  int *realPos = (int *) calloc(N * DIM, sizeof(int));
  int *aux = (int *) calloc(V, sizeof(int));
  int *name = (int *) calloc(N, sizeof(int));

  
  if(SPACE == NULL || zeroPos == NULL || Pos == NULL || realPos == NULL || name == NULL || aux == NULL){
    printf("Something went wrong during the allocation! :(\n");
    exit(-1);
  }

  
#ifdef LEGENDA
  printf("#RETICULAR GAS :    GRIID SIZE %5d DENSITY %5g TIME STEPS %5d\n", L, rho, STEPS);
  printf("#              :    PARTICOLS NUMBER %5d VOLUME %5d STORIES %5d\n", N, V, STORIES);
#endif
  

  for(int j=0; j<STORIES; j++){                               /*************STORIE*************/

        
    initializing(SPACE, zeroPos, Pos, realPos, aux, N, L);      /*********INIZIALLIZZO*********/


#ifdef DEBUG
    for(int i=0; i<V; i++){
      if((i%(L)) == 0)
	printf("\n");
      printf("%d ", SPACE[i]);
    }
    printf("\n\n");
#endif  
    
    
    for(int t=0; t<STEPS; t++){                                  /***********STEP TEMPORALE***********/

       
      
      spostamento(SPACE, Pos, realPos, name, N, L);                    /*************MOVIMENTO*************/

     

#ifdef DEBUG
      for(int i=0; i<V; i++){
	if((i%(L)) == 0)
	  printf("\n");
	printf("%d ", SPACE[i]);
	
      }

      printf("\n\n");
#endif

      
#ifdef PLOT      
      if(DIM < 3 && STORIES == 1)
	plot(gnuplotPipe, Pos, N, L);
#endif
      
    }

    

    double deltaR2 = measure(realPos, zeroPos, N);
    A += deltaR2;


  }
  
  A /= (STORIES * 2 * DIM * STEPS);                              //per fare t diversi faccio script di bash in cui vario t


  printf("%16d %16g %16g\n", STEPS, rho, A);  

  
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  fprintf(pf, "%g\n", time_spent);


  fclose(pf);   
  free(SPACE);
  free(Pos);
  free(zeroPos);
  free(realPos);
  free(aux);
  free(name);

  return 0;
}



//Posso aggiungere delle barre d'errore sui D(t)
