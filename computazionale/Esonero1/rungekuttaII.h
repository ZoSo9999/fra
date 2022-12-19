#include<stdlib.h>
#include<math.h>

#define DIM 2

void pfma (double c, double *a, double *b, double *result){      // result = a*c + b
  for (int i=0; i<DIM ; i++)
    result [i] = fma (c, a [i], b[i]);
}

void f(double *arrin, double *arrout, double t, void *param);

void rungekuttaII ( double *arrin, double *arrout, double t, double dt, void *param){

  double k [DIM];
  double h [DIM];

  //k1
  f (arrin, k, t, param);

  //k2
  pfma (0.5 * dt, k, arrin, h);
  f (h, k, t + 0.5 * dt, param);    //questo è proprio k2
  pfma (dt, k, arrin, arrout);
}
  
