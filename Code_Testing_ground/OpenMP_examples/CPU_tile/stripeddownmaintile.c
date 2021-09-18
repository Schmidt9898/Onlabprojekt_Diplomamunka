/*
only cpu 
loop blocking
tiling and unroling clang14
intel compiler 
*/



#include "stdlib.h"
#include <stdio.h>
#include "stopper.h"







/* constans méretek */   
const int sizex = 800;		  //ha nem constansak akkor nem ad warning ot de az omp blocking résznél nem lesz elérhető a módosított tömb[][][] 
const int sizey = 800;		  //és segfault hibát okoz
const int sizez = 800;


int main(int argc, char ** argv) {

stopper_str_buffer = (char*)malloc(1);
Stopper_Filemode=true;

printf("\n	%s Running.\n\n",__VERSION__);

FileStream *Fs= MakeFileStream("clangloopblocking.txt");
Fs->Write(Fs,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");



//blokméretet argumentumokkal felül lehet definiálni
/*
#define blocksize_x 16
#define blocksize_y 16
#define blocksize_z 1024

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


char output[256];
snprintf(output, 256,"cache size %f MB %d %d %d\n",blocksize_x*blocksize_y*blocksize_z*4/1e6f,blocksize_x, blocksize_y, blocksize_z);
Fs->Write(Fs,output);




//Spawn_stopper(" memory managment");


//data and offloading OpenMP

//this we will use on the cpu


//#ifdef LOMP
float* out= (float*) malloc(meret*4);
float* data= (float*) malloc(meret*4);
float(*restrict data_)[sizey][sizez] = (float(*restrict)[sizey][sizez])data;
float(*restrict out_)[sizey][sizez] = (float(*restrict)[sizey][sizez])out;



//Kill_stopper();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef TILE


Spawn_stopper("Tile size pragma");

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

#else
printf("NO TILE IN THIS ONE\n");
#endif



//////////////////////////////////////////////////////////////////////////////////////////////////////////////


Spawn_stopper("Loop blocking");

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


Fs->Write(Fs,stopper_str_buffer);
Fs->Flush(Fs);



}
