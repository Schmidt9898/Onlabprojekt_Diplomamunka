/*

This is an example for making tiling/blocking in openMP.
The calculation purpose is to reach a lot of memory from a single kernel.

Cpu ram-> gpu vram -> compute -> gpu vram- > cpu ram
*/

//stopper functions defined here, you may choose not to use them with -D NO_TIME
void Spawn_stopper(char *name);
double Kill_stopper();

#ifndef L3D
#define L1D
#endif

#include "stdlib.h"
#include <stdio.h>
#include <math.h>
#include "omp.h"

//#define THREADLIMIT 256

/* constans méretek */
/*
#define X 800
#define Y 800
#define Z 800
*/
const long sizex = 880; //If this is not constant, will cause a segfault in runtime with clang-12
const long sizey = 880; //and clang-14 if -D
const long sizez = 880;

struct dataobj{void *data;};

int main(int argc, char **argv)
{


#ifdef L1D
  printf("compiled with 1 dimensional arrays.\n");
  #define Ddim(arr,x,y,z) arr[(z) + (y)*sizez + (x)*sizez * sizey]

#elif L3D
  printf("compiled with 3 dimensional arrays.\n");
  #define Ddim(arr,x,y,z) arr ## _[x][y][z]
#else
  #pragma message "Dimension was not defined use one of L1D or L3D "
#endif

#if defined(F800_2)
#define window_size 2
#define KERNEL_WINDOW2(out,x,y,z) float r10 = 1.0F/(Ddim(data1,x + 2, y + 2, z + 2)*Ddim(data1,x + 2, y + 2, z + 2)); \
Ddim(out, x + 2, y + 2, z + 2) = (r9*Ddim(data2, x + 1, y + 1, z + 1)*Ddim(data3, x + 2, y + 2, z + 2) + r10*(-r8*(-2.0F*Ddim(data3, x + 2, y + 2, z + 2)) - r8*Ddim(data4, x + 2, y + 2, z + 2)) + 4.44444e-3F*(Ddim(data3, x + 1, y + 2, z + 2) + Ddim(data3, x + 2, y + 1, z + 2) + Ddim(data3, x + 2, y + 2, z + 1) + Ddim(data3, x + 2, y + 2, z + 3) + Ddim(data3, x + 2, y + 3, z + 2) + Ddim(data3, x + 3, y + 2, z + 2)) - 2.66666673e-2F*Ddim(data3, x + 2, y + 2, z + 2))/(r8*r10 + r9*Ddim(data2, x + 1, y + 1, z + 1));
#elif defined(F800_4)
#define window_size 4
#define KERNEL_WINDOW2(out,x,y,z) float r10 = 1.0F/(Ddim(data1,x + 4, y + 4, z + 4)*Ddim(data1,x + 4, y + 4, z + 4)); \
Ddim(out, x + 4, y + 4, z + 4) = (r9*Ddim(data2, x + 1, y + 1, z + 1)*Ddim(data3, x + 4, y + 4, z + 4) + r10*(-r8*(-2.0F*Ddim(data3, x + 4, y + 4, z + 4)) - r8*Ddim(data4, x + 4, y + 4, z + 4)) + 3.70370379e-4F*(-Ddim(data3, x + 2, y + 4, z + 4) - Ddim(data3, x + 4, y + 2, z + 4) - Ddim(data3, x + 4, y + 4, z + 2) - Ddim(data3, x + 4, y + 4, z + 6) - Ddim(data3, x + 4, y + 6, z + 4) - Ddim(data3, x + 6, y + 4, z + 4)) + 5.92592607e-3F*(Ddim(data3, x + 3, y + 4, z + 4) + Ddim(data3, x + 4, y + 3, z + 4) + Ddim(data3, x + 4, y + 4, z + 3) + Ddim(data3, x + 4, y + 4, z + 5) + Ddim(data3, x + 4, y + 5, z + 4) + Ddim(data3, x + 5, y + 4, z + 4)) - 3.33333341e-2F*Ddim(data3, x + 4, y + 4, z + 4))/(r8*r10 + r9*Ddim(data2, x + 1, y + 1, z + 1));
#elif defined(F800_8)
#define window_size 8
#define KERNEL_WINDOW2(out,x,y,z) float r10 = 1.0F/(Ddim(data1,x + 8, y + 8, z + 8)*Ddim(data1,x + 8, y + 8, z + 8)); \
Ddim(out, x + 8, y + 8, z + 8) = (r9*Ddim(data2, x + 1, y + 1, z + 1)*Ddim(data3, x + 8, y + 8, z + 8) + r10*(-r8*(-2.0F*Ddim(data3, x + 8, y + 8, z + 8)) - r8*Ddim(data4, x + 8, y + 8, z + 8)) + 7.93650813e-6F*(-Ddim(data3, x + 4, y + 8, z + 8) - Ddim(data3, x + 8, y + 4, z + 8) - Ddim(data3, x + 8, y + 8, z + 4) - Ddim(data3, x + 8, y + 8, z + 12) - Ddim(data3, x + 8, y + 12, z + 8) - Ddim(data3, x + 12, y + 8, z + 8)) + 1.12874782e-4F*(Ddim(data3, x + 5, y + 8, z + 8) + Ddim(data3, x + 8, y + 5, z + 8) + Ddim(data3, x + 8, y + 8, z + 5) + Ddim(data3, x + 8, y + 8, z + 11) + Ddim(data3, x + 8, y + 11, z + 8) + Ddim(data3, x + 11, y + 8, z + 8)) + 8.8888891e-4F*(-Ddim(data3, x + 6, y + 8, z + 8) - Ddim(data3, x + 8, y + 6, z + 8) - Ddim(data3, x + 8, y + 8, z + 6) - Ddim(data3, x + 8, y + 8, z + 10) - Ddim(data3, x + 8, y + 10, z + 8) - Ddim(data3, x + 10, y + 8, z + 8)) + 7.11111128e-3F*(Ddim(data3, x + 7, y + 8, z + 8) + Ddim(data3, x + 8, y + 7, z + 8) + Ddim(data3, x + 8, y + 8, z + 7) + Ddim(data3, x + 8, y + 8, z + 9) + Ddim(data3, x + 8, y + 9, z + 8) + Ddim(data3, x + 9, y + 8, z + 8)) - 3.79629639e-2F*Ddim(data3, x + 8, y + 8, z + 8))/(r8*r10 + r9*Ddim(data2, x + 1, y + 1, z + 1));
#elif defined(F800_16)
#define window_size 16
#define KERNEL_WINDOW2(out,x,y,z) float r10 = 1.0F/(Ddim(data1,x + 16, y + 16, z + 16)*Ddim(data1,x + 16, y + 16, z + 16)); \
Ddim(out, x + 16, y + 16, z + 16) = (r9*Ddim(data2, x + 1, y + 1, z + 1)*Ddim(data3, x + 16, y + 16, z + 16) + r10*(-r8*(-2.0F*Ddim(data3, x + 16, y + 16, z + 16)) - r8*Ddim(data4, x + 16, y + 16, z + 16)) + 1.07916777e-8F*(-Ddim(data3, x + 8, y + 16, z + 16) - Ddim(data3, x + 16, y + 8, z + 16) - Ddim(data3, x + 16, y + 16, z + 8) - Ddim(data3, x + 16, y + 16, z + 24) - Ddim(data3, x + 16, y + 24, z + 16) - Ddim(data3, x + 24, y + 16, z + 16)) + 2.25524041e-7F*(Ddim(data3, x + 9, y + 16, z + 16) + Ddim(data3, x + 16, y + 9, z + 16) + Ddim(data3, x + 16, y + 16, z + 9) + Ddim(data3, x + 16, y + 16, z + 23) + Ddim(data3, x + 16, y + 23, z + 16) + Ddim(data3, x + 23, y + 16, z + 16)) + 2.30222458e-6F*(-Ddim(data3, x + 10, y + 16, z + 16) - Ddim(data3, x + 16, y + 10, z + 16) - Ddim(data3, x + 16, y + 16, z + 10) - Ddim(data3, x + 16, y + 16, z + 22) - Ddim(data3, x + 16, y + 22, z + 16) - Ddim(data3, x + 22, y + 16, z + 16)) + 1.54709492e-5F*(Ddim(data3, x + 11, y + 16, z + 16) + Ddim(data3, x + 16, y + 11, z + 16) + Ddim(data3, x + 16, y + 16, z + 11) + Ddim(data3, x + 16, y + 16, z + 21) + Ddim(data3, x + 16, y + 21, z + 16) + Ddim(data3, x + 21, y + 16, z + 16)) + 7.85634138e-5F*(-Ddim(data3, x + 12, y + 16, z + 16) - Ddim(data3, x + 16, y + 12, z + 16) - Ddim(data3, x + 16, y + 16, z + 12) - Ddim(data3, x + 16, y + 16, z + 20) - Ddim(data3, x + 16, y + 20, z + 16) - Ddim(data3, x + 20, y + 16, z + 16)) + 3.35203899e-4F*(Ddim(data3, x + 13, y + 16, z + 16) + Ddim(data3, x + 16, y + 13, z + 16) + Ddim(data3, x + 16, y + 16, z + 13) + Ddim(data3, x + 16, y + 16, z + 19) + Ddim(data3, x + 16, y + 19, z + 16) + Ddim(data3, x + 19, y + 16, z + 16)) + 1.38271608e-3F*(-Ddim(data3, x + 14, y + 16, z + 16) - Ddim(data3, x + 16, y + 14, z + 16) - Ddim(data3, x + 16, y + 16, z + 14) - Ddim(data3, x + 16, y + 16, z + 18) - Ddim(data3, x + 16, y + 18, z + 16) - Ddim(data3, x + 18, y + 16, z + 16)) + 7.90123476e-3F*(Ddim(data3, x + 15, y + 16, z + 16) + Ddim(data3, x + 16, y + 15, z + 16) + Ddim(data3, x + 16, y + 16, z + 15) + Ddim(data3, x + 16, y + 16, z + 17) + Ddim(data3, x + 16, y + 17, z + 16) + Ddim(data3, x + 17, y + 16, z + 16)) - 4.07312557e-2F*Ddim(data3, x + 16, y + 16, z + 16))/(r8*r10 + r9*Ddim(data2, x + 1, y + 1, z + 1));
#else
#error "invalid space order"
#endif



// const int blocksize_x = 8;
// const int blocksize_y = 4;
// const int blocksize_z = 32;
// const int window_size = 8;
//#define FORBLOCKED
//#define FORNAIV
//#define FORBLOCKTILLED
//#define FORTILLED



#if defined(FORBLOCKED) || defined(FORBLOCKTILLED) || defined(FORTILLED)
printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);
#endif


const size_t meret = sizex * sizey * sizez;
printf("memory size needed: %lu , %f Gb \n", meret, 7*meret * sizeof(float) / 1e9f);




//Spawn_stopper("offload and memory managment");

//data and offloading OpenMP

//this we will use on the cpu
float * out = (float *)malloc(meret * sizeof(float));
float * out2 = (float *)malloc(meret * sizeof(float));

float * __restrict data = (float *)malloc(meret * sizeof(float));
float * __restrict data1 = (float *)malloc(meret * sizeof(float));
float * __restrict data2 = (float *)malloc(meret * sizeof(float));
float * __restrict data3 = (float *)malloc(meret * sizeof(float));
float * __restrict data4 = (float *)malloc(meret * sizeof(float));
#pragma omp target data map(to : data[0:meret]) \
            map(to : data1[0:meret]) \
            map(to : data2[0:meret]) \
            map(to : data3[0:meret]) \
            map(to : data4[0:meret]) \
            map(from  : out[0:meret])  \
            map(from  : out2[0:meret])
{


/*float(*__restrict data_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data;
float(*__restrict data1_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data1;
float(*__restrict data2_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data2;
float(*__restrict data3_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data3;
float(*__restrict data4_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data4;
float(*__restrict out_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out;
float(*__restrict out2_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out2;
*/

//init aka zeroing
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
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
#ifdef FORNAIV
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
for (int i = 0; i < 21; i++) {
if (i == 11) Spawn_stopper("Kernel 0");
#ifdef LOOP
#pragma omp target teams loop collapse(3)
#else
#ifdef use_simd
#pragma omp target teams distribute collapse(2)
#else
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#endif
#endif
  for (int x = 0; x < sizex - 2*window_size; x++)
  {
    for (int y = 0; y < sizey - 2*window_size; y++)
    {
#ifdef use_simd
	    #pragma omp parallel for simd simdlen(32)
#endif
      for (int z = 0; z < sizez - 2*window_size; z++)
      {
        //kernel start
         KERNEL_WINDOW2(out,x,y,z)
      }
    }
  }
}
Kill_stopper();
#endif

#ifdef FORTILLED
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
for (int i = 0; i < 21; i++) {
if (i == 11) Spawn_stopper("Kernel 1");
// #pragma omp target teams
// #pragma omp distribute parallel for collapse(3)
#pragma omp target teams distribute parallel for collapse(3)
#pragma omp tile sizes(blocksize_x,blocksize_y,blocksize_z)
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
}
Kill_stopper();
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FORBLOCKED
for (int i = 0; i < 21; i++) {
if (i == 11) Spawn_stopper("Kernel 2");
#ifdef LOOP
#pragma omp target teams loop collapse(3) thread_limit(THREADLIMIT)
#else
#ifdef use_simd
#pragma omp target teams distribute collapse(3)
#else
#pragma omp target teams distribute collapse(3) thread_limit(THREADLIMIT) //deviceptr(data,data1,data2,data3,data4,data_,data_1,data_2,data_3,data_4)
#endif
#endif
  for (int x = 0; x < sizex - 2*window_size; x += blocksize_x)
    for (int y = 0; y < sizey - 2*window_size; y += blocksize_y)
      for (int z = 0; z < sizez - 2*window_size; z += blocksize_z)
      {
#ifdef LOOP
  #pragma omp loop collapse(3) bind(parallel)
#else
#ifdef use_simd
  #pragma omp parallel for simd simdlen(32) //shared(out2_,data_)

	for (int c = 0; c < blocksize_x*blocksize_y*blocksize_z; c++) {
		int bx = x + c/blocksize_x*blocksize_y;
		int by = y + c/blocksize_x;
		int bz = z + c%blocksize_z;
#else
  #pragma omp parallel for collapse(3) //shared(out2_,data_)
#endif
#endif
#ifndef use_simd
        for (int bx = x; bx < x + blocksize_x; bx++)
          for (int by = y; by < y + blocksize_y; by++)
            for (int bz = z; bz < z + blocksize_z; bz++)
            {
#endif
              if (bx < sizex - 2*window_size &&
                  by < sizey - 2*window_size &&
                  bz < sizez - 2*window_size) {

                  KERNEL_WINDOW2(out,bx,by,bz);

              }
            }
      }
}
Kill_stopper();
#endif

#ifdef FORBLOCKTILLED
for (int i = 0; i < 21; i++) {
if (i == 11) Spawn_stopper("Kernel 3");
#pragma omp target teams distribute collapse(3) thread_limit(THREADLIMIT) //deviceptr(data,data1,data2,data3,data4,data_,data_1,data_2,data_3,data_4)
  for (int x = 0; x < sizex - 2*window_size; x += blocksize_x)
    for (int y = 0; y < sizey - 2*window_size; y += blocksize_y)
      for (int z = 0; z < sizez - 2*window_size; z += blocksize_z)
      {
  #pragma omp parallel for collapse(3)
  #pragma omp tile sizes(tilesize_x,tilesize_y,tilesize_z)
        for (int bx = x; bx < x + blocksize_x; bx++)
          for (int by = y; by < y + blocksize_y; by++)
            for (int bz = z; bz < z + blocksize_z; bz++)
            {
              if (bx < sizex - 2*window_size &&
                  by < sizey - 2*window_size &&
                  bz < sizez - 2*window_size) {

                  KERNEL_WINDOW2(out,bx,by,bz);

              }
            }
      }
}
Kill_stopper();
#endif

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef EXPORTDATA
{
	FILE *write_ptr;
	write_ptr = fopen("./out.bin","wb");  // w for write, b for binary
	size_t arrsize = meret * sizeof(float);
	fwrite((char*)out,arrsize,1,write_ptr); // write bytes from our buffer

}
#endif



free(out);
free(data);
free(data1);
free(data2);
free(data3);
free(data4);
free(out2);

printf("Done.");

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
      // printf("%s ID:%d started->\n",name,Sum_Stopper);
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
  printf("%s %f sec.\n",Stopper_root->name,Stopper_root->end);
  struct Stopper* t=Stopper_root;//free the stopper
  Stopper_root=Stopper_root->parent;
  free(t);
  return ret;

}
#endif
