/******************************************************************************************
*                                                                                         *
* This code was copied from an example for making tiling/blocking in openMP and openACC.  *
* The calculation purpose is to reach a lot of memory from a single kernel.               * 
* This test will help finde the effects of dfferent blocksizez.                           *
*                                                                                         *
*                                                                                         *
*******************************************************************************************/

//stopper functions defined here, you may choose not to use them with -D NO_TIME
void Spawn_stopper(char *name);
double Kill_stopper();

#include "stdlib.h"
#include <stdio.h>
#include <math.h>
//use this timer and filestream
#include "stopper.h"


#ifdef LOMP
#include "omp.h"
#elif LACC
#include "openacc.h"
#endif



//argv[1] filename


/*defined block sizes*/
/*
#define X 4
#define Y 8
#define Z 32
*/



const int sizex = 800; //If this is not constant, will cause a segfault in runtime with clang-12
const int sizey = 800; //and clang-14 if -D
const int sizez = 800;

struct dataobj{void *data;};

int main(int argc, char **argv)
{

FileStream *Fs= MakeFileStream("acctestfile.txt");
Fs->Write(Fs,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//printf("ez jön a commandline ról %d",LVAR);
stopper_str_buffer = (char*)malloc(1);
Stopper_Filemode=true;



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



const int blocksize_x = X;
const int blocksize_y = Y;
const int blocksize_z = Z;

printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
char output[256];
snprintf(output, 256,"Block size %d Thread limit %d Block dim: %d,%d,%d \n",blocksize_x * blocksize_y * blocksize_z,TSIZE,blocksize_x, blocksize_y, blocksize_z);
Fs->Write(Fs,output);
//window size
const int window_size = 4;

const size_t meret = sizex * sizey * sizez;
printf("memory size needed: %lu , %f Gb \n", meret * 2, meret * 4 / 1e9f * 2);



//Spawn_stopper("offload and memory managment");

//data and offloading OpenMP, OpenACC

//this we will use on the cpu, but not this time
float * nothing = NULL;
#ifdef LOMP
float * out = NULL;//(float *)malloc(meret * 4);
float * out2 = NULL;//(float *)malloc(meret * 4);
float * data = NULL;
#pragma omp target data map(alloc : data[0:meret]) \
            map(alloc  : out[0:meret])  \
            map(alloc  : out2[0:meret])
#elif LACC
#pragma acc data present(nothing) //copyout(out[0:meret]) copyout(out2[0:meret]) 
#endif
{
//Kill_stopper();

#ifdef LACC
float* data=(float*)acc_malloc(meret*4);
float* out=(float*)acc_malloc(meret*4);
float* out2=(float*)acc_malloc(meret*4);
#endif

//pointerfolding, this does not work well with openMP

float(*__restrict data_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data;
float(*__restrict out_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out;
float(*__restrict out2_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out2;

//printf("whoooo\n");
//init aka zeroing
/*
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#elif LACC
#pragma acc parallel loop collapse(3) deviceptr(data,data_,out,out_,out2,out2_)
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
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Spawn_stopper("3d computation collapse (3)");
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#elif LACC
#pragma acc parallel loop collapse(3) deviceptr(data,data_,out,out_,out2,out2_)
#endif
  for (int x = window_size; x < sizex - window_size; x++)
  {
    for (int y = window_size; y < sizey - window_size; y++)
    {
      for (int z = window_size; z < sizez - window_size; z++)
      {
        //kernel start
         KERNEL_WINDOW(out,x,y,z)
      }
    }
  }
Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef LOMP
Spawn_stopper("3d tiling OpenMP");
#pragma omp target teams distribute collapse(3) thread_limit(TSIZE)
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
                KERNEL_WINDOW(out2,bx,by,bz);
              }
            }
      }
#elif LACC
Spawn_stopper("3d tiling OpenACC");
#pragma acc parallel loop deviceptr(data,data_,out,out_,out2,out2_) tile(blocksize_z,blocksize_y,blocksize_x) private(sizex,sizey,sizez)
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
/*
for (int i = 0; i < meret; i++) {
	if (out[i]!=out2[i]) {
    printf("Validation failed\n");
	printf("out1 %f != out2 %f\n",out[i],out2[i]);
    break;
	}
  
}
*/
//free(out);
//free(out2);

Fs->Write(Fs,stopper_str_buffer);
Fs->Flush(Fs);


return 0;
}

//END of example

