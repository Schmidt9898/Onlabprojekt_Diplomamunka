#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
//#include "Timer_C.h"

//my function
size_t size1 = 1000;
size_t size0 = 50000;

void bf0(float (*__restrict in_)[size1], float (*__restrict out_)[size1])
{
//#pragma omp target update to(in_) from(out_)  

#pragma omp target teams distribute parallel for collapse(2) 
  for (size_t i = 0; i < size0; i++)
  {
    //out[i]=in[i]+in[i]/2.0;
    for (size_t j = 0; j < size1; j++)
    {
      out_[i][j] = in_[i][j] + in_[i][j] / 2.0;
    }
  }
}

void bf1(float (*__restrict in_)[size1], float (*__restrict out_)[size1])
{
#pragma omp target teams distribute parallel for collapse(2)
  for (size_t i = 0; i < size0; i++)
  {
    //out[i]=in[i]+in[i]/2.0;
    for (size_t j = 0; j < size1; j++)
    {
      out_[i][j] = in_[i][j] - in_[i][j] / 4.0;
    }
  }
}

void F(float *in, float *out)
{
  float(*__restrict in_)[size1] = (float(*)[size1])in;
  float(*__restrict out_)[size1] = (float(*)[size1])out;

#pragma omp target data map(to : in [0:size0*size1]) map(from : out [0:size0*size1])
  {
    for(size_t i=0;i<30;i++)
      bf0(in_, out_);
    //bf1(in_, out_);

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

  clock_t begin = clock();

  printf("took %f GB memory needed.\n", (float)sizeof(float) * size0 * size1 * 2 / 100000000);

  float *array = malloc(sizeof(float) * size0 * size1);
  float *array2 = malloc(sizeof(float) * size0 * size1);

  F(array, array2);

  clock_t end = clock();

  if (argc > 1)
  {
    for (size_t i = 0; i < 1000000000; i++)
    {
      printf("%f \n", array2[i]);
    }
  }

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("took %f \n", time_spent);
  return 0;
}

/*
nt initdamp(struct dataobj *restrict damp_vec, const float h_x, const float h_y, const float h_z, const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m, const int abc_x_l_ltkn, const int abc_x_r_rtkn, const int abc_y_l_ltkn, const int abc_y_r_rtkn, const int abc_z_l_ltkn, const int abc_z_r_rtkn, struct profiler * timers)
{
  float (*restrict damp)[damp_vec->size[1]][damp_vec->size[2]] __attribute__ ((aligned (64))) = (float (*)[damp_vec->size[1]][damp_vec->size[2]]) damp_vec->data;

 
}
  #pragma omp target enter data map(to: damp[0:damp_vec->size[0]][0:damp_vec->size[1]][0:damp_vec->size[2]])*/