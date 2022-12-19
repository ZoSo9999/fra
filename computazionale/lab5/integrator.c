#include<stdlib.h>
#include<stdio.h>
#include "oscillator.h"

#define COMANDI 2

int main (int argc, char *argv []){

  parameters param = {2 * M_PI, 0.5, 1.15, 2./3.};

  #ifdef POINCARE
  double Fperiod = 2 * M_PI / param.Fomega;
  double t  = Fperiod * 3100.;
  double dt  = Fperiod / 1000.;
  int tsteps = rint(t/dt);
  double term = Fperiod * 100.;
  double phase [DIM] = {M_PI /2., 0.};
  #endif

  #ifdef SIMULATION
  double t  = 100;
  double dt  = 0.001;
  int tsteps = rint(t/dt);
  double phase [DIM] = {M_PI /2., 0.};
  #endif

  #ifdef ATTRAZIONE

  printf("#Ciao.\n");
  
  double t = 100;
  double dt = 0.01;
  int tsteps = rint(t/dt);

  if(argc<2){
    printf("Something went wrong...");
    exit(-1);
  }

  double phase [DIM]= {atof(argv[1]), atof(argv[2])};

  double x0 = phase[0];
  double v0 = phase[1];
  #endif
  
  
  double E, E0, err;

  char *comandi[] = {/*"set term wxt 0", "plot  'pendolo.dat' u 1:2",*/ "set term wxt 1", "plot [-3:3][-0.5:2.5] 'pendolo.dat' u 2:3"};
    
  FILE *gnuplotPipe;
  gnuplotPipe = popen ("gnuplot -persistent","w");

  if(gnuplotPipe == NULL)
  printf("Something went wrong...");
  
  FILE *pf;
  pf = fopen ("pendolo.dat", "w");

  energy(&E0, phase, &param);


  for(int i = 0; i <= tsteps; i++)
    {
      
      double tempo = i *dt;
      
      rungekutta(phase, phase, tempo, dt, (void*)&param);
      energy(&E, phase, &param);
      err = (E-E0)/E0;

     
      if(phase[0] > M_PI)                          //quando ho fattole ultime prove questo era tolto
	phase[0] = phase[0] - 2 * M_PI;
      else if(phase[0] < - M_PI)
      phase[0] = phase [0] + 2 * M_PI;

      #ifdef POINCARE
      if((tempo > term) && (i%1000 == 0))
	fprintf(pf, "%16g %16g %16g\n", tempo,  phase[0], phase[1]);
      #endif
	
     #ifdef SIMULATION 
      fprintf(pf, "%16g %16g %16g %16g %16g\n", tempo, phase[0], phase[1], E, err);
     #endif
    }
  
  fclose(pf);
  

  #ifdef ATTRAZIONE
  if(phase[1]<0)
    printf("%16g %16g\n", x0, v0);
  #endif
   
  #ifdef SIMULATION
  for (int i=0; i < COMANDI; i++){
    fprintf(gnuplotPipe, "%s\n", comandi[i]);
  }
  #endif
  
  #ifdef POINCARE
  for (int i=0; i < COMANDI; i++){
    fprintf(gnuplotPipe, "%s\n", comandi[i]);
  }
  #endif

  pclose(gnuplotPipe);

  return 0;
}
