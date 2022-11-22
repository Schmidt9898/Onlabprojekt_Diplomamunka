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

  #pragma omp target enter data map(to: rec[0:rec_vec->size[0]*rec_vec->size[1]])
  #pragma omp target enter data map(to: u[0:u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]])
  #pragma omp target enter data map(to: damp[0:damp_vec->size[0]*damp_vec->size[1]*damp_vec->size[2]])
  #pragma omp target enter data map(to: rec_coords[0:rec_coords_vec->size[0]*rec_coords_vec->size[1]])
  #pragma omp target enter data map(to: src[0:src_vec->size[0]*src_vec->size[1]])
  #pragma omp target enter data map(to: src_coords[0:src_coords_vec->size[0]*src_coords_vec->size[1]])
  #pragma omp target enter data map(to: vp[0:vp_vec->size[0]*vp_vec->size[1]*vp_vec->size[2]])

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



  for (int time = time_m, t0 = (time)%(3), t1 = (time + 2)%(3), t2 = (time + 1)%(3); time <= time_M; time += 1, t0 = (time)%(3), t1 = (time + 2)%(3), t2 = (time + 1)%(3))
  {
//printf("time\n");
    /* Begin section0 */
    START_TIMER(section0)
    #pragma omp target teams distribute parallel for collapse(3)
	//#pragma omp tile sizes(2,1,4)
    for (int x = x_m; x <= x_M; x += 1)
    {
      for (int y = y_m; y <= y_M; y += 1)
      {
        for (int z = z_m; z <= z_M; z += 1)
        {
          float r10 = 1.0F/(vpL0(x + 16, y + 16, z + 16)*vpL0(x + 16, y + 16, z + 16));
          uL0(t2, x + 16, y + 16, z + 16) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 16, y + 16, z + 16) + r10*(-r8*(-2.0F*uL0(t0, x + 16, y + 16, z + 16)) - r8*uL0(t1, x + 16, y + 16, z + 16)) + 1.07916777e-8F*(-uL0(t0, x + 8, y + 16, z + 16) - uL0(t0, x + 16, y + 8, z + 16) - uL0(t0, x + 16, y + 16, z + 8) - uL0(t0, x + 16, y + 16, z + 24) - uL0(t0, x + 16, y + 24, z + 16) - uL0(t0, x + 24, y + 16, z + 16)) + 2.25524041e-7F*(uL0(t0, x + 9, y + 16, z + 16) + uL0(t0, x + 16, y + 9, z + 16) + uL0(t0, x + 16, y + 16, z + 9) + uL0(t0, x + 16, y + 16, z + 23) + uL0(t0, x + 16, y + 23, z + 16) + uL0(t0, x + 23, y + 16, z + 16)) + 2.30222458e-6F*(-uL0(t0, x + 10, y + 16, z + 16) - uL0(t0, x + 16, y + 10, z + 16) - uL0(t0, x + 16, y + 16, z + 10) - uL0(t0, x + 16, y + 16, z + 22) - uL0(t0, x + 16, y + 22, z + 16) - uL0(t0, x + 22, y + 16, z + 16)) + 1.54709492e-5F*(uL0(t0, x + 11, y + 16, z + 16) + uL0(t0, x + 16, y + 11, z + 16) + uL0(t0, x + 16, y + 16, z + 11) + uL0(t0, x + 16, y + 16, z + 21) + uL0(t0, x + 16, y + 21, z + 16) + uL0(t0, x + 21, y + 16, z + 16)) + 7.85634138e-5F*(-uL0(t0, x + 12, y + 16, z + 16) - uL0(t0, x + 16, y + 12, z + 16) - uL0(t0, x + 16, y + 16, z + 12) - uL0(t0, x + 16, y + 16, z + 20) - uL0(t0, x + 16, y + 20, z + 16) - uL0(t0, x + 20, y + 16, z + 16)) + 3.35203899e-4F*(uL0(t0, x + 13, y + 16, z + 16) + uL0(t0, x + 16, y + 13, z + 16) + uL0(t0, x + 16, y + 16, z + 13) + uL0(t0, x + 16, y + 16, z + 19) + uL0(t0, x + 16, y + 19, z + 16) + uL0(t0, x + 19, y + 16, z + 16)) + 1.38271608e-3F*(-uL0(t0, x + 14, y + 16, z + 16) - uL0(t0, x + 16, y + 14, z + 16) - uL0(t0, x + 16, y + 16, z + 14) - uL0(t0, x + 16, y + 16, z + 18) - uL0(t0, x + 16, y + 18, z + 16) - uL0(t0, x + 18, y + 16, z + 16)) + 7.90123476e-3F*(uL0(t0, x + 15, y + 16, z + 16) + uL0(t0, x + 16, y + 15, z + 16) + uL0(t0, x + 16, y + 16, z + 15) + uL0(t0, x + 16, y + 16, z + 17) + uL0(t0, x + 16, y + 17, z + 16) + uL0(t0, x + 17, y + 16, z + 16)) - 4.07312557e-2F*uL0(t0, x + 16, y + 16, z + 16))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
        }
      }
    }
    STOP_TIMER(section0,timers)
    /* End section0 */

    /* Begin section1 */
	
    /* End section1 */

    /* Begin section2 */

    /* End section2 */
  }

  #pragma omp target update from(rec[0:rec_vec->size[0]*rec_vec->size[1]])
  #pragma omp target exit data map(release: rec[0:rec_vec->size[0]*rec_vec->size[1]]) if(devicerm)
  #pragma omp target update from(u[0:u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]])
  #pragma omp target exit data map(release: u[0:u_vec->size[0]*u_vec->size[1]*u_vec->size[2]*u_vec->size[3]]) if(devicerm)
  #pragma omp target exit data map(delete: damp[0:damp_vec->size[0]*damp_vec->size[1]*damp_vec->size[2]]) if(devicerm && damp_vec->size[0] != 0 && damp_vec->size[1] != 0 && damp_vec->size[2] != 0)
  #pragma omp target exit data map(delete: rec_coords[0:rec_coords_vec->size[0]*rec_coords_vec->size[1]]) if(devicerm && rec_coords_vec->size[0] != 0 && rec_coords_vec->size[1] != 0)
  #pragma omp target exit data map(delete: src[0:src_vec->size[0]*src_vec->size[1]]) if(devicerm && src_vec->size[0] != 0 && src_vec->size[1] != 0)
  #pragma omp target exit data map(delete: src_coords[0:src_coords_vec->size[0]*src_coords_vec->size[1]]) if(devicerm && src_coords_vec->size[0] != 0 && src_coords_vec->size[1] != 0)
  #pragma omp target exit data map(delete: vp[0:vp_vec->size[0]*vp_vec->size[1]*vp_vec->size[2]]) if(devicerm && vp_vec->size[0] != 0 && vp_vec->size[1] != 0 && vp_vec->size[2] != 0)

  return 0;
}
