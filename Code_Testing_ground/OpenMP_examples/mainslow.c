/*

This is an example for making tiling/blocking in openMP.
The calculation purpose is to reach a lot of memory from a single kernel.

Cpu ram-> gpu vram -> compute -> gpu vram- > cpu ram
*/

#include "stdlib.h"
#include <stdio.h>

#ifdef NO_TIME
void Spawn_stopper(char* name){};
double Kill_stopper(){};
#else
#include "stopper.h"
#endif

/* constans méretek */
const int sizex = 800; //If this is not constant, will cause a segfault in runtime.
const int sizey = 800;
const int sizez = 800;

struct dataobj
{
	void *data;
};

int main(int argc, char **argv)
{
//Stopper_Filemode = false; //Needed for the timer.


printf("Hi this is openMP testing..\n\n");


	//The block size can be overridden by arguments.
	int blocksize_x = 32;
	int blocksize_y = 8;
	int blocksize_z = 4;
	if (argc >= 4)
	{
		blocksize_x = atoi(argv[1]);
		blocksize_y = atoi(argv[2]);
		blocksize_z = atoi(argv[3]);
	}
	printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

	printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
	//window size
	const int window_size = 4;

	const size_t meret = sizex * sizey * sizez;
	printf("memory size needed: %lu , %f Gb \n", meret*2, meret * 4 / 1e9f * 2);

	Spawn_stopper("offload and memory managment");

	//data and offloading OpenMP

	//this we will use on the cpu
	float *out = (float *)malloc(meret * 4);
	//this we will not
	float *data = NULL; //4= (float*) malloc(meret*4);

#pragma omp target data map(alloc                                                                              \
							: data [0:meret])                                                                  \
	map(from                                                                                                   \
		: out [0:meret])                                                                                       \
		map(to                                                                                                 \
			: sizex) map(to                                                                                    \
						 : sizey) map(to                                                                       \
									  : sizez) map(to                                                          \
												   : blocksize_x) map(to                                       \
																	  : blocksize_y) map(to                    \
																						 : blocksize_z) map(to \
																											: window_size)
	{

		Kill_stopper();
		//pointer folding to 3 dimension array
		float(*__restrict data_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])data;
		float(*__restrict out_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out;

		//printf("size meret %d \n",meret);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Computation on gpu with simple parallel for collapse(3)
*/
Spawn_stopper("3d computation");
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
		for (int x = window_size; x < sizex - window_size; x++)
		{
			for (int y = window_size; y < sizey - window_size; y++)
			{
				for (int z = window_size; z < sizez - window_size; z++)
				{
					//kernel start
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
In this example we simulate Tile or blocking methods by manualy distribute the threads.
This is a small scale testing to see what is happening.
*/
		//omp_set_num_threads(1024);
		//omp_get_max_threads();
		//printf("omp_get_max_threads = %d\n",omp_get_max_threads());
Spawn_stopper("3 team loop of 3 thread loops");
#pragma omp target teams distribute collapse(2) thread_limit(1024) // run on the teamleaders
/*
we set the thread limit to 1024 and this sets the max thread variable but we dont get 
that many thread / block. We only get 992. 32 les than the max. It is like all the teamleader thread dont count.
Also this is not the final thread count either, thread count depends on the register/thread value. but this is not knowed from 
here.
*/
for (int x = 0; x < 2; x += 1)
{
	for (int y = 0; y < 2; y += 1)
		{
			printf("just a teamleader run this command. The max thread_size is %d \n", omp_get_max_threads());
			//data_[x][y][0]=1; //this cause compiler crash with clang-14
		#pragma omp parallel for collapse(2) // run on the threads
		for (int i = 0; i < 2; i += 1)
			{
			for (int j = 0; j < 2; j += 1)
				{
					printf("omp_get_num_threads = %d\n", omp_get_num_threads());
					int bid = omp_get_team_num();
					int tid = omp_get_thread_num();
					printf("bid %d, tid %d xyij: %d %d %d %d\n", bid, tid, x, y, i, j);
						//data_[0][0][0]=1; // this cause segfault when compiled with clang-14
						/*
					it also couse segfault when compiled with clang-12 or clang-11, but it will  only cause warnings 
					(variable length array folded to constant array as an extension )
					if size static (knowed) at compile time.
					*/
				}
			}
		}
}
Kill_stopper();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
In this example we simulate Tile or blocking methods by manualy distribute the threads.
This part of the code only works with clang-12 or clang-11 in case of static sizexyz parameter.

In clang-14 we can get segmentation fault or nothing but in that case out_ and data_ will not be modified.

In Nvidia visual profiler. If we examine the kernel we see that the blocksize is 640,1,1
and the Occupancy is ~ 31.2 
in the program omp_get_num_threads() is 992.

*/

Spawn_stopper("3d tiling with omp teams");
#pragma omp target teams distribute collapse(3) thread_limit(1024)
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
									
									out_[bx][by][bz] +=
										data_[bx][by][bz - 4] + data_[bx][by][bz - 3] + data_[bx][by][bz - 2] + data_[bx][by][bz - 1] +
										data_[bx][by][bz + 4] + data_[bx][by][bz + 3] + data_[bx][by][bz + 2] + data_[bx][by][bz + 1] +
										data_[bx][by - 4][bz] + data_[bx][by - 3][bz] + data_[bx][by - 2][bz] + data_[bx][by - 1][bz] +
										data_[bx][by + 4][bz] + data_[bx][by + 3][bz] + data_[bx][by + 2][bz] + data_[bx][by + 1][bz] +
										data_[bx - 4][by][bz] + data_[bx - 3][by][bz] + data_[bx - 2][by][bz] + data_[bx - 1][by][bz] +
										data_[bx + 4][by][bz] + data_[bx + 3][by][bz] + data_[bx + 2][by][bz] + data_[bx + 1][by][bz];
								}
							}
				}

Kill_stopper();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

Spawn_stopper("back to ram");
}
Kill_stopper();

free(out);

return 0;
}
