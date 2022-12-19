#include <stdio.h>
#include <math.h>
#include<stdlib.h>

typedef struct {double x, v;} phase ;
typedef struct {double k, m;} parameters;

phase eulero (phase point, parameters param, double dt, double (*)(phase, parameters));
phase cromer (phase point, parameters param, double dt, double (*)(phase, parameters));
phase midpoint (phase point, parameters param, double dt, double (*)(phase, parameters));
phase verlet (phase point, parameters param, double dt, double (*)(phase, parameters));
phase velocityverlet (phase point, parameters param, double dt, double (*)(phase, parameters));

double elastic (phase point, parameters param);
double pendolum (phase, parameters);
double energy (phase point, parameters param, int forza);

#define g 9.81

int main( int argc, char *argv [] ){
 

  if(argc<8)
   {
     printf("tmax dt x0 v0 k method force\n");
     printf("METHODS\t 0=eulero 1=cromer 2=midpoint 3=verlet 4=velocityverlet\n");
     printf("FORCES\t 0=elastic 1=pendolum\n");
     exit(-1);
   }

  //fare controllo sulle varibili di imput

  int method = atoi(argv[6]);
  int force = atoi(argv[7]);

  double (*strongs[ 2 ]) (phase point, parameters param) = {elastic, pendolum};
  double (*strong) (phase point, parameters param) = strongs [force];
  phase (*algos[ 5 ]) (phase point, parameters param, double dt, double (*)(phase, parameters)) = {eulero, cromer, midpoint, verlet, velocityverlet};
  phase (*algo)(phase point, parameters param, double dt, double (*)(phase, parameters)) = algos [method]; 

  double tmax = atof(argv[1]);
  double dt = atof(argv[2]);
  phase p = { atof(argv[3]), atof(argv[4]) };
  parameters param = { atof(argv[5]), 1 };  // massa fissata a 1kg
  
  int tsteps = (int) tmax/dt;
  double E0 = energy(p, param, force);
  double E,err;

  //printf("%s %15s %16s %16s %16s %16s\n\n", str[0], str[1], str[2], str[3], str[4], str[5]);   intestazioni all'inizio

  printf("0\t\t %16f %16f %16f 0\n", p.x, p.v, energy(p, param, force));
  

  for(int i = 1; i <= tsteps; i++)
    {
      p = algo(p, param, dt, strong);
      E = energy(p, param, force);
      err = (E-E0)/E0;
      printf("%16f %16f %16f %16f %16f\n", dt*i, p.x, p.v, E, err);
    }

  printf("# ERR %16f %16f\n", dt, err);
  
  return 0;
}



/************************FORCES***************************/

double elastic (phase point, parameters param){

  double f = -param.k/param.m * point.x;
  return f;
}

double pendolum (phase point, parameters param){

  double f = -param.k * sin(point.x);
  return f;
}



/***********************ALGORITHMS************************/

phase eulero (phase point, parameters param, double dt, double (*force)(phase, parameters)){

  double xnew = point.x + point.v*dt;
  point.v = point.v + force(point, param)*dt;
  point.x = xnew;

  return point;
}

phase cromer (phase point, parameters param, double dt, double (*force)(phase, parameters)){

  point.v = point.v + force(point, param)*dt;
  point.x = point.x + point.v*dt;

  return point;
}

phase midpoint (phase point, parameters param, double dt, double (*force)(phase, parameters)){

  point.x = point.x + point.v*dt + force(point, param)*dt*dt/2;
  point.v = point.v + force(point, param)*dt;
      
  return point;
}

/*phase leapfrog (phase point, parameters param, double dt, double (*force)(phase, parameters)){        UN BEL CASINO IMPLEMETARLO BENE IN QUESTO CODICE

  // primo step
  point.v = point.v + force(point, param)*dt/2;

  //gli altri step
  point.x = point.x + point.v * dt;
  vplus = point.v + force(point. param)*dt;
  point.v = (point.v + vplus) / 2;
  

  return point;
  }*/

phase verlet (phase point, parameters param, double dt, double (*force)(phase, parameters)){            // NON È FUNZIONANTE

  // primo step
  //x0 = point.x;
  //point.x = point.x + point.v * dt + force(point, param)*dt*dt/2;

  // gli altri step
  //point.x = 2 * point.x + force(point, param)*dt*dt - x0;
  
  return point;
}

phase velocityverlet (phase point, parameters param, double dt, double (*force)(phase, parameters)){

  phase point0  = point;
  point.x = point.x + point.v*dt + force(point, param)*dt*dt/2;
  point.v = point.v + (force(point, param) + force(point0, param))*dt/2;
      
  return point;
}



/***********************ENERGY*************************/

double energy (phase point, parameters param, int forza){

  double E;

  if (forza == 0)
    E = (param.m*point.v*point.v + param.k*point.x*point.x)/2;
  else
    E = param.m*point.v*point.v/2 + param.m*g*g*(1-cos(point.x))/param.k;
  
  return E;
}
