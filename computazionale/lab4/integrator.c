#include<stdlib.h>
#include<stdio.h>
#include "oscillator.h"

#define COMANDI 4

int main (int argc, char *argv []){

  parameters param = {2 * M_PI, 0.5, 0., 1/3};
  double t = 100;
  double dt = 0.001;
  double tsteps = t/dt;
  double phase [DIM] = {M_PI /2, 0.};
  double out [DIM];
  double E, E0, err;

  char *comandi[] = {"set term wxt 0", "plot 'pendolo.dat' u 1:2 w l", "set term wxt 1", "plot 'pendolo.dat' u 2:3 w l"};
  FILE *gnuplotPipe;
  gnuplotPipe = popen ("gnuplot -persistent","w");

  if(gnuplotPipe == NULL)
  printf("Something went wrong...");
  
  FILE *pf;
  pf = fopen ("pendolo.dat", "w");

  energy(&E0, phase, &param);
  

  for(int i = 1; i <= tsteps; i++)
    { 
      rungekutta(phase, out, dt*(i), dt, &param);
      energy(&E, phase, &param);
      err = (E-E0)/E0;

      /*if(phase[0] > M_PI)             //posso renderlo una funzione e non perndere genralità sul vettore phase come sotto
	phase[0] = phase[0] - 2 * M_PI;
      else if(phase[0] < - M_PI)
      phase[0] = phase [0] + 2 * M_PI;*/
      
      fprintf(pf, "%16f %16f %16f %16f %16f\n", dt*i, out[0], out[1], E, err);   //qua c'è perdita di genralità sul vettore phase perchè lo sto supponendo di due componenti (risolvo con un for)
    }

  fprintf(pf, "# ERR %16f %16f\n", dt, err);   //qua c'è perdita di genralità sul vettore phase perchè lo sto supponendo di due componenti (risolvo con un for)
  fclose(pf);


  
  for (int i=0; i < COMANDI; i++){
    fprintf(gnuplotPipe, "%s\n", comandi[i]);
  }

  pclose(gnuplotPipe);

  return 0;
}
