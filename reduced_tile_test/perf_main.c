
#include <stdio.h>
#include "stdlib.h"

typedef struct dataobj dataobj;//WHY?????? i finally know why :)


size_t total_memory_needed=0;
dataobj create_data(unsigned long size0, unsigned long size1, unsigned long size2, unsigned long size3, unsigned long elemsize)
{
	dataobj a;


	//a.data = malloc(elemsize * size0 * size1 * size2 * size3); // new char[elemsize*size0*size1*size2*size3]; // 340*340*340
	total_memory_needed += elemsize * size0 * size1 * size2 * size3;

	size_t arrsize = size0 * size1 * size2 * size3;
	/*
	for (size_t i = 0; i < arrsize; i += elemsize)
	{
		float *t = (float *)&a.data[i];
		*t = 1;//TODO (0,1) random
		// *t = 0.0/0.0;  //nan
	}
	*/

	//this is long for f sake
	a.size = malloc(sizeof(unsigned long) * 4); // new int[4];
	printf("Creting %ld\n",size0*size1*size2*size3);
	a.size[0] = size0;
	a.size[1] = size1;
	a.size[2] = size2;
	a.size[3] = size3;
	
	return a;
}
void delete_dataobj(dataobj a)
{
//Write_byte_to_file(filename_macro,(char*)a.data,a.size[0]*a.size[1]*a.size[2]*a.size[3]*sizeof(float));
	free(a.data);
	free(a.size);
}
int check_data(dataobj a)
{
	size_t arrsize = a.size[0] * a.size[1] * a.size[2] * a.size[3];
	for (size_t i = 0; i < arrsize; i += sizeof(float))
	{
		float *t = (float *)&a.data[i];
		if(*t != *t)
			return 0;
	}
	return 1;
};








int main()
{
printf("Start\n");

#ifdef SO2
// 2 800
dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
dataobj rec_vec = create_data( 1,1,1,1,sizeof(float));
dataobj rec_coords_vec = create_data( 1,3,1,1,sizeof(float));
dataobj src_vec = create_data( 1,1,1,1,sizeof(float));
dataobj src_coords_vec = create_data( 1,3,1,1,sizeof(float));
dataobj u_vec = create_data( 3,884,884,884,sizeof(float));
dataobj vp_vec = create_data( 884,884,884,1,sizeof(float));
int x_M = 879;
int x_m = 0;
int y_M = 879;
int y_m = 0;
int z_M = 879;
int z_m = 0;
float dt = 2.143000;
float o_x = -600.000000;
float o_y = -600.000000;
float o_z = -600.000000;
int p_rec_M = 639999;
int p_rec_m = 0;
int p_src_M = 0;
int p_src_m = 0;
int time_m = 1;
//int time_M = 583;
int deviceid = -1;
int devicerm = 1;

#elif defined SO8

// 8 800
dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
dataobj rec_vec = create_data( 1,1,1,1,sizeof(float));
dataobj rec_coords_vec = create_data( 1,3,1,1,sizeof(float));
dataobj src_vec = create_data( 1,1,1,1,sizeof(float));
dataobj src_coords_vec = create_data( 1,3,1,1,sizeof(float));
dataobj u_vec = create_data( 3,896,896,896,sizeof(float));
dataobj vp_vec = create_data( 896,896,896,1,sizeof(float));
int x_M = 879;
int x_m = 0;
int y_M = 879;
int y_m = 0;
int z_M = 879;
int z_m = 0;
float dt = 1.816000;
float o_x = -600.000000;
float o_y = -600.000000;
float o_z = -600.000000;
int p_rec_M = 639999;
int p_rec_m = 0;
int p_src_M = 0;
int p_src_m = 0;
//int time_M = 688;
int time_m = 1;
int deviceid = -1;
int devicerm = 1;

#elif defined SO16
// 16 800
dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
dataobj rec_vec = create_data( 1,1,1,1,sizeof(float));
dataobj rec_coords_vec = create_data( 1,3,1,1,sizeof(float));
dataobj src_vec = create_data( 1,1,1,1,sizeof(float));
dataobj src_coords_vec = create_data( 1,3,1,1,sizeof(float));
dataobj u_vec = create_data( 3,912,912,912,sizeof(float));
dataobj vp_vec = create_data( 912,912,912,1,sizeof(float));
int x_M = 879;
int x_m = 0;
int y_M = 879;
int y_m = 0;
int z_M = 879;
int z_m = 0;
float dt = 1.737000;
float o_x = -600.000000;
float o_y = -600.000000;
float o_z = -600.000000;
int p_rec_M = 639999;
int p_rec_m = 0;
int p_src_M = 0;
int p_src_m = 0;
//int time_M = 719;
int time_m = 1;
int deviceid = -1;
int devicerm = 1;

#endif

int time_M = 10;

printf("total memory: %fGB\n",(float)total_memory_needed / 1e9f );
fflush(stdout);

printf("Create profiler\n");
struct profiler timers={0,0,0};




printf("Forward\n");


Forward((struct dataobj *restrict) &damp_vec,(struct dataobj *restrict) &rec_vec,(struct dataobj *restrict) &rec_coords_vec,(struct dataobj *restrict) &src_vec,(struct dataobj *restrict) &src_coords_vec,(struct dataobj *restrict) &u_vec,(struct dataobj *restrict) &vp_vec,x_M,x_m,y_M,y_m,z_M,z_m,dt,o_x,o_y,o_z,p_rec_M,p_rec_m,p_src_M,p_src_m,time_M,time_m,deviceid,devicerm,&timers);


//printf("Forward\n");

	printf("section 0 %f s\n",timers.section0);
	printf("section 1 %f s\n",timers.section1);
	printf("section 2 %f s\n",timers.section2);

//checking u rec


//printf(check_data(u_vec) ? "u_vec is valid\n" : "u_vec is nan\n");
//printf(check_data(rec_vec) ? "rec_vec is valid\n" : "u_vec is nan\n");

	return 0;
}