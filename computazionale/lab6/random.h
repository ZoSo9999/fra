
#include<stdlib.h>
#include<math.h>
#include<stdlib.h>

typedef unsigned long long int RANDOM;

//I'=(b+a*I)%mod

RANDOM seed, add, mul, mod;

void set_minimal_std(){
  mod = (1<<31) - 1ULL;
  add = 0;
  mul = 16807;
}
void set_randu(){
  mod = (1<<31);
  add = 0;
  mul = 65539;
}
void set_my_LCG( RANDOM a, RANDOM b, RANDOM c){
  mul = (RANDOM) a;               //ATTENZIONE GLI INDICI DEGLI ARRAY NON SONO GIUSTI
  add = (RANDOM) b;
  mod = (RANDOM) c;
}

RANDOM get_int(){
  seed = (mul * seed + add) % mod;;
  return seed;
}
double get_real(){
  double real;
  return real = (double) get_int() / mod;
}
    
RANDOM get_normal(){
  double norm = sqrt(-2 * log(get_real())) * cos(2 * M_PI * get_real());
  return norm;
}


