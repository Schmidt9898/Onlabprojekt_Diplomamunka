
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
int sizex = 500;
int sizey = 500;
int sizez = 500;

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




int blocksize_x=1;
int blocksize_y=1;
int blocksize_z=1;
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
    for(int x=0;x<sizex;x++)
    {
        for(int y=0;y<sizey;y++)
        {
            for(int z=0;z<sizez;z++)
            {
                //kernel start
                
                //printf("x:%d,y:%d,z:%d \n",x,y,z);
                
                // sum for the scope of the window..
                for(int ibx=x-window_size;ibx<x+window_size;ibx++)
                for(int iby=y-window_size;iby<y+window_size;iby++)
                for(int ibz=z-window_size;ibz<z+window_size;ibz++)
                    {
                    if(ibx>=0 && ibx < sizex &&
                        iby>=0 && iby < sizey &&
                        ibz>=0 && ibz < sizez){
                            out_[x][y][z]+=data_[ibx][iby][ibz];
                            //printf("good?\n");
                        }else
                        {
                            //printf("not good\n");
                        }
                }
                        //printf("x:%d,y:%d,z:%d   val: %f\n",x,y,z,out_[x][y][z]);
                //kernel end
            }        
        }
    }
Kill_stopper();


/*
Spawn_stopper("3d tiling with computation");
#ifdef LOMP
#pragma omp target teams //num_teams(128) thread_limit(1024) 
#pragma omp distribute parallel for collapse(6)
#endif
#ifdef LACC
#pragma acc parallel deviceptr(out_,data_) num_gangs(65535) vector_length(256)
#pragma acc loop collapse(6) 
    for (int x = 0; x < sizex; x += blocksize_x)
    {
        for (int y = 0; y < sizey; y += blocksize_y)
        {
            for (int z = 0; z < sizez; z += blocksize_z)
            {
                //#pragma omp parallel for collapse(3)
                for (int bx = x; bx < x + blocksize_x; bx++)
                {
                    for (int by = y; by < y + blocksize_y; by++)
                    {
                        for (int bz = z; bz < z + blocksize_z; bz++)
                        {
                            //kernel starts here
                            // sum for the scope of the window..
                            if (bx >= 0 && bx < sizex &&
                                by >= 0 && by < sizey &&
                                bz >= 0 && bz < sizez)
                            {
                                for (int ibx = bx - window_size; ibx <= (bx + window_size); ibx++)
                                    for (int iby = by - window_size; iby <= (by + window_size); iby++)
                                        for (int ibz = bz - window_size; ibz <= (bz + window_size); ibz++)
                                        {
                                            if (ibx >= 0 && ibx < sizex &&
                                                iby >= 0 && iby < sizey &&
                                                ibz >= 0 && ibz < sizez)
                                            {
                                                //printf("talált  ");

                                                //out_[bx][by][bz] += data_[ibx][iby][ibz];
                                                
                                                //out_[bx][by][bz]=0;
                                            }

                                            //printf("x:%d,y:%d,z:%d\n",ibx,iby,ibz);
                                        }
                            }//kernel ends here
                        }
                    }
                }
            }
        }
    }

#endif
Kill_stopper();
*/


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
int left=65535;
Spawn_stopper("blocking");
//#pragma omp task
/*for(int team_i=0;team_i<team_num;team_i+=65535)
{
if(team_i+65535>team_num)
    left=team_i-65535;*/
#pragma omp target teams num_teams(984375) thread_limit(blocksize_x*blocksize_y*blocksize_z)
#pragma omp parallel  
{
    int bid,tid;
    bid=omp_get_team_num();
    tid=omp_get_thread_num();
    int x,y,z;

    z=bid/(blockdbx*blockdby)*blocksize_z;
    bid-=(bid/(blockdbx*blockdby))*blockdbx*blockdby;
    y=(bid/blockdbx)*blocksize_y;
    x=bid%blockdbx*blocksize_x;
    //printf("nyersen %d %d %d\n",x,y,z);
    z+=tid/(blocksize_x*blocksize_y);
    tid-=(tid/(blocksize_x*blocksize_y))*(blocksize_x*blocksize_y);
    //printf("%d\n",tid);
    y+=(tid/blocksize_x);
    x+=tid%blocksize_x;
    
    /*if(omp_get_team_num()==65535)
    {
        printf(" %d,%d,%d   \n",x,y,z);
    }*/
    /*if(x==0 && y==0 && z==12)
                {
                    printf(" %d,%d,%d   \n",x,y,z);
                }*/


    //printf("kord %d %d %d\n",x,y,z);
    //kernel start
    // sum for the scope of the window..
    for (int ibx = x - window_size; ibx < x + window_size; ibx++){
        for (int iby = y - window_size; iby < y + window_size; iby++){
            for (int ibz = z - window_size; ibz < z + window_size; ibz++)
            {
                /*if(x==0 && y==0 && z==12)
                {
                    printf(" %lu,%lu,%lu   ",ibx,iby,ibz);
                }*/


                //printf(" %lu,%lu,%lu   ",bx,by,bz);
                if (ibx >= 0 && ibx < sizex &&
                    iby >= 0 && iby < sizey &&
                    ibz >= 0 && ibz < sizez &&
                    x >= 0 && x < sizex &&
                    y >= 0 && y < sizey &&
                    z >= 0 && z < sizez)
                {
                    out2_[x][y][z] += data_[ibx][iby][ibz];
                }else
                {
                    //printf("NOOOO\n");
                }
            }
        }
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
