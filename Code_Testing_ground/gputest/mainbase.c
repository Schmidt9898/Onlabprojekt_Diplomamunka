
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
int sizex = 800;
int sizey = 800;
int sizez = 800;

struct dataobj
{
  void* data;
};



int main(int argc, char ** argv) {
Stopper_Filemode=false;
//printf("Hi this is the openMP testing..\n\n");

//following block try all the functions and list properties
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
#pragma message ( "OpenMP compiling." )
#endif
#ifdef LACC
printf("\nOpenACC Running\n\n");
#pragma message ( "OpenACC compiling." )
#endif
/*

printf(" = %d\n",);

printf(" = %d\n",);


*/




int blocksize_x=32;
int blocksize_y=8;
int blocksize_z=4;
if(argc>=4){
    blocksize_x=atoi(argv[1]);
    blocksize_y=atoi(argv[2]);
    blocksize_z=atoi(argv[3]);
}


//printf("omp_get_max_threads = %d\n",omp_get_max_threads());
//window size
int window_size=4;

size_t meret=sizex*sizey*sizez*4;
printf("meret: %f Gb \n",meret/1e9f*3);

Spawn_stopper("offload and memory managment");

//data and offloading OpenMP
#ifdef LOMP
float* data=NULL ;//= (float*) malloc(meret*4);
float* out= (float*) malloc(meret*4);
float* out2= (float*) malloc(meret*4);

#pragma omp target data map(alloc:data[0:sizex*sizey*sizez])  map(from:out2[0:sizex*sizey*sizez]) map(from:out[0:sizex*sizey*sizez]) map(to:sizex) map(to:sizey) map(to:sizez) map(to:blocksize_x) map(to:blocksize_y) map(to:blocksize_z) map(to:window_size)
                                                            
{

float(*__restrict out2_)[sizey][sizez] = (float(*)[sizey][sizez])out2;
#endif


#ifdef LACC
#pragma acc data copy(sizex,sizey,sizez,blocksize_x,blocksize_y,blocksize_z,window_size)
{
float* restrict data=(float*)acc_malloc(meret);
float* restrict out=(float*)acc_malloc(meret);
#endif

Kill_stopper();

float(*__restrict data_)[sizey][sizez] = (float(*)[sizey][sizez])data;
float(*__restrict out_)[sizey][sizez] = (float(*)[sizey][sizez])out;

//printf("size meret %d \n",meret);





Spawn_stopper("3d zeroing");
#ifdef LOMP
#pragma omp target teams
#pragma omp distribute parallel for collapse(3)
#endif
#ifdef LACC
#pragma acc parallel loop collapse(3) deviceptr(out_,data_)
#endif
    for(int x=0;x<sizex;x++)
    {
        for(int y=0;y<sizey;y++)
        {
            for(int z=0;z<sizez;z++)
            {
                out_[x][y][z]=1;
                data_[x][y][z]=1;
                
                #ifdef LOMP
                out2_[x][y][z]=1;
                #endif
            }        
        }
    }

Kill_stopper();


Spawn_stopper("3d computation");
#ifdef LOMP
//num_teams(128) thread_limit(8*8*8) 
//printf("size0,size1  %d",size1);
#pragma omp target teams //num_teams(65535) thread_limit(32)
#pragma omp distribute parallel for collapse(3) 
#endif
#ifdef LACC
#pragma acc parallel loop collapse(3) deviceptr(out_,data_)
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



Spawn_stopper("3d computation with tile() parancs");
#ifdef LOMP
#pragma omp target teams //num_teams(128) thread_limit(1024) 
#pragma omp distribute parallel for collapse(3) tile(sizex,sizey,sizez)
#endif
#ifdef LACC
#pragma acc parallel loop deviceptr(out_,data_) tile(32,8,4)
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
   /*
    if(omp_get_team_num()==65535)
    {
        printf(" %d,%d,%d   \n",x,y,z);
    }
    if(x==0 && y==0 && z==12)
                {
                    printf(" %d,%d,%d   \n",x,y,z);
                }
*/

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






/*

Spawn_stopper("6 collapsed loop");
#pragma omp target teams //num_teams(128) thread_limit(1024) 
{
    //printf("size0,size1  %d",size1);
    //int num2=2;
#pragma omp distribute parallel for collapse(3) //private(num2)
    for (int x = 0; x < 2; x += 1)
    {
        for (int y = 0; y < 2; y += 1)
        {
            for (int z = 0; z < 2; z += 1)
            {
                #pragma omp parallel for collapse(3)
                for (int i = 0; i < 2; i+=1)
                {
                    for (int j = 0; j < 2; j+=1)
                    {
                        for (int k = 0; k < 2; k+=1)
                        {
                            printf("xyzijk: %d %d %d %d %d %d\n",x,y,z,i,j,k);
                        }
                    }
                }
            }
        }
    }
}
Kill_stopper();
*/
Spawn_stopper("back to ram");
}
Kill_stopper();

//data verification only in openmp for now
#ifdef LOMP

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
printf("ertek: %f\n",out2_[0][0][0]);
#endif


/*{
    
    #pragma omp teams distribute parallel for 
    for(int i=0;i<meret;i++)
    {
        data[i]=data[i]+1;
    }
}*/


}
