#include <stdio.h>
//#include <cuda.h>
//#include <cuda_runtime.h>

#include "cuda_section0.h"


void cuda_section0(){
	printf("section0 cpu\n");
}

void kernel_section0()
{
	cuda_section0();
	//cudaDeviceSynchronize();
}


