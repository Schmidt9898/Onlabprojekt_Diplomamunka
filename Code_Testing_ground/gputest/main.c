
#include "stdlib.h"
#include <stdio.h>

#include "omp.h"
#include "stopper.h"


/* constans méretek */
size_t sizex = 16;
size_t sizey = 16;
size_t sizez = 16;

struct dataobj
{
  void* data;
};



int main(int argc, char ** argv) {
Stopper_Filemode=false;
printf("Hi this is the openMP testing..\n\n");

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
printf("\n---------------------gpu---------------------------\n\n");
/*

printf(" = %d\n",);

printf(" = %d\n",);


*/




int blocksize_x=8;
int blocksize_y=8;
int blocksize_z=8;

//window size
int window_size=4;

//int blocksize=8;
//int blockhalf=blocksize/2;
size_t meret=sizex*sizey*sizez*4;
printf("meret: %f Gb \n",meret/1e9f);
float* data=NULL ;//= (float*) malloc(meret*4);
float* out= (float*) malloc(meret*4);
Spawn_stopper("offload");
#pragma omp target data map(alloc:data[0:sizex*sizey*sizez]) map(from:out[0:sizex*sizey*sizez]) map(to:sizex) map(to:sizey) map(to:sizez) map(to:blocksize_x) map(to:blocksize_y) map(to:blocksize_z) map(to:window_size)
{

Kill_stopper();
float(*__restrict data_)[sizey][sizez] = (float(*)[sizey][sizez])data;
float(*__restrict out_)[sizey][sizez] = (float(*)[sizey][sizez])out;

//printf("size meret %d \n",meret);





Spawn_stopper("3d zeroing");
#pragma omp target teams
{
    //printf("size0,size1  %d",size1);
#pragma omp distribute parallel for collapse(3)
    for(size_t x=0;x<sizex;x++)
    {
        for(size_t y=0;y<sizey;y++)
        {
            for(size_t z=0;z<sizez;z++)
            {
                out_[x][y][z]=0;
                data_[x][y][z]=1;
            }        
        }
    }
} 
Kill_stopper();

/*

Spawn_stopper("3d computation");
#pragma omp target teams //num_teams(128) thread_limit(8*8*8) 
{
    //printf("size0,size1  %d",size1);
#pragma omp distribute parallel for collapse(3)
    for(size_t x=0;x<sizex;x++)
    {
        for(size_t y=0;y<sizey;y++)
        {
            for(size_t z=0;z<sizez;z++)
            {
                //out_[x][y][z]=3;
                  //                      printf("%f",2.0);
                //print("asd"); //ernel start
               // sum for the scope of the window..
                for(int ibx=x-window_size;ibx<x+window_size;ibx++)
                for(int iby=y-window_size;iby<y+window_size;iby++)
                for(int ibz=z-window_size;ibz<z+window_size;ibz++)
                    {
                        //printf(" %lu,%lu,%lu   ",bx,by,bz);
                    if(ibx>=0 && ibx < sizex &&
                        iby>=0 && iby < sizey &&
                        ibz>=0 && ibz < sizez){
                            out_[x][y][z]+=data_[ibx][iby][ibz];
                        }
                }
                //kernel end
            }        
        }
    }
} 
Kill_stopper();
*/
/*
Spawn_stopper("3d computation collapse 2 simd");
#pragma omp target teams //num_teams(128) thread_limit(8*8*8) 
{
    //printf("size0,size1  %d",size1);
    //#pragma omp target simd
#pragma omp distribute parallel for collapse(3)
    for(size_t x=0;x<sizex;x++)
    {
        for(size_t y=0;y<sizey;y++)
        { 
            for(size_t z=0;z<sizez;z++)
            {
                //print("asd"); //ernel start
               // sum for the scope of the window..
                for(int ibx=x-window_size;ibx<x+window_size;ibx++)
                for(int iby=y-window_size;iby<y+window_size;iby++)
                for(int ibz=z-window_size;ibz<z+window_size;ibz++)
                    {
                        //printf(" %lu,%lu,%lu   ",bx,by,bz);
                    if(ibx>=0 && ibx < sizex &&
                        iby>=0 && iby < sizey &&
                        ibz>=0 && ibz < sizez){
                            out_[x][y][z]+=data_[ibx][iby][ibz];
                        }
                }
                //kernel end
            }        
        }
    }
} 
Kill_stopper();

*/


/*
int szamlalo=0;
#pragma omp target teams //reduction(+ : szamlalo)
{
#pragma omp distribute parallel for //reduction(+ : szamlalo)
    for (int x = 0; x < 3; x += 1)
    {
#pragma omp parallel for //reduction(+ : szamlalo)
        for (int y = 0; y < 3; y += 1)
        {
            szamlalo++;
            printf("%d,%d\n",x,y);
        }
    }
}
printf("Szamlalo: %d\n",szamlalo);
*/
Spawn_stopper("3d tiling with computation");
#pragma omp target teams //num_teams(128) thread_limit(1024) 
{
    //printf("size0,size1  %d",size1);
#pragma omp distribute parallel for collapse(3)
    for (long x = 0; x < sizex; x += blocksize_x)
    {
        for (long y = 0; y < sizey; y += blocksize_y)
        {
            for (long z = 0; z < sizez; z += blocksize_z)
            {
                //#pragma omp distribute parallel for collapse(3)
                for (long bx = x; bx < x + blocksize_x; bx++)
                {
                    for (long by = y; by < y + blocksize_y; by++)
                    {
                        for (long bz = z; bz < z + blocksize_z; bz++)
                        {
                                        out_[bx][by][bz]=2;
                                        printf("%f",2.0);
                            //printf("--------x:%d,y:%d,z:%d\n",bx,by,bz);
                            // sum for the scope of the window..
                            if (bx >= 0 && bx < sizex &&
                                by >= 0 && by < sizey &&
                                bz >= 0 && bz < sizez)
                            {
                               /* for (int ibx = bx - window_size; ibx <= (bx + window_size); ibx++)
                                    for (int iby = by - window_size; iby <= (by + window_size); iby++)
                                        for (int ibz = bz - window_size; ibz <= (bz + window_size); ibz++)
                                        {
                                            if (ibx >= 0 && ibx < sizex &&
                                                iby >= 0 && iby < sizey &&
                                                ibz >= 0 && ibz < sizez)
                                            {
                                                //printf("talált  ");

                                                out_[bx][by][bz] += data_[ibx][iby][ibz];
                                                //out_[bx][by][bz]=0;
                                            }

                                            //printf("x:%d,y:%d,z:%d\n",ibx,iby,ibz);
                                        }*/
                            }
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

//data verification
float(*__restrict out_)[sizey][sizez] = (float(*)[sizey][sizez])out;
printf("ertek: %f\n",out_[10][10][10]);

/*{
    
    #pragma omp teams distribute parallel for 
    for(int i=0;i<meret;i++)
    {
        data[i]=data[i]+1;
    }
}*/


}
