

#include "stdlib.h"
#include "omp.h"
#include <stdio.h>
#include "stopper.h"


/* constans méretek */
size_t sizex = 80;
size_t sizey = 80;
size_t sizez = 80;

struct dataobj
{
  void* data;
};



int main(int argc, char ** argv) {
Stopper_Filemode=false;
printf("Hi this is the openMP testing..\n\n");

//following block try all the functions and list properties
printf("omp_get_num_threads = %d\n",omp_get_num_threads());

printf("omp_get_max_threads = %d\n",omp_get_max_threads());

printf("omp_get_thread_num = %d\n",omp_get_thread_num());

printf("omp_get_num_procs = %d\n",omp_get_num_procs());

printf("omp_get_thread_limit = %d\n",omp_get_thread_limit());

printf("omp_get_level = %d\n",omp_get_level());

printf("omp_get_team_size = %d\n",omp_get_team_size(0));

printf("omp_get_num_devices = %d\n",omp_get_num_devices());

printf("omp_get_wtime = %f\n",omp_get_wtime());

printf("\n---------------------gpu---------------------------\n\n");
/*

printf(" = %d\n",);

printf(" = %d\n",);


*/






uint blocksize=16;
uint blockhalf=blocksize/2;

uint meret=sizex*sizey*sizez;
printf("meret: %d \n",meret);
float* data=NULL ;//= (float*) malloc(meret*4);
float* out=NULL ;//= (float*) malloc(meret*4);
Spawn_stopper("offload");
#pragma omp target data map(alloc:data[0:sizex*sizey*sizez]) map(alloc:out[0:sizex*sizey*sizez]) map(to:sizex) map(to:sizey) map(to:sizez) map(to:blockhalf) map(to:blocksize)
{

Kill_stopper();
float(*__restrict data_)[sizey][sizez] = (float(*)[sizey][sizez])data;
float(*__restrict out_)[sizey][sizez] = (float(*)[sizey][sizez])out;
/*
Spawn_stopper("compute simple parallel");
#pragma omp target teams distribute parallel for 
    for(uint i=0;i<meret;i++)
    {
        data[i]=0;
    }
Kill_stopper();

int sum_threads=0;
Spawn_stopper("spawn team of thread");
#pragma omp target teams reduction(+:sum_threads) num_teams(1024) thread_limit(10) //distribute parallel for 
    {
        //printf(" %d \n",omp_get_team_size(omp_get_active_level()));
        #pragma omp parallel for reduction(+:sum_threads)
        for(size_t i = 0;i<10;i++)
            sum_threads++;
    }
printf("\nsum of threads: %d\n\n",sum_threads);
Kill_stopper();

Spawn_stopper("compute 10 team_thread");
#pragma omp target teams num_teams(10)
{
#pragma omp distribute parallel for //simd
    for(uint i=0;i<meret;i++)
    {
        data[i]=0;
    }
}
Kill_stopper();

Spawn_stopper("compute ");
#pragma omp target teams 
{
#pragma omp distribute parallel for
    for(uint i=0;i<meret;i+=8)
    {
        #pragma omp parallel for simd
        for(ushort i2=0;i2<8;i2++)
            data[i+i2]=1;
    }
}
Kill_stopper();
*/


Spawn_stopper("3d zeroing");
#pragma omp target teams
{
    //printf("size0,size1  %d",size1);
#pragma omp distribute parallel for collapse(3)
    for(size_t x=0;x<sizex;x++)
    {
        for(size_t y=0;y<sizey;y++)
        {
            for(size_t z=0;z<sizey;z++)
            {
                out_[x][y][z]=0;
            }        
        }
    }
} 
Kill_stopper();

Spawn_stopper("3d tiling with zeroing");
#pragma omp target teams
{
    //printf("size0,size1  %d",size1);
#pragma omp distribute parallel for collapse(3)
    for(size_t x=blockhalf;x<sizex;x+=blocksize)
    {
        for(size_t y=blockhalf;y<sizey;y+=blocksize)
        {
            for(size_t z=blockhalf;z<sizey;z+=blocksize)
            {

                #pragma omp parallel for collapse(3)
                for(size_t bx=x-blockhalf;bx<x+blockhalf;bx++)
                {
                    for(size_t by=y-blockhalf;by<y+blockhalf;by++)
                    {
                        for(size_t bz=z-blockhalf;bz<z+blockhalf;bz++)
                        {
                            data_[bx][by][bz]=0;
                        }
                    }
                }
            }        
        }
    }
} 
Kill_stopper();
Spawn_stopper("3d computation");
#pragma omp target teams num_teams(128) thread_limit(8*8*8) 
{
    //printf("size0,size1  %d",size1);
#pragma omp distribute parallel for collapse(3)
    for(size_t x=blocksize;x<sizex-blockhalf;x+=blocksize)
    {
        for(size_t y=blocksize;y<sizey-blockhalf;y+=blocksize)
        {
            for(size_t z=blocksize;z<sizey-blockhalf;z+=blocksize)
            {
                //print("asd");
                for(size_t ibx=x-blockhalf;ibx<x+blockhalf;ibx++)
                for(size_t iby=y-blockhalf;iby<y+blockhalf;iby++)
                for(size_t ibz=z-blockhalf;ibz<z+blockhalf;ibz++)
                   /* if(ibx>=0 && ibx < sizex &&
                        iby>=0 && iby < sizey &&
                        ibz>=0 && ibz < sizez)*/
                            out_[x][y][z]+=data_[ibx][iby][ibz];
            }        
        }
    }
} 
Kill_stopper();

Spawn_stopper("3d tiling with computation");
#pragma omp target teams num_teams(128) thread_limit(8*8*8) 
{
    //printf("size0,size1  %d",size1);
#pragma omp distribute parallel for collapse(3)
    for(size_t x=blockhalf;x<sizex-blockhalf;x+=blocksize)
    {
        for(size_t y=blockhalf;y<sizey-blockhalf;y+=blocksize)
        {
            for(size_t z=blockhalf;z<sizey-blockhalf;z+=blocksize)
            {

                
                #pragma omp parallel for collapse(3)
                for(size_t bx=x-blockhalf;bx<x+blockhalf;bx++)
                {
                    for(size_t by=y-blockhalf;by<y+blockhalf;by++)
                    {
                        for(size_t bz=z-blockhalf;bz<z+blockhalf;bz++)
                        {
                            
                            for(size_t ibx=bx-blockhalf;ibx<bx+blockhalf;ibx++)
                            for(size_t iby=by-blockhalf;iby<by+blockhalf;iby++)
                            for(size_t ibz=bz-blockhalf;ibz<bz+blockhalf;ibz++)
                                /*if(ibx>=0 && ibx < sizex &&
                                   iby>=0 && iby < sizey &&
                                   ibz>=0 && ibz < sizez)*/
                                    out_[bx][by][bz]+=data_[ibx][iby][ibz];
                        }
                    }
                }
            }        
        }
    }
} 
Kill_stopper();



Spawn_stopper("back to ram");
}
Kill_stopper();
/*{
    
    #pragma omp teams distribute parallel for 
    for(uint i=0;i<meret;i++)
    {
        data[i]=data[i]+1;
    }
}*/


}