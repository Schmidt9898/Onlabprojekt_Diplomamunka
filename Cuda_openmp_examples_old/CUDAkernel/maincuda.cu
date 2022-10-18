/*******************************************************************************************
*                                                                                          *
*This is an example for making tiling/blocking in CUDA.                                    *
*The calculation purpose is to reach a lot of memory from a single kernel.                 *
*                                                                                          *
*Cpu ram-> gpu vram -> compute -> gpu vram- > cpu ram                                      *
*                                                                                          *
*******************************************************************************************/


//nvcc maincuda.cu -arch=sm_70 -O3 -o maincuda -DL3D

//stopper functions defined here, you may choose not to use them with -D NO_TIME
void Spawn_stopper(char *name);
double Kill_stopper();

#include "stdlib.h"
#include <stdio.h>
#include <math.h>
#include <cuda.h>
#define X 8
#define Y 4
#define Z 32


#define SIZEX 800 //On the gpu pointer folding is not allowed with variable size
#define SIZEY 800 //this has to be knowed in compile time
#define SIZEZ 800 //is this bad, for every scale you have to recompile


const int sizex = SIZEX; //If this is not constant, will cause a segfault in runtime with clang-12
const int sizey = SIZEY; //and clang-14 if -D
const int sizez = SIZEZ;// no it will not anymore SIZEXYZ was the answer
                        //BUT if this is not constatn the program will be slower

struct dataobj{void *data;};


#ifdef L1D
  #define Ddim(arr,x,y,z) arr[(z) + (y)*SIZEZ + (x)*SIZEZ * SIZEY]
#elif L3D
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




__global__ void init(
#ifdef L1D
  float * __restrict__ data, float * __restrict__ out, float * __restrict__ out2
#else
  float(*__restrict__ data_)[SIZEY][SIZEZ],
  float(*__restrict__ out_)[SIZEY][SIZEZ],
  float(*__restrict__ out2_)[SIZEY][SIZEZ]
#endif
  ) {
  int z = threadIdx.x + blockIdx.x * blockDim.x;
  int y = threadIdx.y + blockIdx.y * blockDim.y;
  int x = threadIdx.z + blockIdx.z * blockDim.z;

  if (x < SIZEX && y < SIZEY && z < SIZEZ) {
    Ddim(data,x,y,z) = 1.0f;
    Ddim(out,x,y,z) = 1.0f;
    Ddim(out2,x,y,z) = 1.0f;
  }
}

__global__ void simple(int window_size,
#ifdef L1D
  float * __restrict__ data, float * __restrict__ out
#else
  float(*__restrict__ data_)[SIZEY][SIZEZ],
  float(*__restrict__ out_)[SIZEY][SIZEZ]
#endif
  ) {
  int z = threadIdx.x + blockIdx.x * blockDim.x + window_size;
  int y = threadIdx.y + blockIdx.y * blockDim.y + window_size;
  int x = threadIdx.z + blockIdx.z * blockDim.z + window_size;

  if (x < SIZEX-window_size && y < SIZEY-window_size && z < SIZEZ-window_size) {
    KERNEL_WINDOW(out,x,y,z)
    //printf("xyz %d %d %d  out = %f\n",x,y,z,Ddim(out,x,y,z));
  }
}

int main(int argc, char **argv)
{


printf("Hi this is CUDA tiling test, \n");

#ifdef L1D
  printf("compiled with 1 dimensional arrays.\n");
#elif L3D
  printf("compiled with 3 dimensional arrays.\n");
#else
  #pragma message "Dimension was not defined use one of L1D or L3D "
#endif


const int blocksize_x = X;// if this is not constant the program will be slower
const int blocksize_y = Y;
const int blocksize_z = Z;

printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
//window size
const int window_size = 4;

const size_t meret = sizex * sizey * sizez;
printf("memory size needed: %lu , %f Gb \n", meret * 2, meret * 4 / 1e9f * 2);



Spawn_stopper("offload and memory managment");

//data and offloading OpenMP

//this we will use on the cpu
float * out = (float *)malloc(meret * 4);
float * out2 = (float *)malloc(meret * 4);
float * d_out, *d_out2, *data;
cudaMalloc(&d_out, meret*4);
cudaMalloc(&d_out2, meret*4);
cudaMalloc(&data, meret*4);
Kill_stopper();
{

// pointer folding exact size must be know at compile time
float(*__restrict data_)[SIZEY][SIZEZ] =(float(*__restrict)[SIZEY][SIZEZ])data;
float(*__restrict out_)[SIZEY][SIZEZ] = (float(*__restrict)[SIZEY][SIZEZ])d_out;
float(*__restrict out2_)[SIZEY][SIZEZ] = (float(*__restrict)[SIZEY][SIZEZ])d_out2;

dim3 threads(blocksize_z,blocksize_y,blocksize_x);
dim3 blocks((SIZEZ-1)/blocksize_z+1,(SIZEY-1)/blocksize_y+1,(SIZEX-1)/blocksize_x+1);


printf("blocks dim %d %d %d \n",(SIZEZ-1)/blocksize_z+1,(SIZEY-1)/blocksize_y+1,(SIZEX-1)/blocksize_x+1);

#ifdef L1D
init<<<blocks, threads>>>(data, d_out, d_out2);
#else
init<<<blocks, threads>>>(data_, out_, out2_);
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Spawn_stopper("3d computation collapse (3)");
dim3 blocks2((SIZEZ-2*window_size-1)/blocksize_z+1,(SIZEY-2*window_size-1)/blocksize_y+1,(SIZEX-2*window_size-1)/blocksize_x+1);
#ifdef L1D
simple<<<blocks, threads>>>(window_size, data, d_out);
#else
simple<<<blocks, threads>>>(window_size, data_, out_);
#endif
Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Spawn_stopper("3d tiling CUDA");


//Kill_stopper();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

Spawn_stopper("back to ram");
}
cudaMemcpy(out, d_out, meret*4, cudaMemcpyDeviceToHost);
cudaMemcpy(out2, d_out2, meret*4, cudaMemcpyDeviceToHost);
Kill_stopper();






//validation

int good=0;
float(*__restrict data_)[SIZEY][SIZEZ] =(float(*__restrict)[SIZEY][SIZEZ])data;
float(*__restrict out_)[SIZEY][SIZEZ] = (float(*__restrict)[SIZEY][SIZEZ])out;
float(*__restrict out2_)[SIZEY][SIZEZ] = (float(*__restrict)[SIZEY][SIZEZ])out2;

for (int x = window_size; x < sizex - window_size; x++)
{
	for (int y = window_size; y < sizey - window_size; y++)
	{
		for (int z = window_size; z < sizez - window_size; z++)
		{
			if (out_[x][y][z] != 25)
			{
				printf("Validation failed\n");
				printf("out1 %f != 25 \n", out_[x][y][z]);
				good++;
				//goto validationend;
			}
		}
	}
}

/*
for (int i = 0; i < meret; i++) {
  if (out[i]!=out2[i]) {
    printf("Validation failed\n");
  printf("out1 %f != out2 %f\n",out[i],out2[i]);
    good++;
    break;
  }
}
*/
validationend:
if(!good)
  printf("Validation passed\n");

free(out);
free(out2);

return 0;
}

//END of example

//If you dont want to compile this code use -D NO_TIME in compile option.

//This code is for the stopper not part of the example, this was copied and trimed from stopper.h
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

typedef struct Stopper;// Stopper();

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
      cudaDeviceSynchronize();
      struct Stopper * stopper= (Stopper*)malloc(sizeof(struct Stopper));
      printf("%s ID:%d started->\n",name,Sum_Stopper);
      strcpy(stopper->name,name);
      stopper->start=op_timer_core();
      stopper->parent=Stopper_root;
      Stopper_root=stopper;
      Sum_Stopper++;
}
double Kill_stopper()
{
  cudaDeviceSynchronize();
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