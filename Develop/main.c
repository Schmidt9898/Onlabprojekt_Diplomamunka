
#include "forward_only.h"

#include <stdio.h>
#include "stdlib.h"

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
		*t = 1;
	}


	a.size = malloc(sizeof(unsigned int) * 4); // new int[4];
printf("sss\n");
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








int main()
{
printf("Start\n");
void* asd = malloc(sizeof(unsigned int) * 4); // new int[4];

printf("Start\n");


dataobj damp_vec       = create_data(802,802,802,1,sizeof(float));
dataobj rec_vec        = create_data(721,518400,1,1,sizeof(float));
dataobj rec_coords_vec = create_data(518400,3,1,1,sizeof(float));
dataobj src_vec        = create_data(721,1,1,1,sizeof(float));
dataobj src_coords_vec = create_data(1,3,1,1,sizeof(float));
dataobj u_vec          = create_data(3,832,832,832,sizeof(float));
dataobj vp_vec         = create_data(832,832,832,1,sizeof(float));

/*

dataobj damp_vec       = create_data(10,10,10,1,sizeof(float));
dataobj rec_vec        = create_data(10,10,1,1,sizeof(float));
dataobj rec_coords_vec = create_data(10,3,1,1,sizeof(float));
dataobj src_vec        = create_data(10,1,1,1,sizeof(float));
dataobj src_coords_vec = create_data(1,3,1,1,sizeof(float));
dataobj u_vec          = create_data(3,10,10,10,sizeof(float));
dataobj vp_vec         = create_data(10,10,10,1,sizeof(float));
*/
int x_M            	= 799;
int x_m            	= 0;
int y_M            	= 799;
int y_m            	= 0;
int z_m            	= 0;
int z_M            	= 799;
float dt        	= 1.737000;
float o_x       	= -600.000000;
float o_y       	= -600.000000;
float o_z       	= -600.000000;
int p_rec_M        	= 518399;
int p_rec_m        	= 0;
int p_src_M        	= 0;
int p_src_m        	= 0;
int time_M         	= 10;
int time_m         	= 1;
int deviceid       	= -1;
int devicerm       	= 1;


printf("line 80\n");

//struct profiler timers={0,0,0};

printf("line 80\n");

/*
	Forward((struct dataobj *restrict) &damp_vec,
			(struct dataobj *restrict) &rec_vec,
			(struct dataobj *restrict) &rec_coords_vec,
			(struct dataobj *restrict) &src_vec,
			(struct dataobj *restrict) &src_coords_vec,
			(struct dataobj *restrict) &u_vec,
			(struct dataobj *restrict) &vp_vec,
			x_M,
			x_m,
			y_M,
			y_m,
			z_M,
			z_m,
			dt,
			o_x,
			o_y,
			o_z,
			p_rec_M,
			p_rec_m,
			p_src_M,
			p_src_m,
			time_M,
			time_m,
			deviceid,
			devicerm,
			NULL);
*/

	//printf("section 0 %f s\n",timers.section0);


	return 0;
}