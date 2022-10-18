#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
//#include "Timer_C.h"

/* constans méretek */
size_t size1 = 1000;
size_t size0 = 1000;

struct dataobj
{
  void* data;
}


void maxker(float (*__restrict one_)[size1])
{
#pragma omp target teams distribute parallel for collapse(2)
  for (size_t i = 0; i < size0-1; i++)
  {
    //out[i]=in[i]+in[i]/2.0;
    for (size_t j = 0; j < size1-1; j++)
    {
      for (int x = 0; x < 2; x++)
      {
        for (int y = 0; y < 2; y++)
        { //maxkeresés fentebb toljuk
          if (one_[i + x][j + y] > one_[i][j])
            one_[i][j] = one_[i + x][j + y];
        }
      }
    }
  }
}
/*
void bf1(float (*__restrict one_)[size1], float (*__restrict two_)[size1])
{
#pragma omp target teams distribute parallel for collapse(2)
  for (size_t i = 0; i < size0; i++)
  {
    //out[i]=in[i]+in[i]/2.0;
    for (size_t j = 0; j < size1; j++)
    {
      for (int x = -1; x < 2; x++)
      {
        for (int y = -1; y < 2; y++)
        { //maxkeresés one -> two
          if (one_[i + x][j + y] > two_[i][j])
            two_[i][j] = one_[i + x][j + y];
        }
      }
    }
  }
}
*/




//void F(float *in, float *out)

/**/
void Gpu_Max(float *one)
{
  float(*__restrict one_)[size1] = (float(*)[size1])one;
 // float(*__restrict two_)[size1] = (float(*)[size1])two; //dont need for this
  /* offload the data to gpu  */
    #pragma omp target data map(tofrom : one [0:size0*size1]) //map(from : two [0:size0*size1])
      {
        size_t len=sqrt(size0*size0+size0*size0);
        
        /*do a lot of iteration*/
        for(size_t i=0;i<len;i++){
          maxker(one_);
          
          //bf1(one_,two_);
          //void *temp= one_;
          //one_=two_;
          //two_=temp;
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

size_t len=sqrt(size0*size0+size0*size0);
        printf("len: %lu \n",len);

  /*init random generator*/
  time_t t;
  srand((unsigned) time(&t));

  /*start the clock*/
  clock_t begin = clock();
  //printf("Time: %lu \n", clock());
  
  /*print memory usage*/
  printf("took %f GB memory needed.\n", (float)sizeof(float) * size0 * size1  / 100000000);

  
  float *in = malloc(sizeof(float) * size0 * size1);
  //float *out = malloc(sizeof(float) * size0 * size1);

  /*fill with random numbers*/
  for(size_t i =0;i<size0 * size1;i++)
  {
    in[i]=rand()%1000000 /1000.0;
  }
  in[size0 * size1 - 1]=1001.0;
  
  clock_t end = clock();
  //printf("Time: %lu \n", clock());
  /*end of the first timer*/
  
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("rand took %f \n", time_spent);
  
  begin = clock();

  float max_=in[0];
  for(size_t i =0;i<size0 * size1;i++)
    {
      if(in[i]>max_)
        max_=in[i];
    }
  printf("max_ %f \n", max_);
  end=clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("CPU calculate took %f \n", time_spent);
  begin = clock();


  //printf("Time: %lu \n", clock());
  //F(in, out);
  Gpu_Max(in);
  end = clock();
  //printf("Time: %lu \n", clock());


time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
printf("calculate took %f \n", time_spent);
begin = clock();
//printf("Time: %lu \n", clock());


  for (size_t i = 0; i < 10; i++)
  {
    //out[i]=in[i]+in[i]/2.0;
    for (size_t j = 0; j < 10; j++)
    {
      printf(" %f ",in[i*size0+j]); 
    }
    printf(" \n ");
  }

end = clock();
//printf("Time: %lu \n", clock());

time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("calculate took %f \n", time_spent);

/*

  if (argc > 1)
  {
    for (size_t i = 0; i < 1000000000; i++)
    {
      printf("%f \n", in[i]);
    }
  }*/


  return 0;
}

/*
nt initdamp(struct dataobj *restrict damp_vec, const float h_x, const float h_y, const float h_z, const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m, const int abc_x_l_ltkn, const int abc_x_r_rtkn, const int abc_y_l_ltkn, const int abc_y_r_rtkn, const int abc_z_l_ltkn, const int abc_z_r_rtkn, struct profiler * timers)
{
  float (*restrict damp)[damp_vec->size[1]][damp_vec->size[2]] __attribute__ ((aligned (64))) = (float (*)[damp_vec->size[1]][damp_vec->size[2]]) damp_vec->data;

 
}
  #pragma omp target enter data map(to: damp[0:damp_vec->size[0]][0:damp_vec->size[1]][0:damp_vec->size[2]])*/