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

#include "omp.h"
#define X 2
#define Y 2
#define Z 1
#define TSIZE 1000 

/* constans méretek */


#define sizex 2 //If this is not constant, will cause a segfault in runtime with clang-12
#define sizey 2 //and clang-14
#define sizez 2

struct dataobj{void *data;};

int main(int argc, char **argv)
{

printf("Hi this is openMP tiling test, \n");



const int blocksize_x = X;
const int blocksize_y = Y;
const int blocksize_z = Z;

printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
//window size
const int window_size = 4;

const size_t meret = sizex * sizey * sizez;
printf("memory size needed: %lu , %f Gb \n", meret * 2, meret * 4 / 1e9f * 2);



//Spawn_stopper("offload and memory managment");

//data and offloading OpenMP

//this we will use on the cpu

float * data = (float *)malloc(meret * 4);
//float(*__restrict data_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data;

#pragma omp target data map(from: data[0:meret]) 
{
//Kill_stopper();




float(*__restrict data_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data;



/*

#pragma omp target teams distribute collapse(1) thread_limit(2)
  for (int x = 0; x < 10; x += 2)
  #pragma omp parallel for collapse(1) 
        for (int bx = x; bx < x + 2; bx++)
					printf("pointer data data_: %p \n",tomb);
      */


/*
#pragma omp target team
{
data_=(float(*__restrict)[sizey][sizez])data;
}

*/
//init aka zeroing

#pragma omp target teams
#pragma omp distribute parallel for collapse(3)

  for (int x = 0; x < sizex ; x++)
  {
    for (int y = 0; y < sizey ; y++)
    {
      for (int z = 0; z < sizez ; z++)
      {
        //kernel start
        data_[x][y][z] = 0.0f;
      }
    }
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Spawn_stopper("3d computation collapse (3)");
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)

  for (int x = 0; x < sizex ; x++)
  {
    for (int y = 0; y < sizey ; y++)
    {
      for (int z = 0; z < sizez ; z++)
      {
        //kernel start
         //KERNEL_WINDOW(out,x,y,z)
         data_[x][y][z]+=1;
         //printf("On the GPU from collapse3 %p %p \n",data,data_);
         printf("pointer data data_: %p   %p       x,y,z %d %d %d   data %f     T:%d\n",data,data_,x,y,z,data_[x][y][z],omp_get_thread_num());
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

printf("\nOn the CPU: %p %p\n\n",data,data_);


Spawn_stopper("3d tiling OpenMP");

//#pragma omp target data use_device_ptr(data_)
#pragma omp target teams distribute collapse(3) thread_limit(512) firstprivate(data_)
  for (int x = 0; x < sizex ; x += blocksize_x)
    for (int y = 0; y < sizey ; y += blocksize_y)
      for (int z = 0; z < sizez ; z += blocksize_z)
      {
        
				  //printf("On the GPU %p %p %p\n",data_,out_,out2_);
  #pragma omp parallel for collapse(3) firstprivate(data_)
        for (int bx = x; bx < x + blocksize_x; bx++)
          for (int by = y; by < y + blocksize_y; by++)
            for (int bz = z; bz < z + blocksize_z; bz++)
            {
              //printf("a");
              if (bx < sizex  &&
                by < sizey  &&
                bz < sizez )
              {
						    printf("pointer data data_: %p   %p       x,y,z %d %d %d   data %f     T:%d\n",data,data_,bx,by,bz,data_[x][y][z],omp_get_thread_num());
                	data_[bx][by][bz]+=1;

              }
            }
      }


Kill_stopper();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Spawn_stopper("back to ram");
}
//Kill_stopper();
float(*__restrict data_)[sizey][sizez] =(float(*__restrict)[sizey][sizez])data;
  for (int x = 0; x < sizex ; x++)
  {
    for (int y = 0; y < sizey ; y++)
    {
      for (int z = 0; z < sizez ; z++)
      {
      if (data_[x][y][z] !=1) {
        printf("Validation failed     ");
        //goto ldone;
      }else
      printf("Validation passed     ");
      printf("data_ %f != 1\n",data_[x][y][z]);
      }
    }
  }
ldone:
free(data);
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
