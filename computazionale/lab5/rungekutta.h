#include<stdlib.h>
#include<math.h>


void psum (double *a, double *b, double *sum){
  for (int i=0; i<DIM ; i++)
    sum [i] = a [i] + b [i];
}
void pfma (double c, double *a, double *b, double *result){      // result = a*c + b
  for (int i=0; i<DIM ; i++)
    result [i] = fma (c, a [i], b[i]);
}
void pcopy (double *a, double *b){
  for (int i=0; i<DIM ; i++)
    b [i] = a [i];
}

void drift (double *pin, double *pout, double t, double dt, void *param);
void energy (double *E, double *p, void *param);
void rungekutta ( double *pin, double *pout, double t, double dt, void *param){

  double k [DIM];
  double h [DIM];
  double temp [DIM];

  //y1
  drift (pin, k, t, dt, param);
  pcopy (k, temp);

  //y2
  pfma (0.5, k, pin, h);
  drift (h, k, t + 0.5 * dt, dt, param);
  pfma (2., k, temp, temp);

  //y3
  pfma (0.5, k, pin, h);
  drift (h, k, t + 0.5 * dt, dt, param);
  pfma (2., k, temp, temp);

  //y4
  psum (k, pin, h);
  drift (h, k, t + dt, dt, param);
  psum (k, temp, temp);

  //media finale
  pfma (1./6., temp, pin, pout);

}
  
