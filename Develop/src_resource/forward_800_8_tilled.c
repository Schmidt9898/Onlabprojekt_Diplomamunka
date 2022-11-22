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
} ;


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
    /* Begin section0 */
    START_TIMER(section0)
#pragma omp target teams distribute collapse(3) thread_limit(L_THREAD_LIMIT)
	for(int Bx = x_m; Bx <= x_M; Bx += BLOCKSIZE_X)
		for (int By = y_m; By <= y_M; By += BLOCKSIZE_Y)
			for (int Bz = z_m; Bz <= z_M; Bz += BLOCKSIZE_Z)
			{
			#pragma omp parallel for collapse(3)
			for (int x = Bx; x < Bx + BLOCKSIZE_X; x++)
				for (int y = By; y < By + BLOCKSIZE_Y; y++)
					for (int z = Bz; z < Bz + BLOCKSIZE_Z; z++)
					{
						if (x <= x_M && y <= y_M && z <= z_M){
          float r10 = 1.0F/(vpL0(x + 8, y + 8, z + 8)*vpL0(x + 8, y + 8, z + 8));
          uL0(t2, x + 8, y + 8, z + 8) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 8, y + 8, z + 8) + r10*(-r8*(-2.0F*uL0(t0, x + 8, y + 8, z + 8)) - r8*uL0(t1, x + 8, y + 8, z + 8)) + 7.93650813e-6F*(-uL0(t0, x + 4, y + 8, z + 8) - uL0(t0, x + 8, y + 4, z + 8) - uL0(t0, x + 8, y + 8, z + 4) - uL0(t0, x + 8, y + 8, z + 12) - uL0(t0, x + 8, y + 12, z + 8) - uL0(t0, x + 12, y + 8, z + 8)) + 1.12874782e-4F*(uL0(t0, x + 5, y + 8, z + 8) + uL0(t0, x + 8, y + 5, z + 8) + uL0(t0, x + 8, y + 8, z + 5) + uL0(t0, x + 8, y + 8, z + 11) + uL0(t0, x + 8, y + 11, z + 8) + uL0(t0, x + 11, y + 8, z + 8)) + 8.8888891e-4F*(-uL0(t0, x + 6, y + 8, z + 8) - uL0(t0, x + 8, y + 6, z + 8) - uL0(t0, x + 8, y + 8, z + 6) - uL0(t0, x + 8, y + 8, z + 10) - uL0(t0, x + 8, y + 10, z + 8) - uL0(t0, x + 10, y + 8, z + 8)) + 7.11111128e-3F*(uL0(t0, x + 7, y + 8, z + 8) + uL0(t0, x + 8, y + 7, z + 8) + uL0(t0, x + 8, y + 8, z + 7) + uL0(t0, x + 8, y + 8, z + 9) + uL0(t0, x + 8, y + 9, z + 8) + uL0(t0, x + 9, y + 8, z + 8)) - 3.79629639e-2F*uL0(t0, x + 8, y + 8, z + 8))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
        }
      }
    }
    STOP_TIMER(section0,timers)
    /* End section0 */

    /* Begin section1 */
    START_TIMER(section1)
    if (src_vec->size[0]*src_vec->size[1] > 0 && p_src_M - p_src_m + 1 > 0)
    {
      #pragma omp target teams distribute parallel for collapse(1)
      for (int p_src = p_src_m; p_src <= p_src_M; p_src += 1)
      {
        float posx = -o_x + src_coordsL0(p_src, 0);
        float posy = -o_y + src_coordsL0(p_src, 1);
        float posz = -o_z + src_coordsL0(p_src, 2);
        int ii_src_0 = (int)(floor(6.66667e-2F*posx));
        int ii_src_1 = (int)(floor(6.66667e-2F*posy));
        int ii_src_2 = (int)(floor(6.66667e-2F*posz));
        int ii_src_3 = 1 + (int)(floor(6.66667e-2F*posz));
        int ii_src_4 = 1 + (int)(floor(6.66667e-2F*posy));
        int ii_src_5 = 1 + (int)(floor(6.66667e-2F*posx));
        float px = (float)(posx - 1.5e+1F*(int)(floor(6.66667e-2F*posx)));
        float py = (float)(posy - 1.5e+1F*(int)(floor(6.66667e-2F*posy)));
        float pz = (float)(posz - 1.5e+1F*(int)(floor(6.66667e-2F*posz)));
        if (ii_src_0 >= x_m - 1 && ii_src_1 >= y_m - 1 && ii_src_2 >= z_m - 1 && ii_src_0 <= x_M + 1 && ii_src_1 <= y_M + 1 && ii_src_2 <= z_M + 1)
        {
          float r0 = (dt*dt)*(vpL0(ii_src_0 + 8, ii_src_1 + 8, ii_src_2 + 8)*vpL0(ii_src_0 + 8, ii_src_1 + 8, ii_src_2 + 8))*(-2.96296e-4F*px*py*pz + 4.44445e-3F*px*py + 4.44445e-3F*px*pz - 6.66667e-2F*px + 4.44445e-3F*py*pz - 6.66667e-2F*py - 6.66667e-2F*pz + 1)*srcL0(time, p_src);
          #pragma omp atomic update
          uL0(t2, ii_src_0 + 8, ii_src_1 + 8, ii_src_2 + 8) += r0;
        }
        if (ii_src_0 >= x_m - 1 && ii_src_1 >= y_m - 1 && ii_src_3 >= z_m - 1 && ii_src_0 <= x_M + 1 && ii_src_1 <= y_M + 1 && ii_src_3 <= z_M + 1)
        {
          float r1 = (dt*dt)*(vpL0(ii_src_0 + 8, ii_src_1 + 8, ii_src_3 + 8)*vpL0(ii_src_0 + 8, ii_src_1 + 8, ii_src_3 + 8))*(2.96296e-4F*px*py*pz - 4.44445e-3F*px*pz - 4.44445e-3F*py*pz + 6.66667e-2F*pz)*srcL0(time, p_src);
          #pragma omp atomic update
          uL0(t2, ii_src_0 + 8, ii_src_1 + 8, ii_src_3 + 8) += r1;
        }
        if (ii_src_0 >= x_m - 1 && ii_src_2 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_0 <= x_M + 1 && ii_src_2 <= z_M + 1 && ii_src_4 <= y_M + 1)
        {
          float r2 = (dt*dt)*(vpL0(ii_src_0 + 8, ii_src_4 + 8, ii_src_2 + 8)*vpL0(ii_src_0 + 8, ii_src_4 + 8, ii_src_2 + 8))*(2.96296e-4F*px*py*pz - 4.44445e-3F*px*py - 4.44445e-3F*py*pz + 6.66667e-2F*py)*srcL0(time, p_src);
          #pragma omp atomic update
          uL0(t2, ii_src_0 + 8, ii_src_4 + 8, ii_src_2 + 8) += r2;
        }
        if (ii_src_0 >= x_m - 1 && ii_src_3 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_0 <= x_M + 1 && ii_src_3 <= z_M + 1 && ii_src_4 <= y_M + 1)
        {
          float r3 = (dt*dt)*(vpL0(ii_src_0 + 8, ii_src_4 + 8, ii_src_3 + 8)*vpL0(ii_src_0 + 8, ii_src_4 + 8, ii_src_3 + 8))*(-2.96296e-4F*px*py*pz + 4.44445e-3F*py*pz)*srcL0(time, p_src);
          #pragma omp atomic update
          uL0(t2, ii_src_0 + 8, ii_src_4 + 8, ii_src_3 + 8) += r3;
        }
        if (ii_src_1 >= y_m - 1 && ii_src_2 >= z_m - 1 && ii_src_5 >= x_m - 1 && ii_src_1 <= y_M + 1 && ii_src_2 <= z_M + 1 && ii_src_5 <= x_M + 1)
        {
          float r4 = (dt*dt)*(vpL0(ii_src_5 + 8, ii_src_1 + 8, ii_src_2 + 8)*vpL0(ii_src_5 + 8, ii_src_1 + 8, ii_src_2 + 8))*(2.96296e-4F*px*py*pz - 4.44445e-3F*px*py - 4.44445e-3F*px*pz + 6.66667e-2F*px)*srcL0(time, p_src);
          #pragma omp atomic update
          uL0(t2, ii_src_5 + 8, ii_src_1 + 8, ii_src_2 + 8) += r4;
        }
        if (ii_src_1 >= y_m - 1 && ii_src_3 >= z_m - 1 && ii_src_5 >= x_m - 1 && ii_src_1 <= y_M + 1 && ii_src_3 <= z_M + 1 && ii_src_5 <= x_M + 1)
        {
          float r5 = (dt*dt)*(vpL0(ii_src_5 + 8, ii_src_1 + 8, ii_src_3 + 8)*vpL0(ii_src_5 + 8, ii_src_1 + 8, ii_src_3 + 8))*(-2.96296e-4F*px*py*pz + 4.44445e-3F*px*pz)*srcL0(time, p_src);
          #pragma omp atomic update
          uL0(t2, ii_src_5 + 8, ii_src_1 + 8, ii_src_3 + 8) += r5;
        }
        if (ii_src_2 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_5 >= x_m - 1 && ii_src_2 <= z_M + 1 && ii_src_4 <= y_M + 1 && ii_src_5 <= x_M + 1)
        {
          float r6 = (dt*dt)*(vpL0(ii_src_5 + 8, ii_src_4 + 8, ii_src_2 + 8)*vpL0(ii_src_5 + 8, ii_src_4 + 8, ii_src_2 + 8))*(-2.96296e-4F*px*py*pz + 4.44445e-3F*px*py)*srcL0(time, p_src);
          #pragma omp atomic update
          uL0(t2, ii_src_5 + 8, ii_src_4 + 8, ii_src_2 + 8) += r6;
        }
        if (ii_src_3 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_5 >= x_m - 1 && ii_src_3 <= z_M + 1 && ii_src_4 <= y_M + 1 && ii_src_5 <= x_M + 1)
        {
          float r7 = 2.96296e-4F*px*py*pz*(dt*dt)*(vpL0(ii_src_5 + 8, ii_src_4 + 8, ii_src_3 + 8)*vpL0(ii_src_5 + 8, ii_src_4 + 8, ii_src_3 + 8))*srcL0(time, p_src);
          #pragma omp atomic update
          uL0(t2, ii_src_5 + 8, ii_src_4 + 8, ii_src_3 + 8) += r7;
        }
      }
    }
    STOP_TIMER(section1,timers)
    /* End section1 */

    /* Begin section2 */
    START_TIMER(section2)
    if (rec_vec->size[0]*rec_vec->size[1] > 0 && p_rec_M - p_rec_m + 1 > 0)
    {
      #pragma omp target teams distribute parallel for collapse(1)
      for (int p_rec = p_rec_m; p_rec <= p_rec_M; p_rec += 1)
      {
        float posx = -o_x + rec_coordsL0(p_rec, 0);
        float posy = -o_y + rec_coordsL0(p_rec, 1);
        float posz = -o_z + rec_coordsL0(p_rec, 2);
        int ii_rec_0 = (int)(floor(6.66667e-2F*posx));
        int ii_rec_1 = (int)(floor(6.66667e-2F*posy));
        int ii_rec_2 = (int)(floor(6.66667e-2F*posz));
        int ii_rec_3 = 1 + (int)(floor(6.66667e-2F*posz));
        int ii_rec_4 = 1 + (int)(floor(6.66667e-2F*posy));
        int ii_rec_5 = 1 + (int)(floor(6.66667e-2F*posx));
        float px = (float)(posx - 1.5e+1F*(int)(floor(6.66667e-2F*posx)));
        float py = (float)(posy - 1.5e+1F*(int)(floor(6.66667e-2F*posy)));
        float pz = (float)(posz - 1.5e+1F*(int)(floor(6.66667e-2F*posz)));
        float sum = 0.0F;
        if (ii_rec_0 >= x_m - 1 && ii_rec_1 >= y_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_1 <= y_M + 1 && ii_rec_2 <= z_M + 1)
        {
          sum += (-2.96296e-4F*px*py*pz + 4.44445e-3F*px*py + 4.44445e-3F*px*pz - 6.66667e-2F*px + 4.44445e-3F*py*pz - 6.66667e-2F*py - 6.66667e-2F*pz + 1)*uL0(t0, ii_rec_0 + 8, ii_rec_1 + 8, ii_rec_2 + 8);
        }
        if (ii_rec_0 >= x_m - 1 && ii_rec_1 >= y_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_1 <= y_M + 1 && ii_rec_3 <= z_M + 1)
        {
          sum += (2.96296e-4F*px*py*pz - 4.44445e-3F*px*pz - 4.44445e-3F*py*pz + 6.66667e-2F*pz)*uL0(t0, ii_rec_0 + 8, ii_rec_1 + 8, ii_rec_3 + 8);
        }
        if (ii_rec_0 >= x_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_2 <= z_M + 1 && ii_rec_4 <= y_M + 1)
        {
          sum += (2.96296e-4F*px*py*pz - 4.44445e-3F*px*py - 4.44445e-3F*py*pz + 6.66667e-2F*py)*uL0(t0, ii_rec_0 + 8, ii_rec_4 + 8, ii_rec_2 + 8);
        }
        if (ii_rec_0 >= x_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_3 <= z_M + 1 && ii_rec_4 <= y_M + 1)
        {
          sum += (-2.96296e-4F*px*py*pz + 4.44445e-3F*py*pz)*uL0(t0, ii_rec_0 + 8, ii_rec_4 + 8, ii_rec_3 + 8);
        }
        if (ii_rec_1 >= y_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_1 <= y_M + 1 && ii_rec_2 <= z_M + 1 && ii_rec_5 <= x_M + 1)
        {
          sum += (2.96296e-4F*px*py*pz - 4.44445e-3F*px*py - 4.44445e-3F*px*pz + 6.66667e-2F*px)*uL0(t0, ii_rec_5 + 8, ii_rec_1 + 8, ii_rec_2 + 8);
        }
        if (ii_rec_1 >= y_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_1 <= y_M + 1 && ii_rec_3 <= z_M + 1 && ii_rec_5 <= x_M + 1)
        {
          sum += (-2.96296e-4F*px*py*pz + 4.44445e-3F*px*pz)*uL0(t0, ii_rec_5 + 8, ii_rec_1 + 8, ii_rec_3 + 8);
        }
        if (ii_rec_2 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_2 <= z_M + 1 && ii_rec_4 <= y_M + 1 && ii_rec_5 <= x_M + 1)
        {
          sum += (-2.96296e-4F*px*py*pz + 4.44445e-3F*px*py)*uL0(t0, ii_rec_5 + 8, ii_rec_4 + 8, ii_rec_2 + 8);
        }
        if (ii_rec_3 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_3 <= z_M + 1 && ii_rec_4 <= y_M + 1 && ii_rec_5 <= x_M + 1)
        {
          sum += 2.96296e-4F*px*py*pz*uL0(t0, ii_rec_5 + 8, ii_rec_4 + 8, ii_rec_3 + 8);
        }
        recL0(time, p_rec) = sum;
      }
    }
    STOP_TIMER(section2,timers)
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
