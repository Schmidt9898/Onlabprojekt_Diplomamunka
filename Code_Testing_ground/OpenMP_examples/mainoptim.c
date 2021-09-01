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
	#ifdef LOMP
	#include "omp.h"
	#elif LACC
	#include "openacc.h"
	#endif
/* constans méretek */
const int sizex = 800; //If this is not constant, will cause a segfault in runtime with clang-12
const int sizey = 800; //and clang-14 if -D
const int sizez = 800;

struct dataobj{void *data;};

int main(int argc, char **argv)
{

	#ifdef LOMP
	printf("Hi this is openMP tiling testing..\n");
	#elif LACC
	printf("Hi this is openACC tiling testing\n");
	#else
	#pragma message "OpenMP or OpenACC was not defined use one of -LACC or LOMP "  __FILE__
	#endif
	#ifdef L1D
	printf("compiled with 1 dimensional arrays.\n");
	#define Ddim(x,y,z) [(z) + (y)*sizez + (x)*sizez * sizey]
	#define KERNEL_WINDOW(x,y,z) out Ddim(x,y,z) += \
										data Ddim(x,y,z - 4) + data Ddim(x,y,z - 3) + data Ddim(x,y,z - 2) + data Ddim(x,y,z - 1) +\
										data Ddim(x,y,z + 4) + data Ddim(x,y,z + 3) + data Ddim(x,y,z + 2) + data Ddim(x,y,z + 1) +\
										data Ddim(x,y - 4,z) + data Ddim(x,y - 3,z) + data Ddim(x,y - 2,z) + data Ddim(x,y - 1,z) +\
										data Ddim(x,y + 4,z) + data Ddim(x,y + 3,z) + data Ddim(x,y + 2,z) + data Ddim(x,y + 1,z) +\
										data Ddim(x - 4,y,z) + data Ddim(x - 3,y,z) + data Ddim(x - 2,y,z) + data Ddim(x - 1,y,z) +\
										data Ddim(x + 4,y,z) + data Ddim(x + 3,y,z) + data Ddim(x + 2,y,z) + data Ddim(x + 1,y,z);
	#else
	printf("compiled with 3 dimensional arrays.\n");
	#define Ddim(x,y,z) [x][y][z]
	#define KERNEL_WINDOW(x,y,z) out_ Ddim(x,y,z) += \
										data_ Ddim(x,y,z - 4) + data_ Ddim(x,y,z - 3) + data_ Ddim(x,y,z - 2) + data_ Ddim(x,y,z - 1) +\
										data_ Ddim(x,y,z + 4) + data_ Ddim(x,y,z + 3) + data_ Ddim(x,y,z + 2) + data_ Ddim(x,y,z + 1) +\
										data_ Ddim(x,y - 4,z) + data_ Ddim(x,y - 3,z) + data_ Ddim(x,y - 2,z) + data_ Ddim(x,y - 1,z) +\
										data_ Ddim(x,y + 4,z) + data_ Ddim(x,y + 3,z) + data_ Ddim(x,y + 2,z) + data_ Ddim(x,y + 1,z) +\
										data_ Ddim(x - 4,y,z) + data_ Ddim(x - 3,y,z) + data_ Ddim(x - 2,y,z) + data_ Ddim(x - 1,y,z) +\
										data_ Ddim(x + 4,y,z) + data_ Ddim(x + 3,y,z) + data_ Ddim(x + 2,y,z) + data_ Ddim(x + 1,y,z);
	#endif 

	

	int blocksize_x = 4;
	int blocksize_y = 8;
	int blocksize_z = 16;

	printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

	printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
	//window size
	const int window_size = 4;

	const size_t meret = sizex * sizey * sizez;
	printf("memory size needed: %lu , %f Gb \n", meret * 2, meret * 4 / 1e9f * 2);



	//Spawn_stopper("offload and memory managment");

	//data and offloading OpenMP

	//this we will use on the cpu
	float *out = (float *)malloc(meret * 4);
	//this we will not
	float *data = NULL; //4= (float*) malloc(meret*4);

	//offloading
	#ifdef LOMP
	#pragma omp target data map(alloc : data[0:meret]) \
							map(from  : out[0:meret])  
	#elif LACC
	#pragma acc data copyout(out[0:meret])
	#endif
	{

	#ifdef LACC
	float* restrict data=(float*)acc_malloc(meret*4);
	#endif

	//Kill_stopper();
	//pointer folding to 3 dimension array
	float(*__restrict data_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])data;
	float(*__restrict out_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Spawn_stopper("3d computation collapse (3)");
	#ifdef LOMP
	#pragma omp target teams
	#pragma omp distribute parallel for collapse(3)  
	#elif LACC
	#pragma acc parallel loop collapse(3) deviceptr(data_)
	#endif
		for (int x = window_size; x < sizex - window_size; x++)
		{
			for (int y = window_size; y < sizey - window_size; y++)
			{
				for (int z = window_size; z < sizez - window_size; z++)
				{
					//kernel start
					//KERNEL_WINDOW(x,y,z)
					out_[x][y][z] +=
						data_[x][y][z - 4] + data_[x][y][z - 3] + data_[x][y][z - 2] + data_[x][y][z - 1] +
						data_[x][y][z + 4] + data_[x][y][z + 3] + data_[x][y][z + 2] + data_[x][y][z + 1] +
						data_[x][y - 4][z] + data_[x][y - 3][z] + data_[x][y - 2][z] + data_[x][y - 1][z] +
						data_[x][y + 4][z] + data_[x][y + 3][z] + data_[x][y + 2][z] + data_[x][y + 1][z] +
						data_[x - 4][y][z] + data_[x - 3][y][z] + data_[x - 2][y][z] + data_[x - 1][y][z] +
						data_[x + 4][y][z] + data_[x + 3][y][z] + data_[x + 2][y][z] + data_[x + 1][y][z];
					
				}
			}
		}
	Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#ifdef LOMP
	Spawn_stopper("3d tiling OpenMP");
	#pragma omp target teams distribute collapse(3) thread_limit(512)
		for (int x = window_size; x < sizex - window_size; x += blocksize_x)
			for (int y = window_size; y < sizey - window_size; y += blocksize_y)
				for (int z = window_size; z < sizez - window_size; z += blocksize_z)
				{
		#pragma omp parallel for collapse(3)
					for (int bx = x; bx < x + blocksize_x; bx++)
						for (int by = y; by < y + blocksize_y; by++)
							for (int bz = z; bz < z + blocksize_z; bz++)
							{
								if (bx < sizex - window_size &&
									by < sizey - window_size &&
									bz < sizez - window_size)
								{
									KERNEL_WINDOW(bx,by,bz);
								}
							}
				}
	#elif LACC
	Spawn_stopper("3d tiling OpenACC");
	#pragma acc parallel loop deviceptr(data_) tile(blocksize_x,blocksize_y,blocksize_z)
	for(int x=window_size;x<sizex-window_size;x++)
    {
        for(int y=window_size;y<sizey-window_size;y++)
        {
            for(int z=window_size;z<sizez-window_size;z++)
            {
                KERNEL_WINDOW(x,y,z)
            }        
        }
    }
	#endif

	Kill_stopper();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Spawn_stopper("back to ram");
	}
	//Kill_stopper();

	free(out);

	return 0;
}

//END of example

//If you don't want to compile this code use -D NO_TIME in compile option.

//This code is for the stopper not part of the example
//Do not change this code
//The example code if up

#ifdef NO_TIME
void Spawn_stopper(char *name){};
double Kill_stopper(){};
#else
#include "stopper.h"
#endif
