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
#include <cuda.h>
#include <cuda_runtime.h>
//#include "omp.h"

//This is a little wrapper that checks for error codes returned by CUDA API calls
#define cudaCheck(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

//#define THREADLIMIT 256

/* constans méretek */
/*
#define X 800
#define Y 800
#define Z 800
*/

const long sizex = 880;
const long sizey = 880;
const long sizez = 880;

struct dataobj{void *data;};


__constant__ float r8;
__constant__ float r9;



#ifdef L1D
  #define Ddim(arr,x,y,z) arr[(z) + (y)*sizez + (x)*sizez * sizey]

#elif L3D
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









__global__ void cuda_section0(float * data,float * data1,float * data2,float * data3,float * data4,float * out){
	//printf("section0 gpu\n");

  int z = threadIdx.x + blockIdx.x * blocksize_z + 0;
  int y = threadIdx.y + blockIdx.y * blocksize_y + 0;
  int x = threadIdx.z + blockIdx.z * blocksize_x + 0; 
	//if (x <= x_M && y <= y_M && z <= z_M){
		if (x < 880 - 2*window_size && y < 880 - 2*window_size && z < 880 - 2*window_size){		
			KERNEL_WINDOW2(out,x,y,z);
	}
}
__global__ void cuda_init(float * data,float * data1,float * data2,float * data3,float * data4,float * out){
	//printf("section0 gpu\n");

  int z = threadIdx.x + blockIdx.x * blocksize_z + 0;
  int y = threadIdx.y + blockIdx.y * blocksize_y + 0;
  int x = threadIdx.z + blockIdx.z * blocksize_x + 0;
		//kernel start
		
		if (x < 880 && y < 880 && z < 880){		
		Ddim(data,x,y,z) = 1.0;
		Ddim(data1,x,y,z) = 1.0;
		Ddim(data2,x,y,z) = 1.0;
		Ddim(data3,x,y,z) = 1.0;
		Ddim(data4,x,y,z) = 1.0;
		Ddim(out,x,y,z) = 0.0;
		//Ddim(out2,x,y,z) = 0.0;
	}
}

extern "C" void cuda_enter_data(float** device_ptr,float* data,size_t size)
{
	cudaCheck(cudaMalloc(device_ptr, size*sizeof(float)));
	cudaCheck(cudaMemcpy(*device_ptr, data,size*sizeof(float), cudaMemcpyHostToDevice));
}
extern "C" void cuda_exit_data(float* device_ptr){
	cudaCheck(cudaFree(device_ptr));
}
extern "C" void cuda_update_data_from(float* device_ptr,float* data,size_t size){
	//print0<<<1,1>>>(device_ptr);
	cudaCheck(cudaMemcpy(data,device_ptr,size*sizeof(float), cudaMemcpyDeviceToHost));
	cudaCheck(cudaFree(device_ptr));
	//printf("cpu %f,\n",(data)[1518766805]);
}


int main(int argc, char **argv)
{


#ifdef L1D
  printf("compiled with 1 dimensional arrays.\n");
#elif L3D
  printf("compiled with 3 dimensional arrays.\n");
#else
  #pragma message "Dimension was not defined use one of L1D or L3D " 
#endif


#if defined(FORBLOCKED) || defined(FORBLOCKTILLED) || defined(FORTILLED)
printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);
#endif
const size_t meret = sizex * sizey * sizez;
printf("memory size needed: %lu , %f Gb \n", meret, 7*meret * sizeof(float) / 1e9f);


float * cpu_out = (float *)malloc(meret * sizeof(float));
float * cpu_out2 = (float *)malloc(meret * sizeof(float));

float * out;
float * out2;

float * data;
float * data1;
float * data2;
float * data3;
float * data4;

cudaCheck(cudaMalloc(&data, meret*sizeof(float)));
cudaCheck(cudaMalloc(&data1, meret*sizeof(float)));
cudaCheck(cudaMalloc(&data2, meret*sizeof(float)));
cudaCheck(cudaMalloc(&data3, meret*sizeof(float)));
cudaCheck(cudaMalloc(&data4, meret*sizeof(float)));
cudaCheck(cudaMalloc(&out, meret*sizeof(float)));
cudaCheck(cudaMalloc(&out2, meret*sizeof(float))); //nem fontos amúgy
printf("gpu : %p\n",data);

float dt = 1.816000;
float r8_ = 1.0F/(dt*dt);
float r9_ = 1.0F/dt;
cudaCheck(cudaMemcpyToSymbol(r8,&r8_, sizeof(float)));
cudaCheck(cudaMemcpyToSymbol(r9,&r9_, sizeof(float)));

//#pragma omp target data map(alloc : data[0:meret]) \
//			map(alloc : data1[0:meret]) \
//			map(alloc : data2[0:meret]) \
//			map(alloc : data3[0:meret]) \
//			map(alloc : data4[0:meret]) \
//			map(from  : out[0:meret])  \
//			map(from  : out2[0:meret])
{


{
	//init aka zeroing
	dim3 threads(blocksize_z,blocksize_y,blocksize_x);
	dim3 blocks((880)/blocksize_z+1,(880)/blocksize_y+1,(880)/blocksize_x+1);
	cuda_init<<<blocks,threads>>>(data,data1,data2,data3,data4,out);
	cudaCheck(cudaPeekAtLastError());
	cudaCheck(cudaDeviceSynchronize());
}


dim3 threads(blocksize_z,blocksize_y,blocksize_x);
dim3 blocks((880 - 2*window_size)/blocksize_z+1,(880 - 2*window_size)/blocksize_y+1,(880 - 2*window_size)/blocksize_x+1);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
for (int i = 0; i < 21; i++) {
	if (i == 11) 
	{
		cudaCheck(cudaDeviceSynchronize());
		Spawn_stopper("Kernel 0");	
	}
	cuda_section0<<<blocks,threads>>>(data,data1,data2,data3,data4,out);
	cudaCheck(cudaPeekAtLastError());
	
}
cudaCheck(cudaPeekAtLastError());
cudaCheck(cudaDeviceSynchronize());
Kill_stopper();

}
//printf("gpu : %p\n",data);
//printf("gpu : %p\n",data1);

cudaCheck(cudaFree(data));
cudaCheck(cudaFree(data1));
cudaCheck(cudaFree(data2));
cudaCheck(cudaFree(data3));
cudaCheck(cudaFree(data4));
cudaCheck(cudaMemcpy(cpu_out,out,meret*sizeof(float), cudaMemcpyDeviceToHost));
cudaCheck(cudaFree(out));
printf("cudafree done.\n");
//cuda_update_data_from(out,cpu_out,meret);
//cuda_update_data_from(out2,cpu_out2,meret);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef EXPORTDATA
{
	FILE *write_ptr;
	write_ptr = fopen("./out.bin","wb");  // w for write, b for binary
	size_t arrsize = meret * sizeof(float);
	fwrite((char*)cpu_out,arrsize,1,write_ptr); // write bytes from our buffer

}
#endif

free(cpu_out);
free(cpu_out2);

printf("Done.\n");

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


// proper definition of the stopper
struct Stopper
{
	/* data */
	struct Stopper* parent; //am i a child
	double start,end;	   //preaty self explain
	char name[256];		  //20 caracter for a name is enought //update it is few week later and 256 because turns out it isn't enought, this took like 3 painfull hours to find out.
	
};
//typedef struct Stopper Stopper();

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




void Spawn_stopper(char* name)
{
	  struct Stopper * stopper= (Stopper *)malloc(sizeof(struct Stopper));
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

