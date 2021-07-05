#include <iostream>
#include "Timer.h"

#include "stdlib.h"
#include "omp.h"


int main(int argc, char ** argv) {
Timer_Filemode=false;
Timer_Reset();
std::cout<<"Hi\n";
uint meret=1000000000;
float* data = new float[meret];
#pragma omp target map(from:data[0:meret])
{
Timer a("1");
#pragma omp teams distribute parallel for 
    for(uint i=0;i<meret;i++)
    {
        data[i]=0;
    }
}
{
    Timer a("2");
    #pragma omp teams distribute parallel for 
    for(uint i=0;i<meret;i++)
    {
        data[i]=data[i]+1;
    }
}

/*
float sum=0;
{
    Timer a("3");
    for(uint i=0;i<250000000;i++)
    {
        sum+=data[i]/250000000;
    }
}*/
//std::cout<<"sum "<<sum<<"\n";
}