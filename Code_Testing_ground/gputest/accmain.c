

#include "stdlib.h"
#include "omp.h"
#include "openacc.h"
#include <stdio.h>
#include "stopper.h"

//sript avarage 3d computation without tiling
double meres_avg=0.5810;
double meres_sum=0.0;
int meres_num=3;


/* constans méretek */
size_t sizex = 800;//16*20;
size_t sizey = 800;//16*20;
size_t sizez = 800;//16*20;

struct dataobj
{
  void* data;
};



int main(int argc, char ** argv) {
Stopper_Filemode=false;
printf("\nHi this is the openACC testing..\n\n");

printf("\n---------------------gpu---acc---------------------\n\n");
/*

printf(" = %d\n",);

printf(" = %d\n",);

*/

int blocksize_x=1;
int blocksize_y=1;
int blocksize_z=1;
if(argc>=4){
    int blocksize_x=atoi(argv[1]);
    int blocksize_y=atoi(argv[2]);
    int blocksize_z=atoi(argv[3]);
}
//int blockhalf=blocksize/2;
//printf("%d\n%d\n%d\n",blocksize_x,blocksize_y,blocksize_z);
//window size
int window_size=2;

size_t meret=sizex*sizey*sizez*4;
printf("meret: %f Gb \n",meret/1e9f);
//float* data=NULL; //(float*) malloc(meret);
//float* out=NULL; // (float*) malloc(meret);

//Spawn_stopper("offload");
#pragma acc data copy(sizex,sizey,sizez,blocksize_x,blocksize_y,blocksize_z,window_size)
{
float* restrict data=(float*)acc_malloc(meret);
float* restrict out=(float*)acc_malloc(meret);
float(*__restrict data_)[sizey][sizez] = (float(*)[sizey][sizez])data;
float(*__restrict out_)[sizey][sizez] = (float(*)[sizey][sizez])out;

//Kill_stopper();
//printf("size meret %lu \n",meret);




//Spawn_stopper("3d zeroing");
/*
#pragma acc parallel loop collapse(3) deviceptr(out_,data_)
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
*/
//Kill_stopper();


Spawn_stopper("3d computation");

    //printf("size0,size1  %d",size1);
#pragma acc parallel loop deviceptr(out_,data_) tile(32,8,4)
    for(int x=0;x<sizex;x++)
    {
        for(int y=0;y<sizey;y++)
        {
            for(int z=0;z<sizez;z++)
            {
                //print("asd");
                // sum for the scope of the window..
                #pragma unroll
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
            }        
        }
    }

Kill_stopper();


/*
for(int i=0;i<meres_num;i++){

Spawn_stopper("3d tiling with computation");
#pragma acc parallel deviceptr(out_,data_) //present(sizex,sizey,sizez,blocksize_x,blocksize_y,blocksize_z,window_size)
   {
    //printf("size0,size1  %d",size1);
#pragma acc loop collapse(6)
for(int x=0;x<sizex;x+=blocksize_x)
    {
        for(int y=0;y<sizey;y+=blocksize_y)
        {
            for(int z=0;z<sizez;z+=blocksize_z)
            {
               //#pragma acc loop collapse(3) 
                for(int bx=x;bx<x+blocksize_x;bx++)
                {
                    for(int by=y;by<y+blocksize_y;by++)
                    {
                        for(int bz=z;bz<z+blocksize_z;bz++)
                        {
                            // sum for the scope of the window..
                            if(bx>=0 && bx < sizex &&
                                   by>=0 && by < sizey &&
                                   bz>=0 && bz < sizez)
                            for(int ibx = bx - window_size; ibx <= (bx+window_size); ibx++)
                            for(int iby = by - window_size; iby <= (by+window_size); iby++)
                            for(int ibz = bz - window_size; ibz <= (bz+window_size); ibz++)
                                {
                                if(ibx>=0 && ibx < sizex &&
                                   iby>=0 && iby < sizey &&
                                   ibz>=0 && ibz < sizez){
                                    //printf("talált  ");
                                    
                                    out_[bx][by][bz]+=data_[ibx][iby][ibz];
                                    //out_[bx][by][bz]=0;

                                   }

                                //printf("x:%d,y:%d,z:%d\n",ibx,iby,ibz);
                                    

                                }
                            //printf("--------x:%d,y:%d,z:%d\n",x,y,z);
                        }
                    }
                }
            }        
        }
    }
} 
meres_sum+=Kill_stopper();

}
*/
/*
Spawn_stopper("3d with tiling pragma with computation");
 //#pragma acc parallel 
   // {
    //printf("size0,size1  %d",size1);
#pragma acc parallel loop tile(16,16,16) deviceptr(out_,data_)
    for(size_t x=0;x<sizex;x++)
    {
        for(size_t y=0;y<sizey;y++)
        {
            for(size_t z=0;z<sizez;z++)
            {
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
                        
            }        
        }
    }
//} 
Kill_stopper();
*/

//Spawn_stopper("back to ram");
}
//Kill_stopper();

//meres kiértékelés
meres_sum/=meres_num;
meres_sum-=meres_avg;
//printf("a meres: %f\n",meres_sum);
FileStream *Fs=MakeFileStream("Tiling_meresek.csv");
char bev[30];
sprintf(bev,"%d,%d,%d,%lf\n",blocksize_x,blocksize_y,blocksize_z,meres_sum);
printf("%s",bev);
Fs->Write(Fs,bev);
Fs->Flush(Fs);

}
