/******************************************************************************************
*                                                                                         *
* This code was copied from an example for making tiling/blocking in openMP and openACC.  *
* The calculation purpose is to reach a lot of memory from a single kernel.               * 
* This test will help locate the memory problem with 3d indexing                          *
*                                                                                         *
*                                                                                         *
*******************************************************************************************/

//stopper functions defined here, you may choose not to use them with -D NO_TIME
void Spawn_stopper(char *name);
double Kill_stopper();

#include "stdlib.h"
#include <stdio.h>
#include <math.h>
#ifdef LOMP
#include "omp.h"
#define X 5
#define Y 5
#define Z 5
#define TSIZE 125 
#elif LACC
#include "openacc.h"
#define X 5
#define Y 5
#define Z 5
//#define TSIZE not matter
#endif
/* constans méretek */


const int sizex = 10; //If this is not constant, will cause a segfault in runtime with clang-12
const int sizey = 10; //and clang-14 if -D
const int sizez = 10;

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



const int blocksize_x = 4;
const int blocksize_y = 8;
const int blocksize_z = 32;

printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
//window size
const int window_size = 4;

const size_t meret = sizex * sizey * sizez;
printf("memory size needed: %lu , %f Gb \n", meret * 2, meret * 4 / 1e9f * 2);



//Spawn_stopper("offload and memory managment");

//data and offloading OpenMP

//this we will use on the cpu
float * out = (float *)malloc(meret * 4);
float * out2 = (float *)malloc(meret * 4);

#ifdef LOMP
float * data = NULL;
#pragma omp target data map(alloc : data[0:meret]) \
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
float(*__restrict out_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out;
float(*__restrict out2_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out2;

/*
#pragma omp target team
{
data_=data;
out_ = out;
out2_ = out2;
}
*/

//init aka zeroing
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#elif LACC
#pragma acc parallel loop collapse(3) deviceptr(data,data_)
#endif
  for (int x = 0; x < sizex ; x++)
  {
    for (int y = 0; y < sizey ; y++)
    {
      for (int z = 0; z < sizez ; z++)
      {
        //kernel start
        Ddim(data,x,y,z) = 1.0;
        Ddim(out,x,y,z) = 0.0;
        Ddim(out2,x,y,z) = 0.0;
      }
    }
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Spawn_stopper("3d computation collapse (3)");
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#elif LACC
#pragma acc parallel loop collapse(3) deviceptr(data,data_)
#endif
  for (int x = window_size; x < sizex - window_size; x++)
  {
    for (int y = window_size; y < sizey - window_size; y++)
    {
      for (int z = window_size; z < sizez - window_size; z++)
      {
        //kernel start
         KERNEL_WINDOW(out,x,y,z)
         printf("On the GPU from collapse3 %p %p %p\n",data_,out_,out2_);
      }
    }
  }
Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*if(out2_==out2)
printf("pointer: %x   %x\n",out2_,out2);
else
printf("asd\n");*/
/*
#pragma omp target teams distribute parallel for
{
  for(int i=0;i<1;i++)
  printf("asda\n");
}
*/

printf("\nOn the CPU: %p %p %p\n\n",data_,out_,out2_);

#ifdef LOMP
Spawn_stopper("3d tiling OpenMP");
#pragma omp target teams distribute collapse(3) thread_limit(512)
  for (int x = window_size; x < sizex - window_size; x += blocksize_x)
    for (int y = window_size; y < sizey - window_size; y += blocksize_y)
      for (int z = window_size; z < sizez - window_size; z += blocksize_z)
      {
        
				  //printf("On the GPU %p %p %p\n",data_,out_,out2_);
  #pragma omp parallel for collapse(3) shared(out2_,data_)
        for (int bx = x; bx < x + blocksize_x; bx++)
          for (int by = y; by < y + blocksize_y; by++)
            for (int bz = z; bz < z + blocksize_z; bz++)
            {
              if (bx < sizex - window_size &&
                by < sizey - window_size &&
                bz < sizez - window_size)
              {
				  //data_=1;
                	KERNEL_WINDOW(out2,bx,by,bz);
				  //printf("----\n");
				  //if(out2==out2_)
						//printf("pointer out2 out2_: %p   %p\n",out2,out2_);
					//else
					//printf("nem egyenlo\n");
				//out2_[x][y][z] +=24;

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
