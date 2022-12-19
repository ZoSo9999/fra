#include <stdio.h>
#include <math.h>
#include<stdlib.h>



int main( int argc, char *argv [] ){

  double tmax;
  double dt;
  int tsteps;
  int count;
  double v;
  double x, xnew;
  double E0;
  double E;
  double m, k;
  double ciao = 0;
  double f, err;
  char *str [] = {"# SIMULATION START", "t", "x", "v", "E", "(E-E0)/E0"};
  
  if(argc<7)
   {
     printf("Wrong imput: tmax dt x0 v0 omega_quadro\n");
     exit(-1);
   }

  tmax = atof(argv[1]);
  dt = atof(argv[2]);
  x = atof(argv[3]);
  v = atof(argv[4]);
  m = atof(argv[5]);
  k = atof(argv[6]);
  
  tsteps = (int) tmax/dt;
  E0 = (m*v*v + k*x*x)/2;
  E=E0;
  const double H = k/m;

  printf("# mass=%g\t k=%g\t omega_quad=%g\t period=%g\n", m, k, H, 2*M_PI/H);
  
  printf("%s %15s %16s %16s %16s %16s\n\n", str[0], str[1], str[2], str[3], str[4], str[5]);

  printf("\t\t %16f %16f %16f %16f %16f\n", ciao, x, v, E, (E-E0)/E0);
   
  for(count=1; count<=tsteps; count++)
    {
      f = -H*x;
      
      #ifdef EULERO
      xnew = xnew + v*dt;
      v = v + f*dt;
      x=xnew;
      #endif

      #ifdef EULEROCROMER
      v = v + f*dt;
      x = x + v*dt;
      #endif

      #ifdef CENTRAL
      x = x + v*dt + f*dt*dt/2;
      v = v + f*dt;
      #endif
      
     E = (m*v*v + k*x*x)/2;
     err = (E-E0)/E0;

    
     printf("\t\t %16f %16f %16f %16f %16f\n", dt*count, x, v, E, err);
    }

  printf("# ERR %16f %16f\n", dt, err); 

  return(0);
}
  
