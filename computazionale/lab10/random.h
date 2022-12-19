#include<stdlib.h>
#include<math.h>
#include<stdlib.h>

typedef unsigned long long int RANDOM;

//I'=(b+a*I)%mod

RANDOM seed, add, mul, mod;



void set_minimal_std(){
  mod = (1ULL<<31) - 1ULL;
  add = 0ULL;
  mul = 16807ULL;
}
void set_Ecuyer2(){
  mod = (1ULL<<31) - 1ULL;
  add = 0ULL;
  mul = 1385320287ULL;
}
void set_randu(){
  mod = (1ULL<<31);
  add = 0ULL;
  mul = 65539ULL;
}
void set_my_LCG( RANDOM a, RANDOM b, RANDOM c){
  mul = (RANDOM) a;               
  add = (RANDOM) b;
  mod = (RANDOM) c;
}

RANDOM get_int(){
  seed = (mul * seed + add) % mod;
  return seed;
}
double get_real(){
  double real = (double) get_int() / (double) mod;
  return real;
}
    
double get_normal(){
  double norm = sqrt(-2 * log(get_real())) * cos(2 * M_PI * get_real());
  return norm;
}

int get_intero( int A,  int B){ return (int) get_int() % (B - A + 1) + A; } 


