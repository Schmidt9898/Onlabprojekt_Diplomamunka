
#include "forward.h"

#include "stdlib.h"
#include <stdio.h>
#include <math.h>
#include "omp.h"

typedef struct dataobj dataobj;//WHY?????? i finally know why :)


size_t total_memory_needed=0;
dataobj create_data(unsigned long size0, unsigned long size1, unsigned long size2, unsigned long size3, unsigned long elemsize)
{
	dataobj a;


	a.data = malloc(elemsize * size0 * size1 * size2 * size3); // new char[elemsize*size0*size1*size2*size3]; // 340*340*340
	total_memory_needed += elemsize * size0 * size1 * size2 * size3;

	size_t arrsize = size0 * size1 * size2 * size3;
	for (size_t i = 0; i < arrsize; i += elemsize)
	{
		float *t = (float *)&a.data[i];
		*t = 5;//TODO (0,1) random
		//*t = 0.0/0.0;  //nan
	}

	//this is long for f sake
	a.size = malloc(sizeof(unsigned long) * 4); // new int[4];

#ifdef MOREINFO 
	printf("Creting %ld\n",size0*size1*size2*size3);
#endif
	a.size[0] = size0;
	a.size[1] = size1;
	a.size[2] = size2;
	a.size[3] = size3;
	
	return a;
}
void delete_dataobj(dataobj a)
{
	free(a.data);
	free(a.size);
}

size_t five_count = 0;

int check_data(dataobj a)
{
	size_t arrsize = a.size[0] * a.size[1] * a.size[2] * a.size[3];
	for (size_t i = 0; i < arrsize; i += sizeof(float))
	{
		float *t = (float *)&a.data[i];
		if(*t != *t)
			return 0;
		if(*t == 5.0f)
			five_count++ ;
		//printf("%f\n",*t);
	}
	printf("%lu fivecount\n",five_count);
	return 1;
};








int main()
{
printf("Start\n");

#include "dimension_src/declare_800_2.h"
#include "dimension_src/declare_800_4.h"
#include "dimension_src/declare_800_8.h"
#include "dimension_src/declare_800_16.h"
//dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
//dataobj u_vec = create_data( 3,896,896,896,sizeof(float));
//dataobj vp_vec = create_data( 896,896,896,1,sizeof(float));
//int x_M = 879;
//int x_m = 0;
//int y_M = 879;
//int y_m = 0;
//int z_M = 879;
//int z_m = 0;
//float dt = 1.816000;
//int time_M = 688;
//int time_m = 1;
//int deviceid = -1;
//int devicerm = 1;

time_M = 11;
#ifdef blocksize_x
	printf("x = %d,y = %d,z = %d,t = %d\n",blocksize_x,blocksize_y,blocksize_z,-1);
#endif
#ifdef MOREINFO 
#endif


printf("total memory: %fGB\n",(float)total_memory_needed / 1e9f );
fflush(stdout);

#ifdef MOREINFO 
	printf("Create profiler\n");
#endif
struct profiler timers={0,0,0};




//printf("Forward\n");


Forward((struct dataobj *restrict) &damp_vec,(struct dataobj *restrict) &rec_vec,(struct dataobj *restrict) &rec_coords_vec,(struct dataobj *restrict) &src_vec,(struct dataobj *restrict) &src_coords_vec,(struct dataobj *restrict) &u_vec,(struct dataobj *restrict) &vp_vec,x_M,x_m,y_M,y_m,z_M,z_m,dt,o_x,o_y,o_z,p_rec_M,p_rec_m,p_src_M,p_src_m,time_M,time_m,deviceid,devicerm,&timers);
//Forward((struct dataobj *restrict) &damp_vec,(struct dataobj *restrict) &u_vec,(struct dataobj *restrict) &vp_vec,x_M,x_m,y_M,y_m,z_M,z_m,dt,devicerm);


//printf("Forward\n");

	printf("section 0 %f s\n",timers.section0);
	printf("section 1 %f s\n",timers.section1);
	printf("section 2 %f s\n",timers.section2);


#ifdef MOREINFO 
//checking u rec
//printf("u_vec[354] = %f\n",((float*)u_vec.data)[354]);
printf(check_data(u_vec) ? "u_vec is valid\n" : "u_vec is nan or not changed\n");
//printf(check_data(rec_vec) ? "rec_vec is valid\n" : "u_vec is nan or not changed\n");
printf("u_vec[1530428310] = %f\n",((float*)u_vec.data)[1530428310]);
#endif
#ifdef EXPORTUVEC

{
	FILE *write_ptr;
	write_ptr = fopen("test.bin","wb");  // w for write, b for binary
	size_t arrsize = sizeof(float)*u_vec.size[0]*u_vec.size[1]*u_vec.size[2]*u_vec.size[3];
	fwrite((char*)u_vec.data,arrsize,1,write_ptr); // write bytes from our buffer

}
#endif


	return 0;
}
