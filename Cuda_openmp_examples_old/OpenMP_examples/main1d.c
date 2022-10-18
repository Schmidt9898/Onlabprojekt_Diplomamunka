/*

This is an example for making tiling/blocking in openMP.
The calculation purpose is to reach a lot of memory from a single kernel.

Cpu ram-> gpu vram -> compute -> gpu vram- > cpu ram
*/


void Spawn_stopper(char* name);
double Kill_stopper();

#include "stdlib.h"
#include <stdio.h>



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
	const int blocksize_x = 4;
	const int blocksize_y = 8;
	const int blocksize_z = 32;
	/*if (argc >= 4)
	{
		blocksize_x = atoi(argv[1]);
		blocksize_y = atoi(argv[2]);
		blocksize_z = atoi(argv[3]);
	}*/
	printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

	printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
	//window size
	const int window_size = 4;

	const size_t meret = sizex * sizey * sizez;
	printf("memory size needed: %lu , %f Gb \n", meret*2, meret * 4 / 1e9f * 2);

	//Spawn_stopper("offload and memory managment");

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

		//Kill_stopper();
		//pointer folding to 3 dimension array
		float(*__restrict data_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])data;
		float(*__restrict out_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define D1(x,y,z) [(z) + (y) * sizez + (x) * sizez*sizey]


Spawn_stopper("3d tiling with omp teams");
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
									//printf("%d,%d %d\n",bx,by,bz);
									//out D1(x,y,z) ;
									out D1(bx,by,bz) += 
										data D1(bx,by,bz - 4) + data D1(bx,by,bz - 3) + data D1(bx,by,bz - 2) + data D1(bx,by,bz - 1) +
										data D1(bx,by,bz + 4) + data D1(bx,by,bz + 3) + data D1(bx,by,bz + 2) + data D1(bx,by,bz + 1) +
										data D1(bx,by - 4,bz) + data D1(bx,by - 3,bz) + data D1(bx,by - 2,bz) + data D1(bx,by - 1,bz) +
										data D1(bx,by + 4,bz) + data D1(bx,by + 3,bz) + data D1(bx,by + 2,bz) + data D1(bx,by + 1,bz) +
										data D1(bx - 4,by,bz) + data D1(bx - 3,by,bz) + data D1(bx - 2,by,bz) + data D1(bx - 1,by,bz) +
										data D1(bx + 4,by,bz) + data D1(bx + 3,by,bz) + data D1(bx + 2,by,bz) + data D1(bx + 1,by,bz);
									/*
									out[bz + by * sizez + bx * sizez*sizey] += data[bz + by * sizez + bx * sizez*sizey];
									
										data_[bx,by][bz - 4] + data_[bx][by][bz - 3] + data_[bx][by][bz - 2] + data_[bx][by][bz - 1] +
										data_[bx][by][bz + 4] + data_[bx][by][bz + 3] + data_[bx][by][bz + 2] + data_[bx][by][bz + 1] +
										data_[bx][by - 4][bz] + data_[bx][by - 3][bz] + data_[bx][by - 2][bz] + data_[bx][by - 1][bz] +
										data_[bx][by + 4][bz] + data_[bx][by + 3][bz] + data_[bx][by + 2][bz] + data_[bx][by + 1][bz] +
										data_[bx - 4][by][bz] + data_[bx - 3][by][bz] + data_[bx - 2][by][bz] + data_[bx - 1][by][bz] +
										data_[bx + 4][by][bz] + data_[bx + 3][by][bz] + data_[bx + 2][by][bz] + data_[bx + 1][by][bz];
										*/
								}
							}
				}

Kill_stopper();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Spawn_stopper("gpu tile pragma");
#pragma omp target teams
#pragma omp distribute parallel for tile sizes(blocksize_x,blocksize_y,blocksize_z)
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
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Spawn_stopper("back to ram");
}
//Kill_stopper();

free(out);

return 0;
}


//END of example

//If you dont want to compile this code use -D NO_TIME in compile option.

//This code is for the stopper not part of the example
//Do not change this code
//The example code if up

#ifdef NO_TIME
void Spawn_stopper(char* name){};
double Kill_stopper(){};
#else
#include "stopper.h"
#endif