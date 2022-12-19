#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "statistic.h"
#include "random.h"


int main(int argc, char *argv[]){

  if(argc<3){
    printf("insert: N BINS LGC METHOD\n");
    printf("1-->MINIMAL STD\t 2-->RANDU\t 3-->MY LCG\t 4-->RAND");
    exit(-1);
  }
  
  int N = atoi(argv[1]);
  int bins = atoi(argv[2]);
  int choice = atoi(argv[3]);

  seed = time(0);
  
  double *array = (double *) calloc(N, sizeof (double));
  if(choice == 1)
    set_minimal_std();
  else if(choice == 2)
    set_randu();
  else if(choice == 3)
    set_my_LCG(1, 2, 3);                    //ATTENZIONE NON PERMETTE DI USARE RAND O RAND48

 
  for(int i=0; i<N; i++)
    array[i] = get_real();
  
  stat statistic = get_statistic(array, N, bins);
  print_statistic(statistic);
  return 0;
}
