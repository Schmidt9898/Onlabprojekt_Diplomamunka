/*
#include <iostream>
*/
#include "stopper.h"
#include "stdlib.h"
#include "omp.h"
#include <stdio.h>


int main(int argc, char ** argv) {
Spawn_stopper("main");
//Stopper_Filemode=false;

printf("hi\n"); //std::cout<<"Hi\n";
uint meret=1000000000; // this will be 4 gigabyte
float* data = malloc(4*meret); //new float[meret];
Spawn_stopper("off");
#pragma omp target data map(tofrom:data[0:meret])
{
Kill_stopper();
Spawn_stopper("first");
#pragma omp target teams distribute parallel for
    for(uint i=0;i<meret;i++)
    {
        data[i]=0;
    }
Kill_stopper();
Spawn_stopper("secound");
#pragma  omp target teams distribute parallel for
    for(uint i=0;i<meret;i++)
    {
        //Spawn_stopper("sss");
        data[i]=data[i]+1;
        //Kill_stopper();
    }
Kill_stopper();
Spawn_stopper("reload");
}
Kill_stopper();
free(data);
Kill_stopper();
}