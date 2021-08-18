/*
Ebben a C programban egy egyszerő számolást hajtunk végre melynek nem 
számolásban van értelme, próbálunk minél több adatot mozgatni gpu-n
és ennek a tiling hatásait megfigyelni


fordítási opciók:



A program menete:
Cpu ram-> gpu vram -> compute -> gpu vram- > cpu ram

Cpu-n ellenőrizzük az adatot

data ból számolunk és out, out2 be írunk

*/



#include "stdlib.h"
#include <stdio.h>
#include "stopper.h"

#ifdef LOMP
#include "omp.h"
#endif

#ifdef LACC
#include "openacc.h"
#endif


/* constans méretek */   
const int sizex = 800;          //ha nem constansak akkor nem ad warning ot de az omp blocking résznél nem lesz elérhető a módosított tömb[][][] 
const int sizey = 800;          //és segfault hibát okoz
const int sizez = 800;

struct dataobj
{
  void* data;
};



int main(int argc, char ** argv) {

Stopper_Filemode=false; //Mérő beállítás, fileba mentsen e.

//printf("Hi this is the openMP testing..\n\n");

//following block try all(not realy all) the functions and list properties
/*
printf("omp_get_max_threads = %d\n",omp_get_max_threads());
printf("omp_get_thread_limit = %d\n",omp_get_thread_limit());
printf("omp_get_num_threads = %d\n",omp_get_num_threads());

printf("omp_get_team_size = %d\n",omp_get_team_size(0));

printf("omp_get_thread_num = %d\n",omp_get_thread_num());

printf("omp_get_num_procs = %d\n",omp_get_num_procs());


printf("omp_get_level = %d\n",omp_get_level());


printf("omp_get_num_devices = %d\n",omp_get_num_devices());

printf("omp_get_wtime = %f\n",omp_get_wtime());
*/
//#ifdef LOMP
printf("\nClang 14 OpenMP Running\n\n");
//#pragma message "OpenMP compiling "  __FILE__ 
//#endif
/*

printf(" = %d\n",);

printf(" = %d\n",);


*/



//blokméretet argumentumokkal felül lehet definiálni
int blocksize_x=32;
int blocksize_y=8;
int blocksize_z=4;
if(argc>=4){
    blocksize_x=atoi(argv[1]);
    blocksize_y=atoi(argv[2]);
    blocksize_z=atoi(argv[3]);
}

printf("block size: %d,%d,%d \n",blocksize_x,blocksize_y,blocksize_z);
printf("thread size = %d\n",blocksize_x*blocksize_y*blocksize_z);
//int thread_size=blocksize_x*blocksize_y*blocksize_z;

//window size
const int window_size=4;
const size_t meret=sizex*sizey*sizez;
printf("meret: %lu , %f Gb \n",meret*4,meret*4/1e9f*2);

Spawn_stopper("offload and memory managment");


//data and offloading OpenMP

//this we will use on the cpu
//float* out2= (float*) malloc(meret*4);

//#ifdef LOMP
float* data= (float*) malloc(meret*4);
float* out= (float*) malloc(meret*4);
float(*__restrict data_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])data;
float(*__restrict out_)[sizey][sizez] = (float(*__restrict)[sizey][sizez])out;



Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Spawn_stopper("3d zeroing");
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
    for(int x=0;x<sizex;x++)
    {
        for(int y=0;y<sizey;y++)
        {
            for(int z=0;z<sizez;z++)
            {
                data_[x][y][z]=1;
                out_[x][y][z]=0;
            }        
        }
    }

Kill_stopper();
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Spawn_stopper("3d computation");
#pragma omp target teams
#pragma omp distribute parallel for collapse(3) 

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

#ifdef LOMP
// we dont use this, omp doesn't have tile yet 
//#pragma omp target teams
//#pragma omp distribute parallel for collapse(3) 

#endif

/////same with openACC


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#ifdef LOMP
//This is a small scale testing.
//omp_get_max_threads();
/*
Spawn_stopper("3 team loop of 3 thread loops");
#pragma omp target teams distribute collapse(2) thread_limit(128) // run on the teamleaders
    for (int x = 0; x < 2; x += 1)
    {
        for (int y = 0; y < 2; y += 1)
        {
            omp_set_num_threads(128);
            printf("teamleader omp_get_max_threads = %d\n",omp_get_max_threads());
                        //data_[x][y][0]=1;
                #pragma omp parallel for collapse(2)// run on the threads
                for (int i = 0; i < 2; i+=1)
                {
                    for (int j = 0; j < 2; j += 1){
                    printf("omp_get_num_threads = %d\n",omp_get_num_threads());
                    int bid=omp_get_team_num();
                    int tid=omp_get_thread_num();
                    printf("bid %d, tid %d xyij: %d %d %d %d\n",bid,tid,x,y,i,j);
                    //printf("xyij: %d %d %d %d\n",x,y,i,j);
                    //for (int j = 0; j < 2; j+=1)
                    //{
                      //  for (int k = 0; k < 2; k+=1)
                       // {
                        //    printf("xyzijk: %d %d %d %d %d %d\n",x,y,z,i,j,k);
                        //}
                    //}
                    }
                }
                
            
        }
    }
Kill_stopper();
*/
//#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#ifdef LOMP
//omp_set_num_threads(1024);
/*
Spawn_stopper("3d tiling with omp teams");
//#pragma omp target teams
//#pragma omp distribute parallel for collapse(3) 
#pragma omp target teams distribute collapse(3)  thread_limit(1024) 
    for (int x = window_size; x < sizex-window_size; x += blocksize_x)
        for (int y = window_size; y < sizey-window_size; y += blocksize_y)
            for (int z = window_size; z < sizez-window_size; z += blocksize_z)
            {
                //data_[x][y][0]=1;
                #pragma omp parallel for collapse(3)
                for (int bx = x; bx < x + blocksize_x; bx++)
                    for (int by = y; by < y + blocksize_y; by++)
                        for (int bz = z; bz < z + blocksize_z; bz++)
                        {
                            if (bx < sizex-window_size &&
                                by < sizey-window_size &&
                                bz < sizez-window_size){
                                    data_[x][y][z]=1;
                                    //data[x]=1;
                                //float(*__restrict data_)[10][10] = (float(*)[10][10])data;
                                //printf("%f\n",data_[0][0][0]);
                                    
                                    //printf("xyz: %d %d %d\n",bx,by,bz);
                            out2_[bx][by][bz]+=
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
*/
//#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

Spawn_stopper("omp tiling with tiling pragma");

#pragma omp target teams tile sizes(5,5)
for(int x=0;x<10;x++)
    {
        for(int y=0;y<10;y++)
        {
           // for(int z=window_size;z<sizez-window_size;z++)
            //{
                int bid=omp_get_team_num();
                int tid=omp_get_thread_num();
                printf("bid %d, tid %d xy: %d %d \n",bid,tid,x,y);
                //kernel start
                //data_[x][y][0]++;
                /*out_[x][y][z]+=
				data_[x][y][z-4]+data_[x][y][z-3]+data_[x][y][z-2]+data_[x][y][z-1]+
				data_[x][y][z+4]+data_[x][y][z+3]+data_[x][y][z+2]+data_[x][y][z+1]+
				data_[x][y-4][z]+data_[x][y-3][z]+data_[x][y-2][z]+data_[x][y-1][z]+
				data_[x][y+4][z]+data_[x][y+3][z]+data_[x][y+2][z]+data_[x][y+1][z]+
				data_[x-4][y][z]+data_[x-3][y][z]+data_[x-2][y][z]+data_[x-1][y][z]+
				data_[x+4][y][z]+data_[x+3][y][z]+data_[x+2][y][z]+data_[x+1][y][z];*/
                //kernel end
                //printf("x:%d,y:%d,z:%d   val: %f\n",x,y,z,out_[x][y][z]);
            //}        
        }
    }
Kill_stopper();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////


Spawn_stopper("back to ram");
}
Kill_stopper();




}
