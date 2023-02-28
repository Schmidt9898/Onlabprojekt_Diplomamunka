
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>

#include "cuda_section0.h"

//#pragma omp target teams distribute parallel for collapse(3)
//for (int x = x_m; x <= x_M; x += 1)
//{
//  for (int y = y_m; y <= y_M; y += 1)
//  {
//    for (int z = z_m; z <= z_M; z += 1)
//    {
//      float r10 = 1.0F/(vpL0(x + 16, y + 16, z + 16)*vpL0(x + 16, y + 16, z + 16));
//      uL0(t2, x + 16, y + 16, z + 16) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 16, y + 16, z + 16) + r10*(-r8*(-2.0F*uL0(t0, x + 16, y + 16, z + 16)) - r8*uL0(t1, x + 16, y + 16, z + 16)) + 1.07916777e-8F*(-uL0(t0, x + 8, y + 16, z + 16) - uL0(t0, x + 16, y + 8, z + 16) - uL0(t0, x + 16, y + 16, z + 8) - uL0(t0, x + 16, y + 16, z + 24) - uL0(t0, x + 16, y + 24, z + 16) - uL0(t0, x + 24, y + 16, z + 16)) + 2.25524041e-7F*(uL0(t0, x + 9, y + 16, z + 16) + uL0(t0, x + 16, y + 9, z + 16) + uL0(t0, x + 16, y + 16, z + 9) + uL0(t0, x + 16, y + 16, z + 23) + uL0(t0, x + 16, y + 23, z + 16) + uL0(t0, x + 23, y + 16, z + 16)) + 2.30222458e-6F*(-uL0(t0, x + 10, y + 16, z + 16) - uL0(t0, x + 16, y + 10, z + 16) - uL0(t0, x + 16, y + 16, z + 10) - uL0(t0, x + 16, y + 16, z + 22) - uL0(t0, x + 16, y + 22, z + 16) - uL0(t0, x + 22, y + 16, z + 16)) + 1.54709492e-5F*(uL0(t0, x + 11, y + 16, z + 16) + uL0(t0, x + 16, y + 11, z + 16) + uL0(t0, x + 16, y + 16, z + 11) + uL0(t0, x + 16, y + 16, z + 21) + uL0(t0, x + 16, y + 21, z + 16) + uL0(t0, x + 21, y + 16, z + 16)) + 7.85634138e-5F*(-uL0(t0, x + 12, y + 16, z + 16) - uL0(t0, x + 16, y + 12, z + 16) - uL0(t0, x + 16, y + 16, z + 12) - uL0(t0, x + 16, y + 16, z + 20) - uL0(t0, x + 16, y + 20, z + 16) - uL0(t0, x + 20, y + 16, z + 16)) + 3.35203899e-4F*(uL0(t0, x + 13, y + 16, z + 16) + uL0(t0, x + 16, y + 13, z + 16) + uL0(t0, x + 16, y + 16, z + 13) + uL0(t0, x + 16, y + 16, z + 19) + uL0(t0, x + 16, y + 19, z + 16) + uL0(t0, x + 19, y + 16, z + 16)) + 1.38271608e-3F*(-uL0(t0, x + 14, y + 16, z + 16) - uL0(t0, x + 16, y + 14, z + 16) - uL0(t0, x + 16, y + 16, z + 14) - uL0(t0, x + 16, y + 16, z + 18) - uL0(t0, x + 16, y + 18, z + 16) - uL0(t0, x + 18, y + 16, z + 16)) + 7.90123476e-3F*(uL0(t0, x + 15, y + 16, z + 16) + uL0(t0, x + 16, y + 15, z + 16) + uL0(t0, x + 16, y + 16, z + 15) + uL0(t0, x + 16, y + 16, z + 17) + uL0(t0, x + 16, y + 17, z + 16) + uL0(t0, x + 17, y + 16, z + 16)) - 4.07312557e-2F*uL0(t0, x + 16, y + 16, z + 16))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
//    }
//  }
//}

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


//#ifndef blocksize_x
//#define blocksize_x 8
//#endif
//#ifndef blocksize_y
//#define blocksize_y 4
//#endif
//#ifndef blocksize_z
//#define blocksize_z 32
//#endif

#define Ddim(arr,x,y,z) arr[(z) + (y)*SIZEZ + (x)*SIZEZ * SIZEY]

//const int sizex = SIZEX;
//const int sizey = SIZEY;
//const int sizez = SIZEZ;

#define uL0(t, x, y, z) u[(t)*x_stride0 + (x)*y_stride0 + (y)*z_stride0 + (z)]
#define dampL0(x, y, z) damp[(x)*y_stride1 + (y)*z_stride1 + (z)]
#define vpL0(x, y, z) vp[(x)*y_stride0 + (y)*z_stride0 + (z)]

__constant__ float r8;
__constant__ float r9;

__constant__ long x_stride0;
__constant__ long y_stride0;
__constant__ long z_stride0;
__constant__ long y_stride1;
__constant__ long z_stride1;
__constant__ long p_rec_stride0;
__constant__ long d_stride0;
__constant__ long p_src_stride0;


__global__ void cuda_section0(const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m,int t0,int t1,int t2,float *vp,float *u,float *damp){
	//printf("section0 gpu\n");

  int z = threadIdx.x + blockIdx.x * blocksize_z + z_m;
  int y = threadIdx.y + blockIdx.y * blocksize_y + y_m;
  int x = threadIdx.z + blockIdx.z * blocksize_x + x_m; 
	if (x <= x_M && y <= y_M && z <= z_M){
		//printf("section kernel v: %f\n",vp[0]);
		//printf("%f,",vp[(x)*y_stride0 + (y)*z_stride0 + (z)]);


		#include "../dimension_src/func_800_2.h"
		#include "../dimension_src/func_800_4.h"
		#include "../dimension_src/func_800_8.h"
		#include "../dimension_src/func_800_16.h"

		//float r10 = 1.0F/(vpL0(x + 16, y + 16, z + 16)*vpL0(x + 16, y + 16, z + 16));
    	//uL0(t2, x + 16, y + 16, z + 16) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 16, y + 16, z + 16) + r10*(-r8*(-2.0F*uL0(t0, x + 16, y + 16, z + 16)) - r8*uL0(t1, x + 16, y + 16, z + 16)) + 1.07916777e-8F*(-uL0(t0, x + 8, y + 16, z + 16) - uL0(t0, x + 16, y + 8, z + 16) - uL0(t0, x + 16, y + 16, z + 8) - uL0(t0, x + 16, y + 16, z + 24) - uL0(t0, x + 16, y + 24, z + 16) - uL0(t0, x + 24, y + 16, z + 16)) + 2.25524041e-7F*(uL0(t0, x + 9, y + 16, z + 16) + uL0(t0, x + 16, y + 9, z + 16) + uL0(t0, x + 16, y + 16, z + 9) + uL0(t0, x + 16, y + 16, z + 23) + uL0(t0, x + 16, y + 23, z + 16) + uL0(t0, x + 23, y + 16, z + 16)) + 2.30222458e-6F*(-uL0(t0, x + 10, y + 16, z + 16) - uL0(t0, x + 16, y + 10, z + 16) - uL0(t0, x + 16, y + 16, z + 10) - uL0(t0, x + 16, y + 16, z + 22) - uL0(t0, x + 16, y + 22, z + 16) - uL0(t0, x + 22, y + 16, z + 16)) + 1.54709492e-5F*(uL0(t0, x + 11, y + 16, z + 16) + uL0(t0, x + 16, y + 11, z + 16) + uL0(t0, x + 16, y + 16, z + 11) + uL0(t0, x + 16, y + 16, z + 21) + uL0(t0, x + 16, y + 21, z + 16) + uL0(t0, x + 21, y + 16, z + 16)) + 7.85634138e-5F*(-uL0(t0, x + 12, y + 16, z + 16) - uL0(t0, x + 16, y + 12, z + 16) - uL0(t0, x + 16, y + 16, z + 12) - uL0(t0, x + 16, y + 16, z + 20) - uL0(t0, x + 16, y + 20, z + 16) - uL0(t0, x + 20, y + 16, z + 16)) + 3.35203899e-4F*(uL0(t0, x + 13, y + 16, z + 16) + uL0(t0, x + 16, y + 13, z + 16) + uL0(t0, x + 16, y + 16, z + 13) + uL0(t0, x + 16, y + 16, z + 19) + uL0(t0, x + 16, y + 19, z + 16) + uL0(t0, x + 19, y + 16, z + 16)) + 1.38271608e-3F*(-uL0(t0, x + 14, y + 16, z + 16) - uL0(t0, x + 16, y + 14, z + 16) - uL0(t0, x + 16, y + 16, z + 14) - uL0(t0, x + 16, y + 16, z + 18) - uL0(t0, x + 16, y + 18, z + 16) - uL0(t0, x + 18, y + 16, z + 16)) + 7.90123476e-3F*(uL0(t0, x + 15, y + 16, z + 16) + uL0(t0, x + 16, y + 15, z + 16) + uL0(t0, x + 16, y + 16, z + 15) + uL0(t0, x + 16, y + 16, z + 17) + uL0(t0, x + 16, y + 17, z + 16) + uL0(t0, x + 17, y + 16, z + 16)) - 4.07312557e-2F*uL0(t0, x + 16, y + 16, z + 16))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
	  
		
		//printf("uL0(t2, x + 16, y + 16, z + 16) = %f \n",uL0(t2, x + 16, y + 16, z + 16));
	}

	//if(x==0 && y==0 && z==0)
	//{
		//printf("uL0(t2, x + 16, y + 16, z + 16) = %f\n",uL0(t2, x + 16, y + 16, z + 16));
	//}

}
/*
__global__ void print(const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m,int t0,int t1,int t2,float *vp,float *u,float *damp){
	//printf("section0 gpu\n");

  int z = threadIdx.x + blockIdx.x * blockDim.x + z_m;
  int y = threadIdx.y + blockIdx.y * blockDim.y + y_m;
  int x = threadIdx.z + blockIdx.z * blockDim.z + x_m; 
	if (x <= x_M && y <= y_M && z <= z_M){
		printf("kernel:\nv %f,",vpL0(x, y, z));
		printf("u %f,",uL0(t2, x, y, z));
		printf("damp %f\n",dampL0(x, y, z));
	}
}
*/

__global__ void print0(float *test){
	printf("kernel: test %f\n",test[1530428310]);
}

__global__ void print1(const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m,int t0,int t1,int t2,float *vp,float *u,float *damp){
	//printf("section0 gpu\n");
	
	int z = threadIdx.x + blockIdx.x * blocksize_z + z_m;
	int y = threadIdx.y + blockIdx.y * blocksize_y + y_m;
	int x = threadIdx.z + blockIdx.z * blocksize_x + x_m; 
	if (x <= x_M && y <= y_M && z <= z_M){
		printf("idx: %lu ,u_%f, trick %f\n",
		((t2)*x_stride0 + (x + 16)*y_stride0 + (y + 16)*z_stride0 + (z + 16)),
    	uL0(t2, x + 16, y + 16, z + 16),u[1530428310]
	);
	}

}



__global__ void cuda_test(){
	printf("section0 gpu\n");
}
/*
extern "C" void kernel_section0()
{
	printf("asdasd\n");
	cuda_test<<<2,2>>>();
	cudaDeviceSynchronize();
}
*/


extern "C" void kernel_vars(const float dt,const long x_stride0_,const long y_stride0_,const long z_stride0_,const long y_stride1_,const long z_stride1_,const long p_rec_stride0_,const long d_stride0_,const long p_src_stride0_)
{
	//TODO errors
	float r8_ = 1.0F/(dt*dt);
	float r9_ = 1.0F/dt;
	cudaCheck(cudaMemcpyToSymbol(r8,&r8_, sizeof(float)));
	cudaCheck(cudaMemcpyToSymbol(r9,&r9_, sizeof(float)));
	cudaCheck(cudaMemcpyToSymbol( x_stride0, &x_stride0_, sizeof(long) ));
	cudaCheck(cudaMemcpyToSymbol( y_stride0, &y_stride0_, sizeof(long) ));
	cudaCheck(cudaMemcpyToSymbol( z_stride0, &z_stride0_, sizeof(long) ));
	cudaCheck(cudaMemcpyToSymbol( y_stride1, &y_stride1_, sizeof(long) ));
	cudaCheck(cudaMemcpyToSymbol( z_stride1, &z_stride1_, sizeof(long) ));
	cudaCheck(cudaMemcpyToSymbol( p_rec_stride0, &p_rec_stride0_, sizeof(long) ));
	cudaCheck(cudaMemcpyToSymbol( d_stride0, &d_stride0_, sizeof(long) ));
	cudaCheck(cudaMemcpyToSymbol( p_src_stride0, &p_src_stride0_, sizeof(long) ));
}



extern "C" void kernel_section0( const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m, int t0,int t1,int t2,float *vp,float *u,float *damp)
{

	dim3 threads(blocksize_z,blocksize_y,blocksize_x);
	dim3 blocks((x_M-x_m-1)/blocksize_z+1,(y_M-y_m-1)/blocksize_y+1,(z_M-z_m-1)/blocksize_x+1);
	
	//printf("u pointer gpu: %p\n",u);
	//printf("damp pointer gpu: %p\n",damp);
	//printf("vp pointer gpu: %p\n",vp);
	
	cuda_section0<<<blocks,threads>>>(x_M,x_m,y_M,y_m,z_M,z_m,t0,t1,t2,vp,u,damp);
	
	//print1<<<blocks,threads>>>(x_M,x_m,y_M,y_m,z_M,z_m,t0,t1,t2,vp,u,damp);
	//print0<<<1,1>>>(u);
	
	
	//cudaDeviceSynchronize();
}


extern "C" void cuda_enter_data(float** device_ptr,float* data,size_t size)
{
	//printf("%f\n",data[0]);
	//printf("size: %lu\n",size);
	//getchar();
	cudaCheck(cudaMalloc(device_ptr, size*sizeof(float)));
	cudaCheck(cudaMemcpy(*device_ptr, data,size*sizeof(float), cudaMemcpyHostToDevice));
	//dim3 threads(1,1,1);
	//dim3 blocks(1,1,1);
	//print0<<<1,1>>>(*device_ptr);
	//cudaDeviceSynchronize();
	//printf("%f\n",(*device_ptr)[0]);
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



/*

int main()
{
	kernel_section0(10,10,10,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);



	return 0;
}

*/

