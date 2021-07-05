
//#include <iostream>
#include "stopper.h"


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "omp.h"

/*
size_t count_t=0,sum_t;
void prt(std::string s)
{
    std::cout<<s<<": "<<count_t<<"\n";
    sum_t+=count_t;
    count_t=0;
}
*/
typedef struct dataobj dataobj;//WHY??????


struct dataobj
{
  float *__restrict data;
  int * size;
} ;

void bf0(struct dataobj *__restrict b_vec, struct dataobj *__restrict damp_vec, const float dt, struct dataobj *__restrict p_vec, struct dataobj *__restrict v_x_vec, struct dataobj *__restrict v_y_vec, struct dataobj *__restrict v_z_vec, const int t0, const int t1, const int x0_blk0_size, const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m);
void bf1(struct dataobj *__restrict b_vec, struct dataobj *__restrict damp_vec, const float dt, struct dataobj *__restrict p_vec, struct dataobj *__restrict qp_vec, struct dataobj *__restrict r_vec, float *__restrict r1_vec, struct dataobj *__restrict v_x_vec, struct dataobj *__restrict v_y_vec, struct dataobj *__restrict v_z_vec, struct dataobj *__restrict vp_vec, const int x_size, const int y_size, const int z_size, const int t0, const int t1, const int x1_blk0_size, const int x_M, const int x_m, const int y0_blk0_size, const int y_M, const int y_m, const int z_M, const int z_m);

int Forward(struct dataobj *__restrict b_vec, 
            struct dataobj *__restrict damp_vec, const float dt, const float o_x, const float o_y, const float o_z,
            struct dataobj *__restrict p_vec, 
            struct dataobj *__restrict qp_vec, 
            struct dataobj *__restrict r_vec,
            struct dataobj *__restrict rec_vec, 
            struct dataobj *__restrict rec_coords_vec, 
            struct dataobj *__restrict src_vec,
            struct dataobj *__restrict src_coords_vec, 
            struct dataobj *__restrict v_x_vec, 
            struct dataobj *__restrict v_y_vec, 
            struct dataobj *__restrict v_z_vec, 
            struct dataobj *__restrict vp_vec, const int x_M, const int x_m, const int x_size,
             const int y_M, const int y_m, const int y_size, const int z_M, const int z_m, const int z_size, const int p_rec_M,
              const int p_rec_m, const int p_src_M, const int p_src_m, const int time_M, const int time_m, const int x0_blk0_size,
               const int x1_blk0_size, const int y0_blk0_size)
{

/*all dataobj data is void
  lets convert it to float * pointer
*/
  //float(*__restrict b_data_start) = (float*) b_vec->data;
  float (*__restrict b)[b_vec->size[1]][b_vec->size[2]]                           = (float (*)[b_vec->size[1]][b_vec->size[2]]) b_vec->data;
  float (*__restrict damp)[damp_vec->size[1]][damp_vec->size[2]]                  = (float (*)[damp_vec->size[1]][damp_vec->size[2]]) damp_vec->data;
  float (*__restrict p)[p_vec->size[1]][p_vec->size[2]][p_vec->size[3]]           = (float (*)[p_vec->size[1]][p_vec->size[2]][p_vec->size[3]]) p_vec->data;
  float (*__restrict qp)[qp_vec->size[1]][qp_vec->size[2]]                        = (float (*)[qp_vec->size[1]][qp_vec->size[2]]) qp_vec->data;
  float (*__restrict r)[r_vec->size[1]][r_vec->size[2]][r_vec->size[3]]           = (float (*)[r_vec->size[1]][r_vec->size[2]][r_vec->size[3]]) r_vec->data;
  float (*__restrict rec)[rec_vec->size[1]]                                       = (float (*)[rec_vec->size[1]]) rec_vec->data;
  float (*__restrict rec_coords)[rec_coords_vec->size[1]]                         = (float (*)[rec_coords_vec->size[1]]) rec_coords_vec->data;
  float (*__restrict src)[src_vec->size[1]]                                       = (float (*)[src_vec->size[1]]) src_vec->data;
  float (*__restrict src_coords)[src_coords_vec->size[1]]                         = (float (*)[src_coords_vec->size[1]]) src_coords_vec->data;
  float (*__restrict v_x)[v_x_vec->size[1]][v_x_vec->size[2]][v_x_vec->size[3]]   = (float (*)[v_x_vec->size[1]][v_x_vec->size[2]][v_x_vec->size[3]]) v_x_vec->data;
  float (*__restrict v_y)[v_y_vec->size[1]][v_y_vec->size[2]][v_y_vec->size[3]]   = (float (*)[v_y_vec->size[1]][v_y_vec->size[2]][v_y_vec->size[3]]) v_y_vec->data;
  float (*__restrict v_z)[v_z_vec->size[1]][v_z_vec->size[2]][v_z_vec->size[3]]   = (float (*)[v_z_vec->size[1]][v_z_vec->size[2]][v_z_vec->size[3]]) v_z_vec->data;
  float (*__restrict vp)[vp_vec->size[1]][vp_vec->size[2]]                        = (float (*)[vp_vec->size[1]][vp_vec->size[2]]) vp_vec->data;



  //printf("eljut-1 \n");
  float* r1_=malloc(x_size*y_size*z_size*sizeof(float));
  float (*__restrict r1)[y_size][z_size] = (float (*)[y_size][z_size])r1_;//malloc(x_size*y_size*z_size*sizeof(float)) ;


  
//printf("eljut %d %d %d    \n",x_size,y_size,z_size);

//char offloadname[10]="Offload";
       Spawn_stopper("Offload");

#pragma omp target data map(tofrom: b[0:b_vec->size[0]][0:b_vec->size[1]][0:b_vec->size[2]]), \
            map(tofrom: damp[0:damp_vec->size[0]][0:damp_vec->size[1]][0:damp_vec->size[2]]),\
            map(tofrom: p[0:p_vec->size[0]][0:p_vec->size[1]][0:p_vec->size[2]][0:p_vec->size[3]]),\
            map(tofrom: qp[0:qp_vec->size[0]][0:qp_vec->size[1]][0:qp_vec->size[2]]),\
            map(tofrom: r[0:r_vec->size[0]][0:r_vec->size[1]][0:r_vec->size[2]][0:r_vec->size[3]]),\
            map(tofrom: rec[0:rec_vec->size[0]][0:rec_vec->size[1]]),\
            map(tofrom: rec_coords[0:rec_coords_vec->size[0]][0:rec_coords_vec->size[1]]),\
            map(tofrom: src[0:src_vec->size[0]][0:src_vec->size[1]]),\
            map(tofrom: src_coords[0:src_coords_vec->size[0]][0:src_coords_vec->size[1]]),\
            map(tofrom: v_x[0:v_x_vec->size[0]][0:v_x_vec->size[1]][0:v_x_vec->size[2]][0:v_x_vec->size[3]]),\
            map(tofrom: v_y[0:v_y_vec->size[0]][0:v_y_vec->size[1]][0:v_y_vec->size[2]][0:v_y_vec->size[3]]),\
            map(tofrom: v_z[0:v_z_vec->size[0]][0:v_z_vec->size[1]][0:v_z_vec->size[2]][0:v_z_vec->size[3]]),\
            map(tofrom: vp[0:vp_vec->size[0]][0:vp_vec->size[1]][0:vp_vec->size[2]])\
            map(alloc: r1[0:x_size][0:y_size][0:z_size])
            
            //map(alloc: r1_[0:x_size*y_size*z_size]) 

  {

    Kill_stopper();



  

  Spawn_stopper("Section 0");
#pragma omp target teams distribute parallel for collapse(3)
   for (int x = x_m; x <= x_M; x += 1)
    {
      for (int y = y_m; y <= y_M; y += 1)
      {
        for (int z = z_m; z <= z_M; z += 1)
        {
          r1[x][y][z] = sqrt(1.0F + 1.0F / pow(qp[x + 2][y + 2][z + 2], 2));
        }
      }
    }
  Kill_stopper();

    //count_t*=4;
    //prt("Section 0");
    //return 0;
    /* End section0 */
    Spawn_stopper("Time for loop");
    for (int time = time_m, t0 = (time) % (2), t1 = (time + 1) % (2); time <= time_M; time += 1, t0 = (time) % (2), t1 = (time + 1) % (2))
    {
      /* Begin section1 */
       //Spawn_stopper("Section 1");
      
      {

          // Timer t("Section 1");
          bf0(b_vec,damp_vec,dt,p_vec,v_x_vec,v_y_vec,v_z_vec,t0,t1,x0_blk0_size,x_M - (x_M - x_m + 1)%(x0_blk0_size),x_m,y_M,y_m,z_M,z_m);

          bf0(b_vec,damp_vec,dt,p_vec,v_x_vec,v_y_vec,v_z_vec,t0,t1,(x_M - x_m + 1)%(x0_blk0_size),x_M,x_M - (x_M - x_m + 1)%(x0_blk0_size) + 1,y_M,y_m,z_M,z_m);

          bf1(b_vec,damp_vec,dt,p_vec,qp_vec,r_vec,(float *)r1,v_x_vec,v_y_vec,v_z_vec,vp_vec,x_size,y_size,z_size,t0,t1,x1_blk0_size,x_M - (x_M - x_m + 1)%(x1_blk0_size),x_m,y0_blk0_size,y_M - (y_M - y_m + 1)%(y0_blk0_size),y_m,z_M,z_m);

          bf1(b_vec,damp_vec,dt,p_vec,qp_vec,r_vec,(float *)r1,v_x_vec,v_y_vec,v_z_vec,vp_vec,x_size,y_size,z_size,t0,t1,x1_blk0_size,x_M - (x_M - x_m + 1)%(x1_blk0_size),x_m,(y_M - y_m + 1)%(y0_blk0_size),y_M,y_M - (y_M - y_m + 1)%(y0_blk0_size) + 1,z_M,z_m);

          bf1(b_vec,damp_vec,dt,p_vec,qp_vec,r_vec,(float *)r1,v_x_vec,v_y_vec,v_z_vec,vp_vec,x_size,y_size,z_size,t0,t1,(x_M - x_m + 1)%(x1_blk0_size),x_M,x_M - (x_M - x_m + 1)%(x1_blk0_size) + 1,y0_blk0_size,y_M - (y_M - y_m + 1)%(y0_blk0_size),y_m,z_M,z_m);

          bf1(b_vec,damp_vec,dt,p_vec,qp_vec,r_vec,(float *)r1,v_x_vec,v_y_vec,v_z_vec,vp_vec,x_size,y_size,z_size,t0,t1,(x_M - x_m + 1)%(x1_blk0_size),x_M,x_M - (x_M - x_m + 1)%(x1_blk0_size) + 1,(y_M - y_m + 1)%(y0_blk0_size),y_M,y_M - (y_M - y_m + 1)%(y0_blk0_size) + 1,z_M,z_m);

      }
      //Kill_stopper();
      /* End section1 */
      /* Begin section2 */
      
      //Spawn_stopper("Section 2");
      /*{
          // Timer t("Section 2");
          for (int p_src = p_src_m; p_src <= p_src_M; p_src += 1)
        {
          float posx = -o_x + src_coords[p_src][0];
          float posy = -o_y + src_coords[p_src][1];
          float posz = -o_z + src_coords[p_src][2];
          int ii_src_0 = (int)(floor(1.0e-1*posx));
          int ii_src_1 = (int)(floor(1.0e-1*posy));
          int ii_src_2 = (int)(floor(1.0e-1*posz));
          int ii_src_3 = (int)(floor(1.0e-1*posz)) + 1;
          int ii_src_4 = (int)(floor(1.0e-1*posy)) + 1;
          int ii_src_5 = (int)(floor(1.0e-1*posx)) + 1;
          float px = (float)(posx - 1.0e+1F*(int)(floor(1.0e-1F*posx)));
          float py = (float)(posy - 1.0e+1F*(int)(floor(1.0e-1F*posy)));
          float pz = (float)(posz - 1.0e+1F*(int)(floor(1.0e-1F*posz)));
          if (ii_src_0 >= x_m - 1 && ii_src_1 >= y_m - 1 && ii_src_2 >= z_m - 1 && ii_src_0 <= x_M + 1 && ii_src_1 <= y_M + 1 && ii_src_2 <= z_M + 1)
          {
            float r2 = dt*(-1.0e-3F*px*py*pz + 1.0e-2F*px*py + 1.0e-2F*px*pz - 1.0e-1F*px + 1.0e-2F*py*pz - 1.0e-1F*py - 1.0e-1F*pz + 1)*src[time][p_src];
            p[t1][ii_src_0 + 2][ii_src_1 + 2][ii_src_2 + 2] += r2;
          }
          if (ii_src_0 >= x_m - 1 && ii_src_1 >= y_m - 1 && ii_src_3 >= z_m - 1 && ii_src_0 <= x_M + 1 && ii_src_1 <= y_M + 1 && ii_src_3 <= z_M + 1)
          {
            float r3 = dt*(1.0e-3F*px*py*pz - 1.0e-2F*px*pz - 1.0e-2F*py*pz + 1.0e-1F*pz)*src[time][p_src];
            p[t1][ii_src_0 + 2][ii_src_1 + 2][ii_src_3 + 2] += r3;
          }
          if (ii_src_0 >= x_m - 1 && ii_src_2 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_0 <= x_M + 1 && ii_src_2 <= z_M + 1 && ii_src_4 <= y_M + 1)
          {
            float r4 = dt*(1.0e-3F*px*py*pz - 1.0e-2F*px*py - 1.0e-2F*py*pz + 1.0e-1F*py)*src[time][p_src];
            p[t1][ii_src_0 + 2][ii_src_4 + 2][ii_src_2 + 2] += r4;
          }
          if (ii_src_0 >= x_m - 1 && ii_src_3 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_0 <= x_M + 1 && ii_src_3 <= z_M + 1 && ii_src_4 <= y_M + 1)
          {
            float r5 = dt*(-1.0e-3F*px*py*pz + 1.0e-2F*py*pz)*src[time][p_src];
            p[t1][ii_src_0 + 2][ii_src_4 + 2][ii_src_3 + 2] += r5;
          }
          if (ii_src_1 >= y_m - 1 && ii_src_2 >= z_m - 1 && ii_src_5 >= x_m - 1 && ii_src_1 <= y_M + 1 && ii_src_2 <= z_M + 1 && ii_src_5 <= x_M + 1)
          {
            float r6 = dt*(1.0e-3F*px*py*pz - 1.0e-2F*px*py - 1.0e-2F*px*pz + 1.0e-1F*px)*src[time][p_src];
            p[t1][ii_src_5 + 2][ii_src_1 + 2][ii_src_2 + 2] += r6;
          }
          if (ii_src_1 >= y_m - 1 && ii_src_3 >= z_m - 1 && ii_src_5 >= x_m - 1 && ii_src_1 <= y_M + 1 && ii_src_3 <= z_M + 1 && ii_src_5 <= x_M + 1)
          {
            float r7 = dt*(-1.0e-3F*px*py*pz + 1.0e-2F*px*pz)*src[time][p_src];
            p[t1][ii_src_5 + 2][ii_src_1 + 2][ii_src_3 + 2] += r7;
          }
          if (ii_src_2 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_5 >= x_m - 1 && ii_src_2 <= z_M + 1 && ii_src_4 <= y_M + 1 && ii_src_5 <= x_M + 1)
          {
            float r8 = dt*(-1.0e-3F*px*py*pz + 1.0e-2F*px*py)*src[time][p_src];
            p[t1][ii_src_5 + 2][ii_src_4 + 2][ii_src_2 + 2] += r8;
          }
          if (ii_src_3 >= z_m - 1 && ii_src_4 >= y_m - 1 && ii_src_5 >= x_m - 1 && ii_src_3 <= z_M + 1 && ii_src_4 <= y_M + 1 && ii_src_5 <= x_M + 1)
          {
            float r9 = 1.0e-3F*px*py*pz*dt*src[time][p_src];
            p[t1][ii_src_5 + 2][ii_src_4 + 2][ii_src_3 + 2] += r9;
          }
        }
      }
      */
      //Kill_stopper();

      /* End section2 */
      /* Begin section3 */
      
      //Spawn_stopper("Section 3");
      {
        //Timer t("Section 3");
        #pragma omp target teams distribute parallel for
        for (int p_rec = p_rec_m; p_rec <= p_rec_M; p_rec += 1)
        {
          float posx = -o_x + rec_coords[p_rec][0];
          float posy = -o_y + rec_coords[p_rec][1];
          float posz = -o_z + rec_coords[p_rec][2];
          int ii_rec_0 = (int)(floor(1.0e-1*posx));
          int ii_rec_1 = (int)(floor(1.0e-1*posy));
          int ii_rec_2 = (int)(floor(1.0e-1*posz));
          int ii_rec_3 = (int)(floor(1.0e-1*posz)) + 1;
          int ii_rec_4 = (int)(floor(1.0e-1*posy)) + 1;
          int ii_rec_5 = (int)(floor(1.0e-1*posx)) + 1;
          float px = (float)(posx - 1.0e+1F*(int)(floor(1.0e-1F*posx)));
          float py = (float)(posy - 1.0e+1F*(int)(floor(1.0e-1F*posy)));
          float pz = (float)(posz - 1.0e+1F*(int)(floor(1.0e-1F*posz)));
          float sum = 0.0F;
          if (ii_rec_0 >= x_m - 1 && ii_rec_1 >= y_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_1 <= y_M + 1 && ii_rec_2 <= z_M + 1)
          {
            sum += (-1.0e-3F*px*py*pz + 1.0e-2F*px*py + 1.0e-2F*px*pz - 1.0e-1F*px + 1.0e-2F*py*pz - 1.0e-1F*py - 1.0e-1F*pz + 1)*p[t0][ii_rec_0 + 2][ii_rec_1 + 2][ii_rec_2 + 2];
          }
          if (ii_rec_0 >= x_m - 1 && ii_rec_1 >= y_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_1 <= y_M + 1 && ii_rec_3 <= z_M + 1)
          {
            sum += (1.0e-3F*px*py*pz - 1.0e-2F*px*pz - 1.0e-2F*py*pz + 1.0e-1F*pz)*p[t0][ii_rec_0 + 2][ii_rec_1 + 2][ii_rec_3 + 2];
          }
          if (ii_rec_0 >= x_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_2 <= z_M + 1 && ii_rec_4 <= y_M + 1)
          {
            sum += (1.0e-3F*px*py*pz - 1.0e-2F*px*py - 1.0e-2F*py*pz + 1.0e-1F*py)*p[t0][ii_rec_0 + 2][ii_rec_4 + 2][ii_rec_2 + 2];
          }
          if (ii_rec_0 >= x_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_0 <= x_M + 1 && ii_rec_3 <= z_M + 1 && ii_rec_4 <= y_M + 1)
          {
            sum += (-1.0e-3F*px*py*pz + 1.0e-2F*py*pz)*p[t0][ii_rec_0 + 2][ii_rec_4 + 2][ii_rec_3 + 2];
          }
          if (ii_rec_1 >= y_m - 1 && ii_rec_2 >= z_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_1 <= y_M + 1 && ii_rec_2 <= z_M + 1 && ii_rec_5 <= x_M + 1)
          {
            sum += (1.0e-3F*px*py*pz - 1.0e-2F*px*py - 1.0e-2F*px*pz + 1.0e-1F*px)*p[t0][ii_rec_5 + 2][ii_rec_1 + 2][ii_rec_2 + 2];
          }
          if (ii_rec_1 >= y_m - 1 && ii_rec_3 >= z_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_1 <= y_M + 1 && ii_rec_3 <= z_M + 1 && ii_rec_5 <= x_M + 1)
          {
            sum += (-1.0e-3F*px*py*pz + 1.0e-2F*px*pz)*p[t0][ii_rec_5 + 2][ii_rec_1 + 2][ii_rec_3 + 2];
          }
          if (ii_rec_2 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_2 <= z_M + 1 && ii_rec_4 <= y_M + 1 && ii_rec_5 <= x_M + 1)
          {
            sum += (-1.0e-3F*px*py*pz + 1.0e-2F*px*py)*p[t0][ii_rec_5 + 2][ii_rec_4 + 2][ii_rec_2 + 2];
          }
          if (ii_rec_3 >= z_m - 1 && ii_rec_4 >= y_m - 1 && ii_rec_5 >= x_m - 1 && ii_rec_3 <= z_M + 1 && ii_rec_4 <= y_M + 1 && ii_rec_5 <= x_M + 1)
          {
            sum += 1.0e-3F*px*py*pz*p[t0][ii_rec_5 + 2][ii_rec_4 + 2][ii_rec_3 + 2];
          }
          rec[time][p_rec] = sum;
        }
      }
       //Kill_stopper();/* End section3 */
    }
    Kill_stopper();//Tiem loop end

  Spawn_stopper("Back to Ram");
  }
  Kill_stopper();
  //free(r1_);
  
  return 0;
}
  

void bf0(struct dataobj *__restrict b_vec, struct dataobj *__restrict damp_vec, const float dt, struct dataobj *__restrict p_vec, struct dataobj *__restrict v_x_vec, struct dataobj *__restrict v_y_vec, struct dataobj *__restrict v_z_vec, const int t0, const int t1, const int x0_blk0_size, const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m)
{

    //Timer t("bf0");
  float (*__restrict b)[b_vec->size[1]][b_vec->size[2]]                           = (float (*)[b_vec->size[1]][b_vec->size[2]]) b_vec->data;
  float (*__restrict damp)[damp_vec->size[1]][damp_vec->size[2]]                  = (float (*)[damp_vec->size[1]][damp_vec->size[2]]) damp_vec->data;
  float (*__restrict p)[p_vec->size[1]][p_vec->size[2]][p_vec->size[3]]           = (float (*)[p_vec->size[1]][p_vec->size[2]][p_vec->size[3]]) p_vec->data;
  float (*__restrict v_x)[v_x_vec->size[1]][v_x_vec->size[2]][v_x_vec->size[3]]   = (float (*)[v_x_vec->size[1]][v_x_vec->size[2]][v_x_vec->size[3]]) v_x_vec->data;
  float (*__restrict v_y)[v_y_vec->size[1]][v_y_vec->size[2]][v_y_vec->size[3]]   = (float (*)[v_y_vec->size[1]][v_y_vec->size[2]][v_y_vec->size[3]]) v_y_vec->data;
  float (*__restrict v_z)[v_z_vec->size[1]][v_z_vec->size[2]][v_z_vec->size[3]]   = (float (*)[v_z_vec->size[1]][v_z_vec->size[2]][v_z_vec->size[3]]) v_z_vec->data;

  #pragma omp target teams distribute parallel for collapse(3)
  for (int x = x_m; x <= x_M; x += 1)
  {
    for (int y = y_m; y <= y_M; y += 1)
    {

      for (int z = z_m; z <= z_M; z += 1)
      {//1                                                         2                        2                          3                            3                               4                           5
        v_x[t1][x + 2][y + 2][z + 2] = (-5.00000007450581e-2F*dt*(b[x + 2][y + 2][z + 2] + b[x + 3][y + 2][z + 2])*(-p[t0][x + 2][y + 2][z + 2] + p[t0][x + 3][y + 2][z + 2]) + v_x[t0][x + 2][y + 2][z + 2])*damp[x + 1][y + 1][z + 1];
        //count_t++;
      }
    }
  }

  #pragma omp target teams distribute parallel for collapse(3)
  for (int x = x_m; x <= x_M; x += 1)
  {
    for (int y = y_m; y <= y_M; y += 1)
    {
      for (int z = z_m; z <= z_M; z += 1)
      {
        v_y[t1][x + 2][y + 2][z + 2] = (-5.00000007450581e-2F*dt*(b[x + 2][y + 2][z + 2] + b[x + 2][y + 3][z + 2])*(-p[t0][x + 2][y + 2][z + 2] + p[t0][x + 2][y + 3][z + 2]) + v_y[t0][x + 2][y + 2][z + 2])*damp[x + 1][y + 1][z + 1];

      }
    }
  }
  #pragma omp target teams distribute parallel for collapse(3)
  for (int x = x_m; x <= x_M; x += 1)
  {
    for (int y = y_m; y <= y_M; y += 1)
    {

      for (int z = z_m; z <= z_M; z += 1)
      {
        v_z[t1][x + 2][y + 2][z + 2] = (-5.00000007450581e-2F*dt*(b[x + 2][y + 2][z + 2] + b[x + 2][y + 2][z + 3])*(-p[t0][x + 2][y + 2][z + 2] + p[t0][x + 2][y + 2][z + 3]) + v_z[t0][x + 2][y + 2][z + 2])*damp[x + 1][y + 1][z + 1];
      }
    }
  }
  
  //count_t*=15*4;

}

void bf1(struct dataobj *__restrict b_vec, struct dataobj *__restrict damp_vec, const float dt, struct dataobj *__restrict p_vec, struct dataobj *__restrict qp_vec, struct dataobj *__restrict r_vec, float *__restrict r1_vec, struct dataobj *__restrict v_x_vec, struct dataobj *__restrict v_y_vec, struct dataobj *__restrict v_z_vec, struct dataobj *__restrict vp_vec, const int x_size, const int y_size, const int z_size, const int t0, const int t1, const int x1_blk0_size, const int x_M, const int x_m, const int y0_blk0_size, const int y_M, const int y_m, const int z_M, const int z_m)
{
   
  float (*__restrict b)[b_vec->size[1]][b_vec->size[2]]                           = (float (*)[b_vec->size[1]][b_vec->size[2]]) b_vec->data;
  float (*__restrict damp)[damp_vec->size[1]][damp_vec->size[2]]                  = (float (*)[damp_vec->size[1]][damp_vec->size[2]]) damp_vec->data;
  float (*__restrict p)[p_vec->size[1]][p_vec->size[2]][p_vec->size[3]]           = (float (*)[p_vec->size[1]][p_vec->size[2]][p_vec->size[3]]) p_vec->data;
  float (*__restrict qp)[qp_vec->size[1]][qp_vec->size[2]]                        = (float (*)[qp_vec->size[1]][qp_vec->size[2]]) qp_vec->data;
  float (*__restrict r)[r_vec->size[1]][r_vec->size[2]][r_vec->size[3]]           = (float (*)[r_vec->size[1]][r_vec->size[2]][r_vec->size[3]]) r_vec->data;
  float (*__restrict r1)[y_size][z_size]                                          = (float (*)[y_size][z_size]) r1_vec;
  float (*__restrict v_x)[v_x_vec->size[1]][v_x_vec->size[2]][v_x_vec->size[3]]   = (float (*)[v_x_vec->size[1]][v_x_vec->size[2]][v_x_vec->size[3]]) v_x_vec->data;
  float (*__restrict v_y)[v_y_vec->size[1]][v_y_vec->size[2]][v_y_vec->size[3]]   = (float (*)[v_y_vec->size[1]][v_y_vec->size[2]][v_y_vec->size[3]]) v_y_vec->data;
  float (*__restrict v_z)[v_z_vec->size[1]][v_z_vec->size[2]][v_z_vec->size[3]]   = (float (*)[v_z_vec->size[1]][v_z_vec->size[2]][v_z_vec->size[3]]) v_z_vec->data;
  float (*__restrict vp)[vp_vec->size[1]][vp_vec->size[2]]                        = (float (*)[vp_vec->size[1]][vp_vec->size[2]]) vp_vec->data;



#pragma omp target teams distribute parallel for collapse(3)
  for (int x = x_m; x <= x_M; x += 1)
  {
    for (int y = y_m; y <= y_M; y += 1)
    {
      for (int z = z_m; z <= z_M; z += 1)
      {//10 ---11 *4
        float r36 = vp[x + 2][y + 2][z + 2]*vp[x + 2][y + 2][z + 2];
        float r35 = -v_x[t1][x + 1][y + 2][z + 2] + v_x[t1][x + 2][y + 2][z + 2] - v_y[t1][x + 2][y + 1][z + 2] + v_y[t1][x + 2][y + 2][z + 2] - v_z[t1][x + 2][y + 2][z + 1] + v_z[t1][x + 2][y + 2][z + 2];
        float r34 = 1.0/b[x + 2][y + 2][z + 2];
        float r33 = 1.0/qp[x + 2][y + 2][z + 2];
        float r32 = 1.0/(-1.0e-2F*r33 + 1.0e-2F*r1[x][y][z]);
        float r31 = 1.0/(-1.0e+2F*r33 + 1.0e+2F*r1[x][y][z]);
        r[t1][x + 1][y + 1][z + 1] = (r31*(-1.00000001490116e-1F*r34*r35*r36*dt*(r31*(1.0F*r32) - 1.0F) - 1.0F*dt*r[t0][x + 1][y + 1][z + 1]) + r[t0][x + 1][y + 1][z + 1])*damp[x + 1][y + 1][z + 1];
        p[t1][x + 2][y + 2][z + 2] = (r31*(-1.00000001490116e-1F*r32*r34*r35*r36*dt) + dt*(-r[t1][x + 1][y + 1][z + 1]) + p[t0][x + 2][y + 2][z + 2])*damp[x + 1][y + 1][z + 1];
        
        //count_t++;
      }
    }
  }
         

//count_t*=11*4;
}

size_t total_memory_needed=0;
dataobj create_data(int size0, int size1, int size2, int size3, int elemsize) {
  dataobj a;
  a.data =  malloc(elemsize*size0*size1*size2*size3);//new char[elemsize*size0*size1*size2*size3]; // 340*340*340
  total_memory_needed+=elemsize*size0*size1*size2*size3;
  a.size = malloc(sizeof(int)*4);//new int[4];
  a.size[0] = size0;
  a.size[1] = size1;
  a.size[2] = size2;
  a.size[3] = size3;
  return a;
}

int main(int argc, char ** argv) {

    //Timer setting
    //Timer_filename="base.txt";
    Stopper_Filemode=false;



  float dt=1.42900002, o_x=-400, o_y=-400, o_z=-400;
  int x_M=335, x_m=0, x_size=336, y_M=335, y_m=0, y_size=336, z_M=335, z_m=0, z_size=336;
  int p_rec_M=65535, p_rec_m=0, p_src_M=0, p_src_m=0, time_M=10, time_m=0;
  int x0_blk0_size=8, x1_blk0_size=8, y0_blk0_size=8;


  dataobj b_vec         = create_data(340, 340, 340, 1, sizeof(float));
  dataobj damp_vec      = create_data(338, 338, 338, 1, sizeof(float));
  dataobj p_vec         = create_data(2, 340, 340, 340, sizeof(float));
  dataobj qp_vec        = create_data(340, 340, 340, 1, sizeof(float));
  dataobj r_vec         = create_data(2, 340, 340, 340, sizeof(float));
  dataobj rec_vec       = create_data(876, 65536, 1, 1, sizeof(float));
  dataobj rec_coords_vec = create_data(65536, 3, 1, 1, sizeof(float));
  dataobj src_vec       = create_data(876, 1, 1, 1, sizeof(float));
  dataobj src_coords_vec = create_data(65536, 3, 1, 1, sizeof(float));
  dataobj v_x_vec       = create_data(2, 340, 340, 340, sizeof(float));
  dataobj v_y_vec       = create_data(2, 340, 340, 340, sizeof(float));
  dataobj v_z_vec       = create_data(2, 340, 340, 340, sizeof(float));
  dataobj vp_vec        = create_data(340, 340, 340, 1, sizeof(float));

  printf("took %f GB memory needed.\n", (float)total_memory_needed / 1000000000);

  //timer settings
  //Timer_Filemode=0;
  //Timer_buffer=malloc(1);
  //*Timer_buffer='\0';
  
  Spawn_stopper("Forward");


  Forward(&b_vec, &damp_vec, dt, o_x, o_y, o_z, &p_vec, &qp_vec, &r_vec, &rec_vec, &rec_coords_vec, &src_vec,
    &src_coords_vec, &v_x_vec, &v_y_vec, &v_z_vec, &vp_vec, x_M, x_m, x_size, y_M, y_m, y_size, z_M, z_m,
    z_size, p_rec_M, p_rec_m, p_src_M, p_src_m, time_M, time_m, x0_blk0_size, x1_blk0_size, y0_blk0_size);

  Kill_stopper();

  //Stopper_print2file("viscoacoustic_gpu_v100.meres.txt");

  return 0;

}

