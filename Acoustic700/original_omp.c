#define _POSIX_C_SOURCE 200809L
#define START_TIMER(S) struct timeval start_ ## S , end_ ## S ; gettimeofday(&start_ ## S , NULL);
#define STOP_TIMER(S,T) gettimeofday(&end_ ## S, NULL); T->S += (double)(end_ ## S .tv_sec-start_ ## S.tv_sec)+(double)(end_ ## S .tv_usec-start_ ## S .tv_usec)/1000000;

#include "stdlib.h"
#include "math.h"
#include "sys/time.h"
#include "omp.h"

struct dataobj
{
  void *restrict data;
  int * size;
  int * npsize;
  int * dsize;
  int * hsize;
  int * hofs;
  int * oofs;
} ;

struct profiler
{
  double section0;
  double section1;
  double section2;
} ;


int Forward(struct dataobj *restrict damp_vec, const float dt, const float o_x, const float o_y, const float o_z, struct dataobj *restrict rec_vec, struct dataobj *restrict rec_coords_vec, struct dataobj *restrict src_vec, struct dataobj *restrict src_coords_vec, struct dataobj *restrict u_vec, struct dataobj *restrict vp_vec, const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m, const int p_rec_M, const int p_rec_m, const int p_src_M, const int p_src_m, const int time_M, const int time_m, const int devicerm, const int deviceid, struct profiler * timers)
{
  /* Begin of OpenMP setup */
  if (deviceid != -1)
  {
    omp_set_default_device(deviceid);
  }
  /* End of OpenMP setup */

  float (*restrict damp)[damp_vec->size[1]][damp_vec->size[2]] = (float (*)[damp_vec->size[1]][damp_vec->size[2]]) damp_vec->data;
  float (*restrict rec)[rec_vec->size[1]] = (float (*)[rec_vec->size[1]]) rec_vec->data;
  float (*restrict rec_coords)[rec_coords_vec->size[1]] = (float (*)[rec_coords_vec->size[1]]) rec_coords_vec->data;
  float (*restrict src)[src_vec->size[1]] = (float (*)[src_vec->size[1]]) src_vec->data;
  float (*restrict src_coords)[src_coords_vec->size[1]] = (float (*)[src_coords_vec->size[1]]) src_coords_vec->data;
  float (*restrict u)[u_vec->size[1]][u_vec->size[2]][u_vec->size[3]] = (float (*)[u_vec->size[1]][u_vec->size[2]][u_vec->size[3]]) u_vec->data;
  float (*restrict vp)[vp_vec->size[1]][vp_vec->size[2]] = (float (*)[vp_vec->size[1]][vp_vec->size[2]]) vp_vec->data;

  #pragma omp target enter data map(to: rec[0:rec_vec->size[0]][0:rec_vec->size[1]])
  #pragma omp target enter data map(to: u[0:u_vec->size[0]][0:u_vec->size[1]][0:u_vec->size[2]][0:u_vec->size[3]])
  #pragma omp target enter data map(to: damp[0:damp_vec->size[0]][0:damp_vec->size[1]][0:damp_vec->size[2]])
  #pragma omp target enter data map(to: rec_coords[0:rec_coords_vec->size[0]][0:rec_coords_vec->size[1]])
  #pragma omp target enter data map(to: src[0:src_vec->size[0]][0:src_vec->size[1]])
  #pragma omp target enter data map(to: src_coords[0:src_coords_vec->size[0]][0:src_coords_vec->size[1]])
  #pragma omp target enter data map(to: vp[0:vp_vec->size[0]][0:vp_vec->size[1]][0:vp_vec->size[2]])

  float r0 = 1.0F/(dt*dt);
  float r1 = 1.0F/dt;
  for (int time = time_m, t0 = (time)%(3), t1 = (time + 2)%(3), t2 = (time + 1)%(3); time <= time_M; time += 1, t0 = (time)%(3), t1 = (time + 2)%(3), t2 = (time + 1)%(3))
  {
    /* Begin section0 */
    START_TIMER(section0)
    #pragma omp target teams distribute parallel for collapse(3)
    for (int x = x_m; x <= x_M; x += 1)
    {
      for (int y = y_m; y <= y_M; y += 1)
      {
        for (int z = z_m; z <= z_M; z += 1)
        {
          float r10 = 1.0F/(vp[x + 8][y + 8][z + 8]*vp[x + 8][y + 8][z + 8]);
          u[t2][x + 8][y + 8][z + 8] = (r1*damp[x + 1][y + 1][z + 1]*u[t0][x + 8][y + 8][z + 8] + r10*(-r0*(-2.0F*u[t0][x + 8][y + 8][z + 8]) - r0*u[t1][x + 8][y + 8][z + 8]) + 7.93650813e-6F*(-u[t0][x + 4][y + 8][z + 8] - u[t0][x + 8][y + 4][z + 8] - u[t0][x + 8][y + 8][z + 4] - u[t0][x + 8][y + 8][z + 12] - u[t0][x + 8][y + 12][z + 8] - u[t0][x + 12][y + 8][z + 8]) + 1.12874782e-4F*(u[t0][x + 5][y + 8][z + 8] + u[t0][x + 8][y + 5][z + 8] + u[t0][x + 8][y + 8][z + 5] + u[t0][x + 8][y + 8][z + 11] + u[t0][x + 8][y + 11][z + 8] + u[t0][x + 11][y + 8][z + 8]) + 8.8888891e-4F*(-u[t0][x + 6][y + 8][z + 8] - u[t0][x + 8][y + 6][z + 8] - u[t0][x + 8][y + 8][z + 6] - u[t0][x + 8][y + 8][z + 10] - u[t0][x + 8][y + 10][z + 8] - u[t0][x + 10][y + 8][z + 8]) + 7.11111128e-3F*(u[t0][x + 7][y + 8][z + 8] + u[t0][x + 8][y + 7][z + 8] + u[t0][x + 8][y + 8][z + 7] + u[t0][x + 8][y + 8][z + 9] + u[t0][x + 8][y + 9][z + 8] + u[t0][x + 9][y + 8][z + 8]) - 3.79629639e-2F*u[t0][x + 8][y + 8][z + 8])/(r0*r10 + r1*damp[x + 1][y + 1][z + 1]);
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
        float posx = -o_x + src_coords[p_src][0];
        float posy = -o_y + src_coords[p_src][1];
        float posz = -o_z + src_coords[p_src][2];
        int ii_src_0 = (int)(floor(6.66667e-2*posx));
        int ii_src_1 = (int)(floor(6.66667e-2*posy));
        int ii_src_2 = (int)(floor(6.66667e-2*posz));
        int ii_src_3 = (int)(floor(6.66667e-2*posz)) + 1;
        int ii_src_4 = (int)(floor(6.66667e-2*posy)) + 1;
        int ii_src_5 = (int)(floor(6.66667e-2*posx)) + 1;
        float px = (float)(posx - 1.5e+1F*(int)(floor(6.66667e-2F*posx)));
        float py = (float)(posy - 1.5e+1F*(int)(floor(6.66667e-2F*posy)));
        float pz = (float)(posz - 1.5e+1F*(int)(floor(6.66667e-2F*posz)));
        if (ii_src_0 >= x_m - 1 && ii_src_1 >= y_m - 1 && ii_src_2 >= z_m - 1 && ii_src_0 <= x_M + 1 && ii_src_1 <= y_M + 1 && ii_src_2 <= z_M + 1)
        {
          float r2 = (dt*dt)*(vp[ii_src_0 + 8][ii_src_1 + 8][ii_src_2 + 8]*vp[ii_src_0 + 8][ii_src_1 + 8][ii_src_2 + 8])*(-2.96296e-4F*px*py*pz + 4.44445e-3F*px*py + 4.44445e-3F*px*pz - 6.66667e-2F*px + 4.44445e-3F*py*pz - 6.66667e-2F*py - 6.66667e-2F*pz + 1)*src[time][p_src];
          #pragma omp atomic update
          u[t2][ii_src_0 + 8][ii_src_1 + 8][ii_src_2 + 8] += r2;
        }
        if (ii_src_0 >= x_m - 1 && ii_src_1 >= y_m - 1 && ii_src_3 >= z_m - 1 && ii_src_0 <= x_M + 1 && ii_src_1 <= y_M + 1 && ii_src_3 <= z_M + 1)
        {
          float r3 = (dt*dt)*(vp[ii_src_0 + 8][ii_src_1 + 8][ii_src_3 + 8]*vp[ii_src_0 + 8][ii_src_1 + 8][ii_src_3 + 8])*(2.96296e-4F*px*py*pz - 4.44445e-3F*px*pz - 4.44445e-3F*py*pz + 6.66667e-2F*pz)*src[time][p_src];
          #pragma omp atomic update
          u[t2][ii_src_0 + 8][ii_src_1 + 8][ii_src_3 + 8] += r3;
        }
        if (ii_src_0 >= x_m - 1 && ii_src_2 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_0 <= x_M + 1 && ii_src_2 <= z_M + 1 && ii_src_4 <= y_M + 1)
        {
          float r4 = (dt*dt)*(vp[ii_src_0 + 8][ii_src_4 + 8][ii_src_2 + 8]*vp[ii_src_0 + 8][ii_src_4 + 8][ii_src_2 + 8])*(2.96296e-4F*px*py*pz - 4.44445e-3F*px*py - 4.44445e-3F*py*pz + 6.66667e-2F*py)*src[time][p_src];
          #pragma omp atomic update
          u[t2][ii_src_0 + 8][ii_src_4 + 8][ii_src_2 + 8] += r4;
        }
        if (ii_src_0 >= x_m - 1 && ii_src_3 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_0 <= x_M + 1 && ii_src_3 <= z_M + 1 && ii_src_4 <= y_M + 1)
        {
          float r5 = (dt*dt)*(vp[ii_src_0 + 8][ii_src_4 + 8][ii_src_3 + 8]*vp[ii_src_0 + 8][ii_src_4 + 8][ii_src_3 + 8])*(-2.96296e-4F*px*py*pz + 4.44445e-3F*py*pz)*src[time][p_src];
          #pragma omp atomic update
          u[t2][ii_src_0 + 8][ii_src_4 + 8][ii_src_3 + 8] += r5;
        }
        if (ii_src_1 >= y_m - 1 && ii_src_2 >= z_m - 1 && ii_src_5 >= x_m - 1 && ii_src_1 <= y_M + 1 && ii_src_2 <= z_M + 1 && ii_src_5 <= x_M + 1)
        {
          float r6 = (dt*dt)*(vp[ii_src_5 + 8][ii_src_1 + 8][ii_src_2 + 8]*vp[ii_src_5 + 8][ii_src_1 + 8][ii_src_2 + 8])*(2.96296e-4F*px*py*pz - 4.44445e-3F*px*py - 4.44445e-3F*px*pz + 6.66667e-2F*px)*src[time][p_src];
          #pragma omp atomic update
          u[t2][ii_src_5 + 8][ii_src_1 + 8][ii_src_2 + 8] += r6;
        }
        if (ii_src_1 >= y_m - 1 && ii_src_3 >= z_m - 1 && ii_src_5 >= x_m - 1 && ii_src_1 <= y_M + 1 && ii_src_3 <= z_M + 1 && ii_src_5 <= x_M + 1)
        {
          float r7 = (dt*dt)*(vp[ii_src_5 + 8][ii_src_1 + 8][ii_src_3 + 8]*vp[ii_src_5 + 8][ii_src_1 + 8][ii_src_3 + 8])*(-2.96296e-4F*px*py*pz + 4.44445e-3F*px*pz)*src[time][p_src];
          #pragma omp atomic update
          u[t2][ii_src_5 + 8][ii_src_1 + 8][ii_src_3 + 8] += r7;
        }
        if (ii_src_2 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_5 >= x_m - 1 && ii_src_2 <= z_M + 1 && ii_src_4 <= y_M + 1 && ii_src_5 <= x_M + 1)
        {
          float r8 = (dt*dt)*(vp[ii_src_5 + 8][ii_src_4 + 8][ii_src_2 + 8]*vp[ii_src_5 + 8][ii_src_4 + 8][ii_src_2 + 8])*(-2.96296e-4F*px*py*pz + 4.44445e-3F*px*py)*src[time][p_src];
          #pragma omp atomic update
          u[t2][ii_src_5 + 8][ii_src_4 + 8][ii_src_2 + 8] += r8;
        }
        if (ii_src_3 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_5 >= x_m - 1 && ii_src_3 <= z_M + 1 && ii_src_4 <= y_M + 1 && ii_src_5 <= x_M + 1)
        {
          float r9 = 2.96296e-4F*px*py*pz*(dt*dt)*(vp[ii_src_5 + 8][ii_src_4 + 8][ii_src_3 + 8]*vp[ii_src_5 + 8][ii_src_4 + 8][ii_src_3 + 8])*src[time][p_src];
          #pragma omp atomic update
          u[t2][ii_src_5 + 8][ii_src_4 + 8][ii_src_3 + 8] += r9;
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
        float posx = -o_x + rec_coords[p_rec][0];
        float posy = -o_y + rec_coords[p_rec][1];
        float posz = -o_z + rec_coords[p_rec][2];
        int ii_rec_0 = (int)(floor(6.66667e-2*posx));
        int ii_rec_1 = (int)(floor(6.66667e-2*posy));
        int ii_rec_2 = (int)(floor(6.66667e-2*posz));
        int ii_rec_3 = (int)(floor(6.66667e-2*posz)) + 1;
        int ii_rec_4 = (int)(floor(6.66667e-2*posy)) + 1;
        int ii_rec_5 = (int)(floor(6.66667e-2*posx)) + 1;
        float px = (float)(posx - 1.5e+1F*(int)(floor(6.66667e-2F*posx)));
        float py = (float)(posy - 1.5e+1F*(int)(floor(6.66667e-2F*posy)));
        float pz = (float)(posz - 1.5e+1F*(int)(floor(6.66667e-2F*posz)));
        float sum = 0.0F;
        if (ii_rec_0 >= x_m - 1 && ii_rec_1 >= y_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_1 <= y_M + 1 && ii_rec_2 <= z_M + 1)
        {
          sum += (-2.96296e-4F*px*py*pz + 4.44445e-3F*px*py + 4.44445e-3F*px*pz - 6.66667e-2F*px + 4.44445e-3F*py*pz - 6.66667e-2F*py - 6.66667e-2F*pz + 1)*u[t0][ii_rec_0 + 8][ii_rec_1 + 8][ii_rec_2 + 8];
        }
        if (ii_rec_0 >= x_m - 1 && ii_rec_1 >= y_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_1 <= y_M + 1 && ii_rec_3 <= z_M + 1)
        {
          sum += (2.96296e-4F*px*py*pz - 4.44445e-3F*px*pz - 4.44445e-3F*py*pz + 6.66667e-2F*pz)*u[t0][ii_rec_0 + 8][ii_rec_1 + 8][ii_rec_3 + 8];
        }
        if (ii_rec_0 >= x_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_2 <= z_M + 1 && ii_rec_4 <= y_M + 1)
        {
          sum += (2.96296e-4F*px*py*pz - 4.44445e-3F*px*py - 4.44445e-3F*py*pz + 6.66667e-2F*py)*u[t0][ii_rec_0 + 8][ii_rec_4 + 8][ii_rec_2 + 8];
        }
        if (ii_rec_0 >= x_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_3 <= z_M + 1 && ii_rec_4 <= y_M + 1)
        {
          sum += (-2.96296e-4F*px*py*pz + 4.44445e-3F*py*pz)*u[t0][ii_rec_0 + 8][ii_rec_4 + 8][ii_rec_3 + 8];
        }
        if (ii_rec_1 >= y_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_1 <= y_M + 1 && ii_rec_2 <= z_M + 1 && ii_rec_5 <= x_M + 1)
        {
          sum += (2.96296e-4F*px*py*pz - 4.44445e-3F*px*py - 4.44445e-3F*px*pz + 6.66667e-2F*px)*u[t0][ii_rec_5 + 8][ii_rec_1 + 8][ii_rec_2 + 8];
        }
        if (ii_rec_1 >= y_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_1 <= y_M + 1 && ii_rec_3 <= z_M + 1 && ii_rec_5 <= x_M + 1)
        {
          sum += (-2.96296e-4F*px*py*pz + 4.44445e-3F*px*pz)*u[t0][ii_rec_5 + 8][ii_rec_1 + 8][ii_rec_3 + 8];
        }
        if (ii_rec_2 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_2 <= z_M + 1 && ii_rec_4 <= y_M + 1 && ii_rec_5 <= x_M + 1)
        {
          sum += (-2.96296e-4F*px*py*pz + 4.44445e-3F*px*py)*u[t0][ii_rec_5 + 8][ii_rec_4 + 8][ii_rec_2 + 8];
        }
        if (ii_rec_3 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_3 <= z_M + 1 && ii_rec_4 <= y_M + 1 && ii_rec_5 <= x_M + 1)
        {
          sum += 2.96296e-4F*px*py*pz*u[t0][ii_rec_5 + 8][ii_rec_4 + 8][ii_rec_3 + 8];
        }
        rec[time][p_rec] = sum;
      }
    }
    STOP_TIMER(section2,timers)
    /* End section2 */
  }

  #pragma omp target update from(rec[0:rec_vec->size[0]][0:rec_vec->size[1]])
  #pragma omp target exit data map(release: rec[0:rec_vec->size[0]][0:rec_vec->size[1]]) if(devicerm)
  #pragma omp target update from(u[0:u_vec->size[0]][0:u_vec->size[1]][0:u_vec->size[2]][0:u_vec->size[3]])
  #pragma omp target exit data map(release: u[0:u_vec->size[0]][0:u_vec->size[1]][0:u_vec->size[2]][0:u_vec->size[3]]) if(devicerm)
  #pragma omp target exit data map(delete: damp[0:damp_vec->size[0]][0:damp_vec->size[1]][0:damp_vec->size[2]]) if(devicerm && (damp_vec->size[0] != 0) && (damp_vec->size[1] != 0) && (damp_vec->size[2] != 0))
  #pragma omp target exit data map(delete: rec_coords[0:rec_coords_vec->size[0]][0:rec_coords_vec->size[1]]) if(devicerm && (rec_coords_vec->size[0] != 0) && (rec_coords_vec->size[1] != 0))
  #pragma omp target exit data map(delete: src[0:src_vec->size[0]][0:src_vec->size[1]]) if(devicerm && (src_vec->size[0] != 0) && (src_vec->size[1] != 0))
  #pragma omp target exit data map(delete: src_coords[0:src_coords_vec->size[0]][0:src_coords_vec->size[1]]) if(devicerm && (src_coords_vec->size[0] != 0) && (src_coords_vec->size[1] != 0))
  #pragma omp target exit data map(delete: vp[0:vp_vec->size[0]][0:vp_vec->size[1]][0:vp_vec->size[2]]) if(devicerm && (vp_vec->size[0] != 0) && (vp_vec->size[1] != 0) && (vp_vec->size[2] != 0))

  return 0;
}


dataobj create_data(int size0, int size1, int size2, int size3, int elemsize) {
  dataobj a;

  a.data =  malloc(elemsize*size0*size1*size2*size3);//new char[elemsize*size0*size1*size2*size3]; // 340*340*340

  a.size = malloc(sizeof(int)*4);
  a.size[0] = size0;
  a.size[1] = size1;
  a.size[2] = size2;
  a.size[3] = size3;
  return a;
}



int main(int argc, char ** argv) {

  float dt=1.42900002, o_x=-400, o_y=-400, o_z=-400;
  int x_M=335, x_m=0, x_size=336, y_M=335, y_m=0, y_size=336, z_M=335, z_m=0, z_size=336;
  int p_rec_M=65535, p_rec_m=0, p_src_M=0, p_src_m=0, time_M=10, time_m=0;
  int x0_blk0_size=8, x1_blk0_size=8, y0_blk0_size=8;


  dataobj u_vec         = create_data(340, 340, 340, 1, sizeof(float));
  dataobj rec_vec       = create_data(876, 65536, 1, 1, sizeof(float));
  dataobj rec_coords_vec = create_data(65536, 3, 1, 1, sizeof(float));
  dataobj src_vec       = create_data(876, 1, 1, 1, sizeof(float));
  dataobj src_coords_vec = create_data(65536, 3, 1, 1, sizeof(float));
  dataobj vp_vec        = create_data(340, 340, 340, 1, sizeof(float));

  Forward(struct dataobj *restrict damp_vec,
   const float dt,
   const float o_x,
   const float o_y,
   const float o_z,
   struct dataobj *restrict rec_vec,
   struct dataobj *restrict rec_coords_vec,
   struct dataobj *restrict src_vec,
   struct dataobj *restrict src_coords_vec,
   struct dataobj *restrict u_vec,
   struct dataobj *restrict vp_vec,
   const int x_M,
   const int x_m,
   const int y_M,
   const int y_m,
   const int z_M,
   const int z_m,
   const int p_rec_M,
   const int p_rec_m,
   const int p_src_M,
   const int p_src_m,
   const int time_M,
   const int time_m,
   const int devicerm,
   const int deviceid,
   struct profiler * timers)

}
