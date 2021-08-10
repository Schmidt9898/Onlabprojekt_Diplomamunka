/*
Ebben a C programban egy egyszerő számolást hajtunk végre melynek nem 
számolásban van értelme, próbálunk minél több adatot mozgatni gpu-n
és ennek a tiling hatásait megfigyelni

A kódban Definíciókkal elkülönített pragmákat és kódrészreteket találunk
Értelme hogy minden fordításban ugyanazt a számolást végezzzük el
opciók 
-LOMP
-LACC

késöbb lehet cpu külön

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
const int sizex = 800;
const int sizey = 800;
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
printf("omp_get_num_threads = %d\n",omp_get_num_threads());

printf("omp_get_max_threads = %d\n",omp_get_max_threads());

printf("omp_get_thread_num = %d\n",omp_get_thread_num());

printf("omp_get_num_procs = %d\n",omp_get_num_procs());

printf("omp_get_thread_limit = %d\n",omp_get_thread_limit());

printf("omp_get_level = %d\n",omp_get_level());

printf("omp_get_team_size = %d\n",omp_get_team_size(0));

printf("omp_get_num_devices = %d\n",omp_get_num_devices());

printf("omp_get_wtime = %f\n",omp_get_wtime());
*/
#ifdef LOMP
printf("\nOpenMP Running\n\n");
#pragma message "OpenMP compiling "  __FILE__ 
#endif
#ifdef LACC
printf("\nOpenACC Running\n\n");
#pragma message  "OpenACC compiling "  __FILE__ 
#endif
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
int thread_size=blocksize_x*blocksize_y*blocksize_z;
//window size
int window_size=4;

size_t meret=sizex*sizey*sizez;
printf("meret: %lu , %f Gb \n",meret,meret*4/1e9f*3);

Spawn_stopper("offload and memory managment");

//data and offloading OpenMP

//this we will use on the cpu
float* out= (float*) malloc(meret*4);
float* out2= (float*) malloc(meret*4);

#ifdef LOMP
float* data=NULL ;//4= (float*) malloc(meret*4);

#pragma omp target data map(alloc:data[0:meret])\
                        map(from:out2[0:meret])\
                        map(from:out[0:meret])\
        map(to:sizex) map(to:sizey) map(to:sizez) map(to:blocksize_x) map(to:blocksize_y) map(to:blocksize_z) map(to:window_size)                                                    
{

#endif

//data and offloading OpenACC
#ifdef LACC
#pragma acc data copyout(out[0:meret]) copyout(out2[0:meret])  \
copy(sizex,sizey,sizez,blocksize_x,blocksize_y,blocksize_z,window_size)
{
float* restrict data=(float*)acc_malloc(meret*4);
//float* restrict out=(float*)acc_malloc(meret);
//float* restrict out2=(float*)acc_malloc(meret);

#endif

Kill_stopper();

float(*__restrict data_)[sizey][sizez] = (float(*)[sizey][sizez])data;
float(*__restrict out_)[sizey][sizez] = (float(*)[sizey][sizez])out;
float(*__restrict out2_)[sizey][sizez] = (float(*)[sizey][sizez])out2;

//printf("size meret %d \n",meret);



////////////////////////////////////////////////////////////////////////////////////////////////////////////

Spawn_stopper("3d zeroing");
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#endif
#ifdef LACC
#pragma acc parallel loop collapse(3) deviceptr(data_) //deviceptr(out_,out2_,data_)
// megjegyzés ha csak a gpun allocálunk akkor kell deviceptr, ha nem csak ott akkor nem fog működni futásnál
#endif
    for(int x=0;x<sizex;x++)
    {
        for(int y=0;y<sizey;y++)
        {
            for(int z=0;z<sizez;z++)
            {
                data_[x][y][z]=1;
                out_[x][y][z]=0;
                out2_[x][y][z]=0;
                //printf("%f\n",data_[x][y][z]);
                //#ifdef LOMP
                //#endif
            }        
        }
    }

Kill_stopper();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Spawn_stopper("3d computation");
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3) 
#endif
#ifdef LACC
#pragma acc parallel loop collapse(3) deviceptr(data_)
#endif
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

#ifdef LACC
Spawn_stopper("3d computation with tile parancs");
#pragma acc parallel loop deviceptr(data_) tile(32,8,4)
for(int x=window_size;x<sizex-window_size;x++)
    {
        for(int y=window_size;y<sizey-window_size;y++)
        {
            for(int z=window_size;z<sizez-window_size;z++)
            {
                //kernel start
                out2_[x][y][z]+=
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
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef LOMP
//This is a small scale testing.
omp_set_num_threads(1024);
//omp_get_max_threads();
printf("omp_get_max_threads = %d\n",omp_get_max_threads());
Spawn_stopper("3 team loop of 3 thread loops");
#pragma omp target teams distribute collapse(2) thread_limit(1024) // run on the teamleaders
    for (int x = 0; x < 1; x += 1)
    {
        for (int y = 0; y < 1; y += 1)
        {
            //for (int z = 0; z < 2; z += 1)
            printf("just a teamleader\n");
                #pragma omp parallel for collapse(1)// run on the threads
                for (int i = 0; i < 2; i+=1)
                {
                    printf("omp_get_num_threads = %d\n",omp_get_num_threads());
                    int bid=omp_get_team_num();
                    int tid=omp_get_thread_num();
                    printf("bid %d, tid %d\n",bid,tid);
                    /*for (int j = 0; j < 2; j+=1)
                    {
                        for (int k = 0; k < 2; k+=1)
                        {
                            printf("xyzijk: %d %d %d %d %d %d\n",x,y,z,i,j,k);
                        }
                    }*/
                }
            
        }
    }
Kill_stopper();

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef LOMP
omp_set_num_threads(1024);
printf("omp_get_level = %d\n",omp_get_level());

Spawn_stopper("3d tiling with omp teams");
//#pragma omp target teams
//#pragma omp distribute parallel for collapse(3) 
#pragma omp target teams distribute collapse(3)  thread_limit(1024)
    for (int x = window_size; x < sizex-window_size; x += blocksize_x)
        for (int y = window_size; y < sizey-window_size; y += blocksize_y)
            for (int z = window_size; z < sizez-window_size; z += blocksize_z)
            {
    
                #pragma omp parallel for collapse(3)
                for (int bx = x; bx < x + blocksize_x; bx++)
                    for (int by = y; by < y + blocksize_y; by++)
                        for (int bz = z; bz < z + blocksize_z; bz++)
                        {
                            if (bx < sizex-window_size &&
                                by < sizey-window_size &&
                                bz < sizez-window_size){
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
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#ifdef LOMP

//sizex = 5;
//sizey = 5;
//sizez = 5;

//int bsx=2,bsy=2,bsz=2;


int blockdbx=sizex/blocksize_x+((sizex%blocksize_x!=0)?1:0);
int blockdby=sizey/blocksize_y+((sizey%blocksize_y!=0)?1:0);
int blockdbz=sizez/blocksize_z+((sizez%blocksize_z!=0)?1:0);
printf("block number dim %d %d %d\n",blockdbx,blockdby,blockdbz);
printf("thread_num %d \n",blocksize_x*blocksize_y*blocksize_z);
printf("team_num %d \n",blockdbx*blockdby*blockdbz);
int team_num=blockdbx*blockdby*blockdbz;
int thread_limit=blocksize_x*blocksize_y*blocksize_z;


Spawn_stopper("2 loop blocking hack");
#pragma omp target teams distribute parallel for collapse(2) thread_limit(thread_limit) private(blockdbx,blockdbx,blockdbx)
for(int bid=0;bid<team_num;bid++)
    for(int tid=0;tid<thread_limit;tid++)
    {

    //bid=omp_get_team_num();
    //tid=omp_get_thread_num();
    int x,y,z;
    //printf("nyersen %d %d\n",bid,tid);

    z=bid/(blockdbx*blockdby)*blocksize_z;
    bid-=(bid/(blockdbx*blockdby))*blockdbx*blockdby;
    y=(bid/blockdbx)*blocksize_y;
    x=bid%blockdbx*blocksize_x;
    z+=tid/(blocksize_x*blocksize_y);
    tid-=(tid/(blocksize_x*blocksize_y))*(blocksize_x*blocksize_y);
    //printf("%d\n",tid);
    y+=(tid/blocksize_x);
    x+=tid%blocksize_x;
    //printf("kord %d %d %d\n",x,y,z);
   
    if(omp_get_team_num()==65535)
    {
        printf(" %d,%d,%d   \n",x,y,z);
    }
    if(x==0 && y==0 && z==12)
                {
                    printf(" %d,%d,%d   \n",x,y,z);
                }


    //kernel start
    // sum for the scope of the window..
    if(x >= window_size && x < sizex-window_size &&
                    y >= window_size && y < sizey-window_size &&
                    z >= window_size && z < sizez-window_size){

                        out2_[x][y][z]+=
				data_[x][y][z-4]+data_[x][y][z-3]+data_[x][y][z-2]+data_[x][y][z-1]+
				data_[x][y][z+4]+data_[x][y][z+3]+data_[x][y][z+2]+data_[x][y][z+1]+
				data_[x][y-4][z]+data_[x][y-3][z]+data_[x][y-2][z]+data_[x][y-1][z]+
				data_[x][y+4][z]+data_[x][y+3][z]+data_[x][y+2][z]+data_[x][y+1][z]+
				data_[x-4][y][z]+data_[x-3][y][z]+data_[x-2][y][z]+data_[x-1][y][z]+
				data_[x+4][y][z]+data_[x+3][y][z]+data_[x+2][y][z]+data_[x+1][y][z];
    }

           //printf("ertek:%d %d %d:  %f \n",x,y,z,out2_[x][y][z]);
    //kernel end

    }

    Kill_stopper();

#endif

*/




/*
#ifdef LOMP

sizex = 5;
sizey = 5;
sizez = 5;

int bsx=2,bsy=2,bsz=2;

int blockdbx=sizex/bsx+1;
int blockdby=sizey/bsy+1;
int blockdbz=sizez/bsz+1;


#pragma omp target teams num_teams(blockdbx*blockdby*blockdbz) thread_limit(bsx*bsy*bsz)
#pragma omp parallel 
{
    int bid,tid;
    bid=omp_get_team_num();
    tid=omp_get_thread_num();
    int x,y,z;

    z=bid/(blockdbx*blockdby)*bsz;
    bid-=(bid/(blockdbx*blockdby))*blockdbx*blockdby;
    y=(bid/blockdbx)*bsy;
    x=bid%blockdbx*bsx;
    printf("nyersen %d %d %d\n",x,y,z);
    z+=tid/(bsx*bsy);
    tid-=(tid/(bsx*bsy))*(bsx*bsy);
    //printf("%d\n",tid);
    y+=(tid/bsx);
    x+=tid%bsx;




//    y+=(tid/bsx);
  //  x+=tid%bsx;
    //printf("asd\n");
    //printf("omp_get_num_threads = %d\n",omp_get_num_threads());
    //printf("ez = %d\n",omp_get_team_num());
    printf("%d %d %d\n",x,y,z);
}




#endif

*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////


Spawn_stopper("back to ram");
}
Kill_stopper();

//data verification
//#ifdef DATA_V

float(*__restrict out2_)[sizey][sizez] = (float(*)[sizey][sizez])out2;
float(*__restrict out_)[sizey][sizez] = (float(*)[sizey][sizez])out;


//size_t m=sizex*sizey*sizez;
bool valid=true;
for(int x=0;x<sizex;x++)
{
    for(int y=0;y<sizey;y++)
    {
        for(int z=0;z<sizez;z++)
        {
            if(out_[x][y][z]!=out2_[x][y][z])
            {
                valid=false;
                printf("hibás ertek:%d %d %d:  %f , %f\n",x,y,z,out_[x][y][z],out2_[x][y][z]);
                goto end_test;
            }else
            {
                //printf("ertek:%d %d %d:  %f , %f\n",x,y,z,out_[x][y][z],out2_[x][y][z]);
            }
        }
    }
}

end_test:
printf("valid:%d\n",valid);
if(valid)
printf("out and out2 is equal.\n");
else
printf("out testing is failed.\n");

//float(*__restrict out2_)[sizey][sizez] = (float(*)[sizey][sizez])out2;
printf("ertek: %f\n",out2_[5][5][5]);
//#endif


/*{
    
    #pragma omp teams distribute parallel for 
    for(int i=0;i<meret;i++)
    {
        data[i]=data[i]+1;
    }
}*/


}
