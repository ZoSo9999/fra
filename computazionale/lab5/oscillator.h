#define DIM 2
#include<stdlib.h>
#include<math.h>
#include "rungekutta.h"

#define g 9.81

typedef struct {double T, gamma, forz, Fomega;} parameters;

/*void drift (double *pin, double *pout, double t, double dt, void *param){

  parameters *P = (parameters *) param;

  for(int i = 0 ; i < DIM/2 ; i++){
    double temp[DIM];
    pcopy(pin, temp);
    
    pout [i] = dt * pin [i + DIM/2];
    pout [i + DIM/2] =  dt * (- (2 * M_PI/P->T)* (2 * M_PI/P->T) * sin(temp [i]) - P->gamma * pin [i + DIM/2] + P->forz * cos( P->Fomega * t));
  }
  }*/

void energy (double *E, double *p, void *param){

  parameters *para = (parameters *) param;
  *E=0;

  for(int i = 0 ; i < DIM/2 ; i++)
    *E += p[i + DIM/2] * p[i + DIM/2] / 2 + g * g * (1-cos(p[i])) * para->T * para ->T / (4 * M_PI * M_PI);
}


void drift (double *pin, double *pout, double t, double dt, void *param){
  
  parameters *P = (parameters *) param;

  double temp = pin [0];

  pout[0] = dt * pin[1];
  pout[1] = dt * (- ( 2 * M_PI/P->T) * (2 * M_PI/P->T) * sin(temp) - P->gamma * pin [1] + P->forz * cos( P->Fomega * t));
  
}
