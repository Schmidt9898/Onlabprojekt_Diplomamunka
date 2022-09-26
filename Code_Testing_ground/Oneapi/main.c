#include "stdlib.h"
#include <stdio.h>
#include "stopper.h"

#define TILE


void Getcompiler()
{
  char compiler_name[20] =
#ifdef __clang__
   "Clang";
#elif __GNU__
   "GNU";
#elif __INTEL_COMPILER
  "Intel API";
#else
"NOT FOUND";
#endif


printf("\n	%s Running\n\n",__VERSION__);
printf("%s\n",compiler_name);

printf("\n\n");

}








//#include "omp.h"



/* constans méretek */   
const int sizex = 800;		  //ha nem constansak akkor nem ad warning ot de az omp blocking résznél nem lesz elérhető a módosított tömb[][][] 
const int sizey = 800;		  //és segfault hibát okoz
const int sizez = 800;

struct dataobj
{
  void* data;
};



int main(int argc, char ** argv) {


printf("ez az");


Stopper_Filemode=false; 
//printf("Hi this is the openMP testing..\n\n");

//following block try all(not realy all) the functions and list properties


//printf("\nClang 14 OpenMP Running\n\n");
//printf("\n	%s Running.\n\n",__VERSION__);
Getcompiler();






//blokméretet argumentumokkal felül lehet definiálni
#define blocksize_x 16
#define blocksize_y 16
#define blocksize_z 512


/*if(argc>=4){
	blocksize_x=atoi(argv[1]);
	blocksize_y=atoi(argv[2]);
	blocksize_z=atoi(argv[3]);
}
*/



printf("block size: %d,%d,%d \n",blocksize_x,blocksize_y,blocksize_z);
//printf("thread size = %d\n",blocksize_x*blocksize_y*blocksize_z);
//int thread_size=blocksize_x*blocksize_y*blocksize_z;

//window size
const int window_size=4;
//#define window_size 4
const size_t meret=sizex*sizey*sizez;
printf("meret: %lu , %f Gb \n",meret*4,meret*4/1e9f*2);


printf("cache size %f MB \n",blocksize_x*blocksize_y*blocksize_z*4/1e6f);

Spawn_stopper(" memory managment");


//data and offloading OpenMP

//this we will use on the cpu


//#ifdef LOMP
float* out= (float*) malloc(meret*4);
float* data= (float*) malloc(meret*4);
float(*restrict data_)[sizey][sizez] = (float(*restrict)[sizey][sizez])data;
float(*restrict out_)[sizey][sizez] = (float(*restrict)[sizey][sizez])out;



Kill_stopper();



////////////////////////////////////////////////////////////////////////////////////////////////////////////

Spawn_stopper("serial");
//#pragma omp parallel for collapse(3) 
for(int x=window_size;x<sizex-window_size;x++)
	{
		for(int y=window_size;y<sizey-window_size;y++)
		{
			for(int z=window_size;z<sizez-window_size;z++)
			{
				//kernel start
				
				out_[x][y][z]+=
				data_[x][y][z-4]+data_[x][y][z-3]+data_[x][y][z-2]+data_[x][y][z-1]+
				data_[x][y][z+4]+data_[x][y][z+3]+data_[x][y][z+2]+data_[x][y][z+1]+
				data_[x][y-4][z]+data_[x][y-3][z]+data_[x][y-2][z]+data_[x][y-1][z]+
				data_[x][y+4][z]+data_[x][y+3][z]+data_[x][y+2][z]+data_[x][y+1][z]+
				data_[x-4][y][z]+data_[x-3][y][z]+data_[x-2][y][z]+data_[x-1][y][z]+
				data_[x+4][y][z]+data_[x+3][y][z]+data_[x+2][y][z]+data_[x+1][y][z];
				//kernel end
				//printf("x:%d,y:%d,z:%d   val: %f\n",x,y,z,out_[x][y][z]);
			}		
		}
	}
Kill_stopper();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Spawn_stopper("parallel default");

#pragma omp parallel for collapse(3)
for(int x=window_size;x<sizex-window_size;x++)
	{
		for(int y=window_size;y<sizey-window_size;y++)
		{
			//#pragma clang loop vectorize(enable)
			for(int z=window_size;z<sizez-window_size;z++)
			{
				//kernel start
				
				out_[x][y][z]+=
				data_[x][y][z-4]+data_[x][y][z-3]+data_[x][y][z-2]+data_[x][y][z-1]+
				data_[x][y][z+4]+data_[x][y][z+3]+data_[x][y][z+2]+data_[x][y][z+1]+
				data_[x][y-4][z]+data_[x][y-3][z]+data_[x][y-2][z]+data_[x][y-1][z]+
				data_[x][y+4][z]+data_[x][y+3][z]+data_[x][y+2][z]+data_[x][y+1][z]+
				data_[x-4][y][z]+data_[x-3][y][z]+data_[x-2][y][z]+data_[x-1][y][z]+
				data_[x+4][y][z]+data_[x+3][y][z]+data_[x+2][y][z]+data_[x+1][y][z];
				//kernel end
				//printf("x:%d,y:%d,z:%d   val: %f\n",x,y,z,out_[x][y][z]);
			}		
		}
	}
Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef TILE


Spawn_stopper("tile size pragma");

#pragma omp parallel for
#pragma omp tile sizes(blocksize_x,blocksize_y,blocksize_z)
for(int x=window_size;x<sizex-window_size;x++)
	{
		for(int y=window_size;y<sizey-window_size;y++)
		{
			#pragma clang loop vectorize(enable)
			for(int z=window_size;z<sizez-window_size;z++)
			{
				//kernel start
				
				out_[x][y][z]+=
				data_[x][y][z-4]+data_[x][y][z-3]+data_[x][y][z-2]+data_[x][y][z-1]+
				data_[x][y][z+4]+data_[x][y][z+3]+data_[x][y][z+2]+data_[x][y][z+1]+
				data_[x][y-4][z]+data_[x][y-3][z]+data_[x][y-2][z]+data_[x][y-1][z]+
				data_[x][y+4][z]+data_[x][y+3][z]+data_[x][y+2][z]+data_[x][y+1][z]+
				data_[x-4][y][z]+data_[x-3][y][z]+data_[x-2][y][z]+data_[x-1][y][z]+
				data_[x+4][y][z]+data_[x+3][y][z]+data_[x+2][y][z]+data_[x+1][y][z];
				//kernel end
				//printf("x:%d,y:%d,z:%d   val: %f\n",x,y,z,out_[x][y][z]);
			}		
		}
	}
Kill_stopper();
/*
#pragma omp parallel for
#pragma omp tile sizes(2,2)
for(int x=0;x<10;x++)
	{
		for(int y=0;y<10;y++)
		{
			//for(int z=0;z<2;z++)
			//{
				int num_thread = omp_get_num_threads();
				//int bid=omp_get_team_num();
				int tid=omp_get_thread_num();
				printf("num_thread %d xyz: %d %d  tID %d \n",num_thread,x,y,tid);
			//}		
		}
	}
	*/
#else
printf("NO TILE IN THIS ONE\n");
#endif







Spawn_stopper("omp with collapse 2 and simd");
//#pragma omp tile sizes(blocksize_x,blocksize_y,blocksize_z)
#pragma omp parallel for collapse(2) 
for(int x=window_size;x<sizex-window_size;x++)
	{
		for(int y=window_size;y<sizey-window_size;y++)
		{
			#pragma omp simd
			//#pragma clang loop vectorize(enable)
			for(int z=window_size;z<sizez-window_size;z++)
			{
				//int bid=omp_get_team_num();
				//int tid=omp_get_thread_num();
				//printf("bid %d, tid %d xy: %d %d \n",bid,tid,x,y);
				//kernel start
				
				out_[x][y][z]+=
				data_[x][y][z-4]+data_[x][y][z-3]+data_[x][y][z-2]+data_[x][y][z-1]+
				data_[x][y][z+4]+data_[x][y][z+3]+data_[x][y][z+2]+data_[x][y][z+1]+
				data_[x][y-4][z]+data_[x][y-3][z]+data_[x][y-2][z]+data_[x][y-1][z]+
				data_[x][y+4][z]+data_[x][y+3][z]+data_[x][y+2][z]+data_[x][y+1][z]+
				data_[x-4][y][z]+data_[x-3][y][z]+data_[x-2][y][z]+data_[x-1][y][z]+
				data_[x+4][y][z]+data_[x+3][y][z]+data_[x+2][y][z]+data_[x+1][y][z];
				//kernel end
				//printf("x:%d,y:%d,z:%d   val: %f\n",x,y,z,out_[x][y][z]);
			}		
		}
	}
	

Kill_stopper();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


Spawn_stopper("computing loop blocking");
//#pragma omp target teams
//#pragma omp distribute parallel for collapse(3) 
#pragma omp parallel for collapse(3) 
	for (int x = window_size; x < sizex-window_size; x += blocksize_x)
		for (int y = window_size; y < sizey-window_size; y += blocksize_y)
			for (int z = window_size; z < sizez-window_size; z += blocksize_z)
			{
				//#pragma omp parallel for collapse(2)
				for (int bx = x; bx < x + blocksize_x; bx++)
					for (int by = y; by < y + blocksize_y; by++)
						//#pragma clang loop vectorize(enable)
						#pragma omp simd
						for (int bz = z; bz < z + blocksize_z; bz++)
						{
							if (bx < sizex-window_size &&
								by < sizey-window_size &&
								bz < sizez-window_size){
								   
							out_[bx][by][bz]+=
				data_[bx][by][bz-4]+data_[bx][by][bz-3]+data_[bx][by][bz-2]+data_[bx][by][bz-1]+
				data_[bx][by][bz+4]+data_[bx][by][bz+3]+data_[bx][by][bz+2]+data_[bx][by][bz+1]+
				data_[bx][by-4][bz]+data_[bx][by-3][bz]+data_[bx][by-2][bz]+data_[bx][by-1][bz]+
				data_[bx][by+4][bz]+data_[bx][by+3][bz]+data_[bx][by+2][bz]+data_[bx][by+1][bz]+
				data_[bx-4][by][bz]+data_[bx-3][by][bz]+data_[bx-2][by][bz]+data_[bx-1][by][bz]+
				data_[bx+4][by][bz]+data_[bx+3][by][bz]+data_[bx+2][by][bz]+data_[bx+1][by][bz];
								}
						}
			}
					
				
	
Kill_stopper();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


free(data);
free(out);

}
