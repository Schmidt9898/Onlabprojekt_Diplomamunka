#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
//#include "Timer_C.h"

/* constans méretek */
size_t size1 = 1000;
size_t size0 = 1000;

typedef struct dataobj dataobj;
struct dataobj
{
  void *__restrict data;
};


void gpu_function(struct dataobj *__restrict O)
{
float(*__restrict o)[size1] = (float(*)[size1])O->data;
#pragma omp target teams distribute parallel for collapse(2)
  for (size_t i = 0; i < size0; i++)
  {
    //out[i]=in[i]+in[i]/2.0;
    for (size_t j = 0; j < size1; j++)
    {
      o[i][j]=i;
    }
  }
}


void Gpu(struct dataobj *__restrict O)
{
  float(*__restrict o)[size1] = (float(*)[size1])O->data;
  /* offload the data to gpu  */
    #pragma omp target data map(tofrom : o [0:size0][0:size1])
      {
        /*do a lot of iteration*/
        for(size_t i=0;i<100;i++){
          gpu_function(O);
        }

        /* 
      for(size_t i=0;i<size0;i++)
      { 
        //out[i]=in[i]+in[i]/2.0;
        for(size_t j=0;j<size1;j++)
        { 
            out_[i][j]=in_[i][j]+in_[i][j]/2.0;
        }
      }*/
      }
}

int main(int argc, char **argv)
{

  /*start the clock*/
  clock_t begin = clock();
  double time_spent ;
  
  /*print memory usage*/
  printf("took %f GB memory needed.\n", (float)sizeof(float) * size0 * size1  / 100000000);

  
  dataobj O;
  O.data=malloc(sizeof(float) * size0 * size1);
  
  begin = clock();

  Gpu(&O);
  
  clock_t end = clock();
  
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Gpu calculate took %f \n", time_spent);


  for (size_t i = 0; i < 10; i++)
  {
    //out[i]=in[i]+in[i]/2.0;
    for (size_t j = 0; j < 10; j++)
    {
      printf(" %f ",((float*)O.data)[i*size0+j]); 
    }
    printf(" \n ");
  }

  return 0;
}
