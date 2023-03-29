#define _POSIX_C_SOURCE 200809L
#define uL0(t, x, y, z) u[(t)*x_stride0 + (x)*y_stride0 + (y)*z_stride0 + (z)]
#define dampL0(x, y, z) damp[(x)*y_stride1 + (y)*z_stride1 + (z)]
#define vpL0(x, y, z) vp[(x)*y_stride0 + (y)*z_stride0 + (z)]
#define recL0(time, p_rec) rec[(p_rec) + (time)*p_rec_stride0]
#define rec_coordsL0(p_rec, d) rec_coords[(d) + (p_rec)*d_stride0]
#define srcL0(time, p_src) src[(p_src) + (time)*p_src_stride0]
#define src_coordsL0(p_src, d) src_coords[(d) + (p_src)*d_stride0]
#define START_TIMER(S) struct timeval start_ ## S , end_ ## S ; gettimeofday(&start_ ## S , NULL);
#define STOP_TIMER(S,T) gettimeofday(&end_ ## S, NULL); T->S += (double)(end_ ## S .tv_sec-start_ ## S.tv_sec)+(double)(end_ ## S .tv_usec-start_ ## S .tv_usec)/1000000;

#include "stdlib.h"
#include <stdio.h>
#include "math.h"
#include "sys/time.h"
#include "omp.h"

#include "cuda_section0.h"

struct dataobj
{
  void *restrict data;
  unsigned long * size;
  unsigned long * npsize;
  unsigned long * dsize;
  int * hsize;
  int * hofs;
  int * oofs;
  void * dmap;
} ;

struct profiler
{
  double section0;
  double section1;
  double section2;
};


int Forward(struct dataobj *restrict damp_vec, struct dataobj *restrict rec_vec, struct dataobj *restrict rec_coords_vec, struct dataobj *restrict src_vec, struct dataobj *restrict src_coords_vec, struct dataobj *restrict u_vec, struct dataobj *restrict vp_vec, const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m, const float dt, const float o_x, const float o_y, const float o_z, const int p_rec_M, const int p_rec_m, const int p_src_M, const int p_src_m, const int time_M, const int time_m, const int deviceid, const int devicerm, struct profiler * timers)
{
  /* Begin of OpenMP setup */
  if (deviceid != -1)
  {
    omp_set_default_device(deviceid);
  }
  /* End of OpenMP setup */

  
	float *damp = (float *) damp_vec->data;
	float *rec = (float *) rec_vec->data;
	float *rec_coords = (float *) rec_coords_vec->data;
	float *src = (float *) src_vec->data;
	float *src_coords = (float *) src_coords_vec->data;
	float *u = (float *) u_vec->data;
	float *vp = (float *) vp_vec->data;

#ifdef MOREINFO 
	printf("Before GPU\n");
	printf("u pointer cpu: %p\n",u);
	printf("damp pointer cpu: %p\n",damp);
	printf("vp pointer cpu: %p\n",vp);
#endif


  cuda_enter_data(&damp,damp_vec->data, damp_vec->size[0]*damp_vec->size[1]*damp_vec->size[2]);
  cuda_enter_data(&rec,rec_vec->data,rec_vec->size[0]*rec_vec->size[1]);
  cuda_enter_data(&u,u_vec->data,u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]);
  cuda_enter_data(&damp,damp_vec->data,damp_vec->size[0]*damp_vec->size[1]*damp_vec->size[2]);
  cuda_enter_data(&rec_coords,rec_coords_vec->data,rec_coords_vec->size[0]*rec_coords_vec->size[1]);
  cuda_enter_data(&src,src_vec->data,src_vec->size[0]*src_vec->size[1]);
  cuda_enter_data(&src_coords,src_coords_vec->data,src_coords_vec->size[0]*src_coords_vec->size[1]);
  cuda_enter_data(&vp,vp_vec->data,vp_vec->size[0]*vp_vec->size[1]*vp_vec->size[2]);


  const long x_fsz0 = u_vec->size[1];
  const long y_fsz0 = u_vec->size[2];
  const long z_fsz0 = u_vec->size[3];
  const long y_fsz1 = damp_vec->size[1];
  const long z_fsz1 = damp_vec->size[2];
  const long p_rec_fsz0 = rec_vec->size[1];
  const long d_fsz0 = rec_coords_vec->size[1];
  const long p_src_fsz0 = src_vec->size[1];

  const long x_stride0 = x_fsz0*y_fsz0*z_fsz0;
  const long y_stride0 = y_fsz0*z_fsz0;
  const long z_stride0 = z_fsz0;
  const long y_stride1 = y_fsz1*z_fsz1;
  const long z_stride1 = z_fsz1;
  const long p_rec_stride0 = p_rec_fsz0;
  const long d_stride0 = d_fsz0;
  const long p_src_stride0 = p_src_fsz0;

  float r8 = 1.0F/(dt*dt);
  float r9 = 1.0F/dt;


	kernel_vars(dt,x_stride0,y_stride0,z_stride0,y_stride1,z_stride1,p_rec_stride0,d_stride0,p_src_stride0);

	kernel_section0(x_M,x_m,y_M,y_m,z_M,z_m,1,0,2,vp,u,damp);
	cudaDeviceSynchronize();

    START_TIMER(section0)
  //for (int time = time_m, t0 = (time)%(3), t1 = (time + 2)%(3), t2 = (time + 1)%(3); time <= time_M; time += 1, t0 = (time)%(3), t1 = (time + 2)%(3), t2 = (time + 1)%(3))
  for (int time = time_m + 1, t0 = (time)%(3), t1 = (time + 2)%(3), t2 = (time + 1)%(3); time <= time_M; time += 1, t0 = (time)%(3), t1 = (time + 2)%(3), t2 = (time + 1)%(3))
  {
//printf("time\n");
    /* Begin section0 */

	kernel_section0(x_M,x_m,y_M,y_m,z_M,z_m,t0,t1,t2,vp,u,damp);
    
    /* End section0 */

    /* Begin section1 */
	
    /* End section1 */

    /* Begin section2 */

    /* End section2 */
  }
	cudaDeviceSynchronize();
	STOP_TIMER(section0,timers)


  cuda_update_data_from(rec,rec_vec->data,rec_vec->size[0]*rec_vec->size[1]);
  cuda_update_data_from(u,u_vec->data,u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]);
  cuda_exit_data(damp);
  cuda_exit_data(rec_coords);
  cuda_exit_data(src);
  cuda_exit_data(src_coords);
  cuda_exit_data(vp);
	cudaDeviceSynchronize();


  return 0;
}
