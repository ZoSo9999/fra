#include<stdio.h>
#include "rungekuttaII.h"


void f(double *arrin, double *arrout, double t, void *param){

  double *n = (double*) param;
  double temp = arrin[0];

  arrout[0] = arrin[1];
  arrout[1] = - pow(temp,*n) - 2*arrin[1]/t;
}


int main (int argc, char *argv []){

  if(argc < 7){
    printf("Wrong imput:\n n r0 P0 D0 dr rMAX\n");
    exit(-1);
  }

  double n = atof(argv[1]);
  double r0 = atof(argv[2]);
  double phase[DIM] = {atof(argv[3]), atof(argv[4])};
  double dr = atof(argv[5]);
  double rmax = atof(argv[6]);

  int counts = rint( (rmax-r0) /dr);

  printf("#r\t\tP\t\tD\n");
  printf("%16.12g %16.12g %16.12g\n", r0, phase[0], phase[1]);

  for(int i=0; i<counts; i++){

    double r = r0 + i*dr;
    rungekuttaII(phase, phase, r, dr, (void*)&n);

    printf("%16.12g %16.12g %16.12g\n", r + dr, phase[0], phase[1]);
  }

  
  return 0;
}
