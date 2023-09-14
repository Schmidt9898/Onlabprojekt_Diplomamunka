#ifdef DECOMP
#else
#endif

//performance metrics
//stopper functions defined here, you may choose not to use them with -D NO_TIME
void Spawn_stopper(char *name);
double Kill_stopper();
typedef struct dataobj dataobj;
struct dataobj
{
  void * __restrict data;
  unsigned long * size;
  unsigned long * npsize;
  unsigned long * dsize;
  int * hsize;
  int * hofs;
  int * oofs;
  void * dmap;
} ;
dataobj create_data(unsigned long size0, unsigned long size1, unsigned long size2, unsigned long size3, unsigned long elemsize);
void delete_dataobj(dataobj a);



#include "stdlib.h"
#include <stdio.h>
#include <math.h>
#include "omp.h"
/* constans méretek */
/*
#define X 800
#define Y 800
#define Z 800
*/
//const int sizex = 800; //If this is not constant, will cause a segfault in runtime with clang-12
//const int sizey = 800; //and clang-14 if -D
//const int sizez = 800;
const int blocksize_x = 8;
const int blocksize_y = 4;
const int blocksize_z = 32;
//#define blocksize_x 8
//#define blocksize_y 4
//#define blocksize_z 32

#define DECOMP

#define Ddim(arr,x,y,z) arr[(z) + (y)*sizez + (x)*sizez * sizey]

#ifdef DECOMP
#define uL0(t,x, y, z) u[(x)*y_stride0 + (y)*z_stride0 + (z)]
#define uL1(t,x, y, z) u0[(x)*y_stride0 + (y)*z_stride0 + (z)]
#define uL2(t,x, y, z) u1[(x)*y_stride0 + (y)*z_stride0 + (z)]
#else
#define uL0(t, x, y, z) u[(t)*x_stride0 + (x)*y_stride0 + (y)*z_stride0 + (z)]
#endif


#define dampL0(x, y, z) damp[(x)*y_stride1 + (y)*z_stride1 + (z)]
#define vpL0(x, y, z) vp[(x)*y_stride0 + (y)*z_stride0 + (z)]

#ifdef NO
#define KERNEL_WINDOW(out,x,y,z) Ddim(out,x,y,z) += \
                 Ddim(data,x,y,z - 4) + Ddim(data,x,y,z - 3) + Ddim(data,x,y,z - 2) + Ddim(data,x,y,z - 1) +\
                 Ddim(data,x,y,z + 4) + Ddim(data,x,y,z + 3) + Ddim(data,x,y,z + 2) + Ddim(data,x,y,z + 1) +\
                 Ddim(data,x,y - 4,z) + Ddim(data,x,y - 3,z) + Ddim(data,x,y - 2,z) + Ddim(data,x,y - 1,z) +\
                 Ddim(data,x,y + 4,z) + Ddim(data,x,y + 3,z) + Ddim(data,x,y + 2,z) + Ddim(data,x,y + 1,z) +\
                 Ddim(data,x - 4,y,z) + Ddim(data,x - 3,y,z) + Ddim(data,x - 2,y,z) + Ddim(data,x - 1,y,z) +\
                 Ddim(data,x + 4,y,z) + Ddim(data,x + 3,y,z) + Ddim(data,x + 2,y,z) + Ddim(data,x + 1,y,z);
#endif

//#define KERNEL_WINDOW2(out,x,y,z) float r10 = 1.0F/(Ddim(data1,x + 8, y + 8, z + 8)*Ddim(data1,x + 8, y + 8, z + 8)); \
//Ddim(out, x + 8, y + 8, z + 8) = (r9*Ddim(data2, x + 1, y + 1, z + 1)*Ddim(data3, x + 8, y + 8, z + 8) + r10*(-r8*(-2.0F*Ddim(data3, x + 8, y + 8, z + 8)) - r8*Ddim(data4, x + 8, y + 8, z + 8)) + 7.93650813e-6F*(-Ddim(data3, x + 4, y + 8, z + 8) - Ddim(data3, x + 8, y + 4, z + 8) - Ddim(data3, x + 8, y + 8, z + 4) - Ddim(data3, x + 8, y + 8, z + 12) - Ddim(data3, x + 8, y + 12, z + 8) - Ddim(data3, x + 12, y + 8, z + 8)) + 1.12874782e-4F*(Ddim(data3, x + 5, y + 8, z + 8) + Ddim(data3, x + 8, y + 5, z + 8) + Ddim(data3, x + 8, y + 8, z + 5) + Ddim(data3, x + 8, y + 8, z + 11) + Ddim(data3, x + 8, y + 11, z + 8) + Ddim(data3, x + 11, y + 8, z + 8)) + 8.8888891e-4F*(-Ddim(data3, x + 6, y + 8, z + 8) - Ddim(data3, x + 8, y + 6, z + 8) - Ddim(data3, x + 8, y + 8, z + 6) - Ddim(data3, x + 8, y + 8, z + 10) - Ddim(data3, x + 8, y + 10, z + 8) - Ddim(data3, x + 10, y + 8, z + 8)) + 7.11111128e-3F*(Ddim(data3, x + 7, y + 8, z + 8) + Ddim(data3, x + 8, y + 7, z + 8) + Ddim(data3, x + 8, y + 8, z + 7) + Ddim(data3, x + 8, y + 8, z + 9) + Ddim(data3, x + 8, y + 9, z + 8) + Ddim(data3, x + 9, y + 8, z + 8)) - 3.79629639e-2F*Ddim(data3, x + 8, y + 8, z + 8))/(r8*r10 + r9*Ddim(data2, x + 1, y + 1, z + 1));




#ifdef DECOMP

#define DEV_KERNEL float r10 = 1.0F/(vpL0(x + 8, y + 8, z + 8)*vpL0(x + 8, y + 8, z + 8));\
uL0(t2, x + 8, y + 8, z + 8) = (r9*dampL0(x + 1, y + 1, z + 1)*uL1(t0, x + 8, y + 8, z + 8) + r10*(-r8*(-2.0F*uL1(t0, x + 8, y + 8, z + 8)) - r8*uL2(t1, x + 8, y + 8, z + 8)) + 7.93650813e-6F*(-uL1(t0, x + 4, y + 8, z + 8) - uL1(t0, x + 8, y + 4, z + 8) - uL1(t0, x + 8, y + 8, z + 4) - uL1(t0, x + 8, y + 8, z + 12) - uL1(t0, x + 8, y + 12, z + 8) - uL1(t0, x + 12, y + 8, z + 8)) + 1.12874782e-4F*(uL1(t0, x + 5, y + 8, z + 8) + uL1(t0, x + 8, y + 5, z + 8) + uL1(t0, x + 8, y + 8, z + 5) + uL1(t0, x + 8, y + 8, z + 11) + uL1(t0, x + 8, y + 11, z + 8) + uL1(t0, x + 11, y + 8, z + 8)) + 8.8888891e-4F*(-uL1(t0, x + 6, y + 8, z + 8) - uL1(t0, x + 8, y + 6, z + 8) - uL1(t0, x + 8, y + 8, z + 6) - uL1(t0, x + 8, y + 8, z + 10) - uL1(t0, x + 8, y + 10, z + 8) - uL1(t0, x + 10, y + 8, z + 8)) + 7.11111128e-3F*(uL1(t0, x + 7, y + 8, z + 8) + uL1(t0, x + 8, y + 7, z + 8) + uL1(t0, x + 8, y + 8, z + 7) + uL1(t0, x + 8, y + 8, z + 9) + uL1(t0, x + 8, y + 9, z + 8) + uL1(t0, x + 9, y + 8, z + 8)) - 3.79629639e-2F*uL1(t0, x + 8, y + 8, z + 8))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));

#else

#define DEV_KERNEL float r10 = 1.0F/(vpL0(x + 8, y + 8, z + 8)*vpL0(x + 8, y + 8, z + 8)); \
uL0(t2, x + 8, y + 8, z + 8) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 8, y + 8, z + 8) + r10*(-r8*(-2.0F*uL0(t0, x + 8, y + 8, z + 8)) - r8*uL0(t1, x + 8, y + 8, z + 8)) + 7.93650813e-6F*(-uL0(t0, x + 4, y + 8, z + 8) - uL0(t0, x + 8, y + 4, z + 8) - uL0(t0, x + 8, y + 8, z + 4) - uL0(t0, x + 8, y + 8, z + 12) - uL0(t0, x + 8, y + 12, z + 8) - uL0(t0, x + 12, y + 8, z + 8)) + 1.12874782e-4F*(uL0(t0, x + 5, y + 8, z + 8) + uL0(t0, x + 8, y + 5, z + 8) + uL0(t0, x + 8, y + 8, z + 5) + uL0(t0, x + 8, y + 8, z + 11) + uL0(t0, x + 8, y + 11, z + 8) + uL0(t0, x + 11, y + 8, z + 8)) + 8.8888891e-4F*(-uL0(t0, x + 6, y + 8, z + 8) - uL0(t0, x + 8, y + 6, z + 8) - uL0(t0, x + 8, y + 8, z + 6) - uL0(t0, x + 8, y + 8, z + 10) - uL0(t0, x + 8, y + 10, z + 8) - uL0(t0, x + 10, y + 8, z + 8)) + 7.11111128e-3F*(uL0(t0, x + 7, y + 8, z + 8) + uL0(t0, x + 8, y + 7, z + 8) + uL0(t0, x + 8, y + 8, z + 7) + uL0(t0, x + 8, y + 8, z + 9) + uL0(t0, x + 8, y + 9, z + 8) + uL0(t0, x + 9, y + 8, z + 8)) - 3.79629639e-2F*uL0(t0, x + 8, y + 8, z + 8))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));

#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


int Forward(struct dataobj *restrict damp_vec, struct dataobj *restrict u_vec, struct dataobj *restrict vp_vec, const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m, const float dt, const int devicerm)
{

float r8 = 1.0F/(dt*dt);
float r9 = 1.0F/dt;

//remove some

  const long x_fsz0 = u_vec->size[1];
  const long y_fsz0 = u_vec->size[2];
  const long z_fsz0 = u_vec->size[3];
  const long y_fsz1 = damp_vec->size[1];
  const long z_fsz1 = damp_vec->size[2];
  const long x_stride0 = x_fsz0*y_fsz0*z_fsz0;
  const long y_stride0 = y_fsz0*z_fsz0;
  const long z_stride0 = z_fsz0;
  const long y_stride1 = y_fsz1*z_fsz1;
  const long z_stride1 = z_fsz1;
///////////////////////////////////////////////
/*
  const long x_fsz0 = 896;
  const long y_fsz0 = 896;
  const long z_fsz0 = 896;
  const long y_fsz1 = 882;
  const long z_fsz1 = 882;
  const long x_stride0 = 719323136;
  const long y_stride0 = 802816;
  const long z_stride0 = 896;
  const long y_stride1 = 777924;
  const long z_stride1 = 882;
*/


int t0 = 0;
int t1 = 1;
int t2 = 2;


//printf("block size: %d,%d,%d \n", blocksize_x, blocksize_y, blocksize_z);

//printf("thread size needed = %d\n", blocksize_x * blocksize_y * blocksize_z);
//window size
//const int window_size = 8;

//const size_t meret = sizex * sizey * sizez;
//printf("memory size needed: %lu , %f Gb \n", meret * 2, meret * 4 / 1e9f * 2);



//Spawn_stopper("offload and memory managment");

//data and offloading OpenMP


  float * __restrict damp = (float *) damp_vec->data;
  float * __restrict u = (float *) u_vec->data;
  float * __restrict vp = (float *) vp_vec->data;


  #ifdef DECOMP
  float * __restrict u0 = ((float *) u_vec->data) + u_vec->size[3]*u_vec->size[1]*u_vec->size[2] ;
  float * __restrict u1 = ((float *) u_vec->data) + u_vec->size[3]*u_vec->size[1]*u_vec->size[2] * 2 ;
	#else
	#endif


//#pragma omp target data map(tofrom : damp_vec->data[0:meret]) \
{
#ifdef DECOMP
  #pragma omp target enter data map(to: u[0:u_vec->size[3]*u_vec->size[1]*u_vec->size[2]])
  #pragma omp target enter data map(to: u0[0:u_vec->size[3]*u_vec->size[1]*u_vec->size[2]])
  #pragma omp target enter data map(to: u1[0:u_vec->size[3]*u_vec->size[1]*u_vec->size[2]])
#else
  #pragma omp target enter data map(to: u[0:u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]])
#endif
  #pragma omp target enter data map(to: damp[0:damp_vec->size[0]*damp_vec->size[1]*damp_vec->size[2]])
  #pragma omp target enter data map(to: vp[0:vp_vec->size[0]*vp_vec->size[1]*vp_vec->size[2]])




//#pragma omp target data map(to: u[0:u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]]) \
//	map(to: damp[0:damp_vec->size[0]*damp_vec->size[1]*damp_vec->size[2]]) \
//	map(to: vp[0:vp_vec->size[0]*vp_vec->size[1]*vp_vec->size[2]])
{



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Spawn_stopper("Naiv");

//for (int t = 0;t<10;t++)
{
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
  for (int x = x_m; x < x_M ; x++)
  {
    for (int y = y_m; y < y_M ; y++)
    {
      for (int z = z_m; z < z_M ; z++)
      {
        //kernel start
        //KERNEL_WINDOW2(out,x,y,z)
		DEV_KERNEL
      }
    }
  }
}
Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*if(out2_==out2)
printf("pointer: %x   %x\n",out2_,out2);
else
printf("asd\n");*/


Spawn_stopper("Tiling");

//for (int t = 0;t<10;t++)
{
//shared(out2_,data_)
#pragma omp target teams distribute collapse(3) thread_limit(256) //deviceptr(damp,u,u0,u1,vp)
for(int Bx = x_m; Bx <= x_M; Bx += blocksize_x)
	for (int By = y_m; By <= y_M; By += blocksize_y)
		for (int Bz = z_m; Bz <= z_M; Bz += blocksize_z)
		{
		#pragma omp parallel for collapse(3)
		for (int x = Bx; x < Bx + blocksize_x; x++)
			for (int y = By; y < By + blocksize_y; y++)
				for (int z = Bz; z < Bz + blocksize_z; z++)
				{
					if (x <= x_M && y <= y_M && z <= z_M){
			//DEV_KERNEL
			//DEV_KERNEL(bx,by,bz)
                  //KERNEL_WINDOW2(out2,bx,by,bz);
				float r10 = 1.0F/(vpL0(x + 8, y + 8, z + 8)*vpL0(x + 8, y + 8, z + 8));
				uL0(t2, x + 8, y + 8, z + 8) = (r9*dampL0(x + 1, y + 1, z + 1)*uL1(t0, x + 8, y + 8, z + 8) + r10*(-r8*(-2.0F*uL1(t0, x + 8, y + 8, z + 8)) - r8*uL2(t1, x + 8, y + 8, z + 8)) + 7.93650813e-6F*(-uL1(t0, x + 4, y + 8, z + 8) - uL1(t0, x + 8, y + 4, z + 8) - uL1(t0, x + 8, y + 8, z + 4) - uL1(t0, x + 8, y + 8, z + 12) - uL1(t0, x + 8, y + 12, z + 8) - uL1(t0, x + 12, y + 8, z + 8)) + 1.12874782e-4F*(uL1(t0, x + 5, y + 8, z + 8) + uL1(t0, x + 8, y + 5, z + 8) + uL1(t0, x + 8, y + 8, z + 5) + uL1(t0, x + 8, y + 8, z + 11) + uL1(t0, x + 8, y + 11, z + 8) + uL1(t0, x + 11, y + 8, z + 8)) + 8.8888891e-4F*(-uL1(t0, x + 6, y + 8, z + 8) - uL1(t0, x + 8, y + 6, z + 8) - uL1(t0, x + 8, y + 8, z + 6) - uL1(t0, x + 8, y + 8, z + 10) - uL1(t0, x + 8, y + 10, z + 8) - uL1(t0, x + 10, y + 8, z + 8)) + 7.11111128e-3F*(uL1(t0, x + 7, y + 8, z + 8) + uL1(t0, x + 8, y + 7, z + 8) + uL1(t0, x + 8, y + 8, z + 7) + uL1(t0, x + 8, y + 8, z + 9) + uL1(t0, x + 8, y + 9, z + 8) + uL1(t0, x + 9, y + 8, z + 8)) - 3.79629639e-2F*uL1(t0, x + 8, y + 8, z + 8))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));




              }
            }
      }
}
Kill_stopper();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

  #pragma omp target update from(u[0:u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]])
  #pragma omp target exit data map(release: u[0:u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]]) if(devicerm)
  #pragma omp target exit data map(delete: damp[0:damp_vec->size[0]*damp_vec->size[1]*damp_vec->size[2]]) if(devicerm && damp_vec->size[0] != 0 && damp_vec->size[1] != 0 && damp_vec->size[2] != 0)
  #pragma omp target exit data map(delete: vp[0:vp_vec->size[0]*vp_vec->size[1]*vp_vec->size[2]]) if(devicerm && vp_vec->size[0] != 0 && vp_vec->size[1] != 0 && vp_vec->size[2] != 0)




}




int main(int argc, char **argv)
{







dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
dataobj u_vec = create_data( 3,896,896,896,sizeof(float));
dataobj vp_vec = create_data( 896,896,896,1,sizeof(float));
const int x_M = 879;
const int x_m = 0;
const int y_M = 879;
const int y_m = 0;
const int z_M = 879;
const int z_m = 0;
float dt = 1.816000;

int devicerm = 1;


Forward((struct dataobj *restrict) &damp_vec,(struct dataobj *restrict) &u_vec,(struct dataobj *restrict) &vp_vec,x_M,x_m,y_M,y_m,z_M,z_m,dt,devicerm);











//Spawn_stopper("back to ram");
//}
//Kill_stopper();
/*
for (int i = 0; i < meret; i++) {
  if (out[i]!=out2[i]) {
    printf("Validation failed\n");
  printf("out1 %f != out2 %f\n",out[i],out2[i]);
    break;
  }
  
}
free(out);
free(out2);
*/

return 0;
}







size_t total_memory_needed=0;
dataobj create_data(unsigned long size0, unsigned long size1, unsigned long size2, unsigned long size3, unsigned long elemsize)
{
	dataobj a;


	a.data = malloc(elemsize * size0 * size1 * size2 * size3); // new char[elemsize*size0*size1*size2*size3]; // 340*340*340
	total_memory_needed += elemsize * size0 * size1 * size2 * size3;

	size_t arrsize = size0 * size1 * size2 * size3;
	for (size_t i = 0; i < arrsize; i += elemsize)
	{
		float *t = (float *)&a.data[i];
		*t = 5;//TODO (0,1) random
		//*t = 0.0/0.0;  //nan
	}

	//this is long for f sake
	a.size = malloc(sizeof(unsigned long) * 4); // new int[4];

#ifdef MOREINFO 
	printf("Creting %ld\n",size0*size1*size2*size3);
#endif
	a.size[0] = size0;
	a.size[1] = size1;
	a.size[2] = size2;
	a.size[3] = size3;
	
	return a;
}
void delete_dataobj(dataobj a)
{
	free(a.data);
	free(a.size);
}





//////////////////////////////////////////////////////////////
//  _______ _                         						//
// |__   __(_)                        						//
//    | |   _ _ __ ___   ___ _ __ ___ 						//
//    | |  | | '_ ` _ \ / _ \ '__/ __|						//
//    | |  | | | | | | |  __/ |  \__ \						//
//    |_|  |_|_| |_| |_|\___|_|  |___/						//
//															//
//////////////////////////////////////////////////////////////	

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

