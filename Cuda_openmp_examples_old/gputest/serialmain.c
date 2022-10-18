

#include "stdlib.h"
//#include "omp.h"
#include <stdio.h>
#include "stopper.h"

//#define addres int int 
/* constans méretek */
size_t sizex = 100;
size_t sizey = 100;
size_t sizez = 50;

struct dataobj
{
  void* data;
};



int main(int argc, char ** argv) {
Stopper_Filemode=false;
printf("\n\nHi this is the Serial testing..\n\n");


printf("\n---------------------cpu---------------------------\n\n");
/*

printf(" = %d\n",);

printf(" = %d\n",);


*/





int blocksize=16;
//int blockhalf=blocksize/2;
int avg_window=4;


size_t meret=sizex*sizey*sizez*4;
printf("meret: %lu \n",meret);
float* data= (float*) malloc(meret);
float* out= (float*) malloc(meret);
Spawn_stopper("offload");


Kill_stopper();
float(*__restrict data_)[sizey][sizez] = (float(*)[sizey][sizez])data;
float(*__restrict out_)[sizey][sizez] = (float(*)[sizey][sizez])out;

printf("size meret %lu \n",meret);

Spawn_stopper("3d zeroing");

    //printf("size0,size1  %d",size1);
    for(size_t x=0;x<sizex;x++)
    {
        for(size_t y=0;y<sizey;y++)
        {
            for(size_t z=0;z<sizez;z++)
            {
                //printf("%lu,%lu,%lu  ",x,y,z);
                out_[x][y][z]=0;
                data_[x][y][z]=1;
            }        
        }
    }

Kill_stopper();
/*
Spawn_stopper("3d computation");
    for(int x=0;x<sizex;x++)
    {
        for(int y=0;y<sizey;y++)
        {
            for(int z=0;z<sizez;z++)
            {
                //print("asd");
                // sum for the scope of the window..
                for(int ibx=x-avg_window;ibx<x+avg_window;ibx++)
                for(int iby=y-avg_window;iby<y+avg_window;iby++)
                for(int ibz=z-avg_window;ibz<z+avg_window;ibz++)
                    {
                        //printf(" %lu,%lu,%lu   ",bx,by,bz);
                    if(ibx>=0 && ibx < sizex &&
                        iby>=0 && iby < sizey &&
                        ibz>=0 && ibz < sizez)
                            out_[x][y][z]+=data_[ibx][iby][ibz];
                    }
                        
            }        
        }
    }

Kill_stopper();
*/
Spawn_stopper("3d tiling with computation");
    for(int x=0;x<sizex;x+=blocksize)
    {
        for(int y=0;y<sizey;y+=blocksize)
        {
            for(int z=0;z<sizez;z+=blocksize)
            {
               // #pragma omp parallel
                //#pragma omp for collapse(3) 
                for(int bx=x;bx<x+blocksize;bx++)
                {
                    for(int by=y;by<y+blocksize;by++)
                    {
                        for(int bz=z;bz<z+blocksize;bz++)
                        {
                            // sum for the scope of the window..
                            if(bx>=0 && bx < sizex &&
                                   by>=0 && by < sizey &&
                                   bz>=0 && bz < sizez)
                            for(int ibx=bx-avg_window;ibx<=bx+avg_window;ibx++)
                            for(int iby=by-avg_window;iby<=by+avg_window;iby++)
                            for(int ibz=bz-avg_window;ibz<=bz+avg_window;ibz++)
                                {
                                if(ibx>=0 && ibx < sizex &&
                                   iby>=0 && iby < sizey &&
                                   ibz>=0 && ibz < sizez){
                                    //printf("talált  ");
                                    out_[bx][by][bz]+=data_[ibx][iby][ibz];

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
//} 
Kill_stopper();

printf("ertek: %f\n",out_[10][10][10]);

Spawn_stopper("back to ram");

Kill_stopper();

// optimizer hack
printf("asd:%f\n",out_[0][0][0]);
}