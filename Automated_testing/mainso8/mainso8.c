/*

This is an example for making tiling/blocking in openMP.
The calculation purpose is to reach a lot of memory from a single kernel.

Cpu ram-> gpu vram -> compute -> gpu vram- > cpu ram
*/

//stopper functions defined here, you may choose not to use them with -D NO_TIME
void Spawn_stopper(char *name);
double Kill_stopper();

#include "stdlib.h"
#include <stdio.h>
#include <math.h>
#ifdef LOMP
#include "omp.h"
#elif LACC
#include "openacc.h"
#endif
/* constans méretek */
/*
#define X 800
#define Y 800
#define Z 800
*/
const int sizex = 800; //If this is not constant, will cause a segfault in runtime with clang-12
const int sizey = 800; //and clang-14 if -D
const int sizez = 800;

struct dataobj{void *data;};

int main(int argc, char **argv)
{

#ifdef LOMP
printf("Hi this is openMP tiling test, \n");
#elif LACC
printf("Hi this is openACC tiling test, \n");
#else
#pragma message "OpenMP or OpenACC was not defined use one of -LACC or LOMP "  __FILE__
#endif

#ifdef L1D
  printf("compiled with 1 dimensional arrays.\n");
  #define Ddim(arr,x,y,z) arr[(z) + (y)*sizez + (x)*sizez * sizey]

#elif L3D
  printf("compiled with 3 dimensional arrays.\n");
  #define Ddim(arr,x,y,z) arr ## _[x][y][z]
#else
  #pragma message "Dimension was not defined use one of L1D or L3D " 
#endif

#define KERNEL_WINDOW(out,x,y,z) Ddim(out,x,y,z) += \
                 Ddim(data,x,y,z - 4) + Ddim(data,x,y,z - 3) + Ddim(data,x,y,z - 2) + Ddim(data,x,y,z - 1) +\
                 Ddim(data,x,y,z + 4) + Ddim(data,x,y,z + 3) + Ddim(data,x,y,z + 2) + Ddim(data,x,y,z + 1) +\
                 Ddim(data,x,y - 4,z) + Ddim(data,x,y - 3,z) + Ddim(data,x,y - 2,z) + Ddim(data,x,y - 1,z) +\
                 Ddim(data,x,y + 4,z) + Ddim(data,x,y + 3,z) + Ddim(data,x,y + 2,z) + Ddim(data,x,y + 1,z) +\
                 Ddim(data,x - 4,y,z) + Ddim(data,x - 3,y,z) + Ddim(data,x - 2,y,z) + Ddim(data,x - 1,y,z) +\
                 Ddim(data,x + 4,y,z) + Ddim(data,x + 3,y,z) + Ddim(data,x + 2,y,z) + Ddim(data,x + 1,y,z);

#define KERNEL_WINDOW2(out,x,y,z) float r10 = 1.0F/(Ddim(data1,x + 8, y + 8, z + 8)*Ddim(data1,x + 8, y + 8, z + 8)); \
Ddim(out, x + 8, y + 8, z + 8) = (r9*Ddim(data2, x + 1, y + 1, z + 1)*Ddim(data3, x + 8, y + 8, z + 8) + r10*(-r8*(-2.0F*Ddim(data3, x + 8, y + 8, z + 8)) - r8*Ddim(data4, x + 8, y + 8, z + 8)) + 7.93650813e-6F*(-Ddim(data3, x + 4, y + 8, z + 8) - Ddim(data3, x + 8, y + 4, z + 8) - Ddim(data3, x + 8, y + 8, z + 4) - Ddim(data3, x + 8, y + 8, z + 12) - Ddim(data3, x + 8, y + 12, z + 8) - Ddim(data3, x + 12, y + 8, z + 8)) + 1.12874782e-4F*(Ddim(data3, x + 5, y + 8, z + 8) + Ddim(data3, x + 8, y + 5, z + 8) + Ddim(data3, x + 8, y + 8, z + 5) + Ddim(data3, x + 8, y + 8, z + 11) + Ddim(data3, x + 8, y + 11, z + 8) + Ddim(data3, x + 11, y + 8, z + 8)) + 8.8888891e-4F*(-Ddim(data3, x + 6, y + 8, z + 8) - Ddim(data3, x + 8, y + 6, z + 8) - Ddim(data3, x + 8, y + 8, z + 6) - Ddim(data3, x + 8, y + 8, z + 10) - Ddim(data3, x + 8, y + 10, z + 8) - Ddim(data3, x + 10, y + 8, z + 8)) + 7.11111128e-3F*(Ddim(data3, x + 7, y + 8, z + 8) + Ddim(data3, x + 8, y + 7, z + 8) + Ddim(data3, x + 8, y + 8, z + 7) + Ddim(data3, x + 8, y + 8, z + 9) + Ddim(data3, x + 8, y + 9, z + 8) + Ddim(data3, x + 9, y + 8, z + 8)) - 3.79629639e-2F*Ddim(data3, x + 8, y + 8, z + 8))/(r8*r10 + r9*Ddim(data2, x + 1, y + 1, z + 1));



const int blocksize_x = 8;
const int blocksize_y = 4;
const int blocksize_z = 32;

printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
//window size
const int window_size = 8;

const size_t meret = sizex * sizey * sizez;
printf("memory size needed: %lu , %f Gb \n", meret * 2, meret * 4 / 1e9f * 2);



//Spawn_stopper("offload and memory managment");

//data and offloading OpenMP

//this we will use on the cpu
float * out = (float *)malloc(meret * 4);
float * out2 = (float *)malloc(meret * 4);

#ifdef LOMP
float * __restrict data = NULL;
float * __restrict data1 = NULL;
float * __restrict data2 = NULL;
float * __restrict data3 = NULL;
float * __restrict data4 = NULL;
#pragma omp target data map(alloc : data[0:meret]) \
            map(alloc : data1[0:meret]) \
            map(alloc : data2[0:meret]) \
            map(alloc : data3[0:meret]) \
            map(alloc : data4[0:meret]) \
            map(from  : out[0:meret])  \
            map(from  : out2[0:meret])
#elif LACC
#pragma acc data copyout(out[0:meret]) copyout(out2[0:meret]) 
#endif
{
//Kill_stopper();

#ifdef LACC
float* data=(float*)acc_malloc(meret*4);
#endif


float(*__restrict data_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data;
float(*__restrict data1_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data1;
float(*__restrict data2_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data2;
float(*__restrict data3_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data3;
float(*__restrict data4_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data4;
float(*__restrict out_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out;
float(*__restrict out2_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out2;


//init aka zeroing
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#elif LACC
#pragma acc parallel loop collapse(3) deviceptr(data,data1,data2,data3,data4,data_,data_1,data_2,data_3,data_4)
#endif
  for (int x = 0; x < sizex ; x++)
  {
    for (int y = 0; y < sizey ; y++)
    {
      for (int z = 0; z < sizez ; z++)
      {
        //kernel start
        Ddim(data,x,y,z) = 1.0;
        Ddim(data1,x,y,z) = 1.0;
        Ddim(data2,x,y,z) = 1.0;
        Ddim(data3,x,y,z) = 1.0;
        Ddim(data4,x,y,z) = 1.0;
        Ddim(out,x,y,z) = 0.0;
        Ddim(out2,x,y,z) = 0.0;
        
      }
    }
  }

float dt = 1.816000;
float r8 = 1.0F/(dt*dt);
float r9 = 1.0F/dt;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Spawn_stopper("3d computation collapse (3)");
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#elif LACC
#pragma acc parallel loop collapse(3) deviceptr(data,data1,data2,data3,data4,data_,data_1,data_2,data_3,data_4)
#endif
  for (int x = 0; x < sizex - 2*window_size; x++)
  {
    for (int y = 0; y < sizey - 2*window_size; y++)
    {
      for (int z = 0; z < sizez - 2*window_size; z++)
      {
        //kernel start
         KERNEL_WINDOW2(out,x,y,z)
      }
    }
  }
Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*if(out2_==out2)
printf("pointer: %x   %x\n",out2_,out2);
else
printf("asd\n");*/


#ifdef LOMP
Spawn_stopper("3d tiling OpenMP");
#pragma omp target teams distribute collapse(3) thread_limit(256) deviceptr(data,data1,data2,data3,data4,data_,data_1,data_2,data_3,data_4)
  for (int x = 0; x < sizex - 2*window_size; x += blocksize_x)
    for (int y = 0; y < sizey - 2*window_size; y += blocksize_y)
      for (int z = 0; z < sizez - 2*window_size; z += blocksize_z)
      {
  #pragma omp parallel for collapse(3) //shared(out2_,data_)
        for (int bx = x; bx < x + blocksize_x; bx++)
          for (int by = y; by < y + blocksize_y; by++)
            for (int bz = z; bz < z + blocksize_z; bz++)
            {
              if (bx < sizex - 2*window_size &&
                by < sizey - 2*window_size &&
                bz < sizez - 2*window_size)
              {
          //data_=1;
          //printf("%p %p %p\n",data_,out_,out2_);
                  KERNEL_WINDOW2(out2,bx,by,bz);
         /* printf("----");
          if(out2==out2_)
            printf("pointer: %x   %x\n",out2,out2);
          else
          printf("asd\n");*/
        //out2_[x][y][z] +=24;
        /*
          data_[x][y][z - 4] + data_[x][y][z - 3] + data_[x][y][z - 2] + data_[x][y][z - 1] +
          data_[x][y][z + 4] + data_[x][y][z + 3] + data_[x][y][z + 2] + data_[x][y][z + 1] +
          data_[x][y - 4][z] + data_[x][y - 3][z] + data_[x][y - 2][z] + data_[x][y - 1][z] +
          data_[x][y + 4][z] + data_[x][y + 3][z] + data_[x][y + 2][z] + data_[x][y + 1][z] +
          data_[x - 4][y][z] + data_[x - 3][y][z] + data_[x - 2][y][z] + data_[x - 1][y][z] +
          data_[x + 4][y][z] + data_[x + 3][y][z] + data_[x + 2][y][z] + data_[x + 1][y][z];*/
              }
            }
      }
#elif LACC
Spawn_stopper("3d tiling OpenACC");
#pragma acc parallel loop deviceptr(data_) deviceptr(data) tile(blocksize_z,blocksize_y,blocksize_x) private(sizex,sizey,sizez) // tile(4,8,16) vector_length(512)
for(int x=window_size;x<sizex-window_size;x++)
  {
      for(int y=window_size;y<sizey-window_size;y++)
      {
          for(int z=window_size;z<sizez-window_size;z++)
          {
              KERNEL_WINDOW(out2,x,y,z)
          }
      }
  }
#endif

Kill_stopper();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Spawn_stopper("back to ram");
}
//Kill_stopper();

for (int i = 0; i < meret; i++) {
  if (out[i]!=out2[i]) {
    printf("Validation failed\n");
  printf("out1 %f != out2 %f\n",out[i],out2[i]);
    break;
  }
  
}
free(out);
free(out2);

return 0;
}

//END of example

//If you dont want to compile this code use -D NO_TIME in compile option.

//This code is for the stopper not part of the example
//Do not change this code
//The example code is up

#ifdef NO_TIME
void Spawn_stopper(char* name){};
double Kill_stopper(){};
#else
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

typedef struct Stopper Stopper();

double Totaltime=0;
int Sum_Stopper=0;
// Stoppers follow LIFO structure
struct Stopper * Stopper_root=NULL; //root of the timers

//get the curent time
//also it does magic because i dont remember what it does actualy
double op_timer_core() {
struct timeval t;
gettimeofday(&t, (struct timezone *)0);
return t.tv_sec + t.tv_usec * 1.0e-6;
}

// proper definition of the stopper
struct Stopper
{
  /* data */
  struct Stopper* parent; //am i a child
  double start,end;       //preaty self explain
  char name[256];          //20 caracter for a name is enought //update it is few week later and 256 because turns out it isn't enought, this took like 3 painfull hours to find out.

};


void Spawn_stopper(char* name)
{
      struct Stopper * stopper= malloc(sizeof(struct Stopper));
      printf("%s ID:%d started->\n",name,Sum_Stopper);
      strcpy(stopper->name,name);
      stopper->start=op_timer_core();
      stopper->parent=Stopper_root;
      Stopper_root=stopper;
      Sum_Stopper++;
}
double Kill_stopper()
{
  double ret=0;
  if(Stopper_root==NULL)
      return ret;
  Stopper_root->end=op_timer_core();
  ret=Stopper_root->end-=Stopper_root->start;
  printf("%s took: %f sec.\n",Stopper_root->name,Stopper_root->end);
  struct Stopper* t=Stopper_root;//free the stopper
  Stopper_root=Stopper_root->parent;
  free(t);
  return ret;

}
#endif

