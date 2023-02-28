//2 800
dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
dataobj rec_vec = create_data( 585,640000,1,1,sizeof(float));
dataobj rec_coords_vec = create_data( 640000,3,1,1,sizeof(float));
dataobj src_vec = create_data( 585,1,1,1,sizeof(float));
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
int time_M = 583;
int time_m = 1;
int deviceid = -1;
int devicerm = 1;
        {
          float r10 = 1.0F/(vpL0(x + 2, y + 2, z + 2)*vpL0(x + 2, y + 2, z + 2));
          uL0(t2, x + 2, y + 2, z + 2) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 2, y + 2, z + 2) + r10*(-r8*(-2.0F*uL0(t0, x + 2, y + 2, z + 2)) - r8*uL0(t1, x + 2, y + 2, z + 2)) + 4.44444e-3F*(uL0(t0, x + 1, y + 2, z + 2) + uL0(t0, x + 2, y + 1, z + 2) + uL0(t0, x + 2, y + 2, z + 1) + uL0(t0, x + 2, y + 2, z + 3) + uL0(t0, x + 2, y + 3, z + 2) + uL0(t0, x + 3, y + 2, z + 2)) - 2.66666673e-2F*uL0(t0, x + 2, y + 2, z + 2))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
        }


/////////////////////////////////////////////////////////////////////////////////////////////////////////

//4 800
dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
dataobj rec_vec = create_data( 645,640000,1,1,sizeof(float));
dataobj rec_coords_vec = create_data( 640000,3,1,1,sizeof(float));
dataobj src_vec = create_data( 645,1,1,1,sizeof(float));
dataobj src_coords_vec = create_data( 1,3,1,1,sizeof(float));
dataobj u_vec = create_data( 3,888,888,888,sizeof(float));
dataobj vp_vec = create_data( 888,888,888,1,sizeof(float));
int x_M = 879;
int x_m = 0;
int y_M = 879;
int y_m = 0;
int z_M = 879;
int z_m = 0;
float dt = 1.941000;
float o_x = -600.000000;
float o_y = -600.000000;
float o_z = -600.000000;
int p_rec_M = 639999;
int p_rec_m = 0;
int p_src_M = 0;
int p_src_m = 0;
int time_M = 643;
int time_m = 1;
int deviceid = -1;
int devicerm = 1;

        {
          float r10 = 1.0F/(vpL0(x + 4, y + 4, z + 4)*vpL0(x + 4, y + 4, z + 4));
          uL0(t2, x + 4, y + 4, z + 4) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 4, y + 4, z + 4) + r10*(-r8*(-2.0F*uL0(t0, x + 4, y + 4, z + 4)) - r8*uL0(t1, x + 4, y + 4, z + 4)) + 3.70370379e-4F*(-uL0(t0, x + 2, y + 4, z + 4) - uL0(t0, x + 4, y + 2, z + 4) - uL0(t0, x + 4, y + 4, z + 2) - uL0(t0, x + 4, y + 4, z + 6) - uL0(t0, x + 4, y + 6, z + 4) - uL0(t0, x + 6, y + 4, z + 4)) + 5.92592607e-3F*(uL0(t0, x + 3, y + 4, z + 4) + uL0(t0, x + 4, y + 3, z + 4) + uL0(t0, x + 4, y + 4, z + 3) + uL0(t0, x + 4, y + 4, z + 5) + uL0(t0, x + 4, y + 5, z + 4) + uL0(t0, x + 5, y + 4, z + 4)) - 3.33333341e-2F*uL0(t0, x + 4, y + 4, z + 4))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//8 800
dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
dataobj rec_vec = create_data( 690,640000,1,1,sizeof(float));
dataobj rec_coords_vec = create_data( 640000,3,1,1,sizeof(float));
dataobj src_vec = create_data( 690,1,1,1,sizeof(float));
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
int time_M = 688;
int time_m = 1;
int deviceid = -1;
int devicerm = 1;

        {
          float r10 = 1.0F/(vpL0(x + 8, y + 8, z + 8)*vpL0(x + 8, y + 8, z + 8));
          uL0(t2, x + 8, y + 8, z + 8) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 8, y + 8, z + 8) + r10*(-r8*(-2.0F*uL0(t0, x + 8, y + 8, z + 8)) - r8*uL0(t1, x + 8, y + 8, z + 8)) + 7.93650813e-6F*(-uL0(t0, x + 4, y + 8, z + 8) - uL0(t0, x + 8, y + 4, z + 8) - uL0(t0, x + 8, y + 8, z + 4) - uL0(t0, x + 8, y + 8, z + 12) - uL0(t0, x + 8, y + 12, z + 8) - uL0(t0, x + 12, y + 8, z + 8)) + 1.12874782e-4F*(uL0(t0, x + 5, y + 8, z + 8) + uL0(t0, x + 8, y + 5, z + 8) + uL0(t0, x + 8, y + 8, z + 5) + uL0(t0, x + 8, y + 8, z + 11) + uL0(t0, x + 8, y + 11, z + 8) + uL0(t0, x + 11, y + 8, z + 8)) + 8.8888891e-4F*(-uL0(t0, x + 6, y + 8, z + 8) - uL0(t0, x + 8, y + 6, z + 8) - uL0(t0, x + 8, y + 8, z + 6) - uL0(t0, x + 8, y + 8, z + 10) - uL0(t0, x + 8, y + 10, z + 8) - uL0(t0, x + 10, y + 8, z + 8)) + 7.11111128e-3F*(uL0(t0, x + 7, y + 8, z + 8) + uL0(t0, x + 8, y + 7, z + 8) + uL0(t0, x + 8, y + 8, z + 7) + uL0(t0, x + 8, y + 8, z + 9) + uL0(t0, x + 8, y + 9, z + 8) + uL0(t0, x + 9, y + 8, z + 8)) - 3.79629639e-2F*uL0(t0, x + 8, y + 8, z + 8))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
        }


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//16 800
dataobj damp_vec = create_data( 882,882,882,1,sizeof(float));
dataobj rec_vec = create_data( 721,640000,1,1,sizeof(float));
dataobj rec_coords_vec = create_data( 640000,3,1,1,sizeof(float));
dataobj src_vec = create_data( 721,1,1,1,sizeof(float));
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
int time_M = 10;
int time_m = 1;
int deviceid = -1;
int devicerm = 1;


        {
          float r10 = 1.0F/(vpL0(x + 16, y + 16, z + 16)*vpL0(x + 16, y + 16, z + 16));
          uL0(t2, x + 16, y + 16, z + 16) = (r9*dampL0(x + 1, y + 1, z + 1)*uL0(t0, x + 16, y + 16, z + 16) + r10*(-r8*(-2.0F*uL0(t0, x + 16, y + 16, z + 16)) - r8*uL0(t1, x + 16, y + 16, z + 16)) + 1.07916777e-8F*(-uL0(t0, x + 8, y + 16, z + 16) - uL0(t0, x + 16, y + 8, z + 16) - uL0(t0, x + 16, y + 16, z + 8) - uL0(t0, x + 16, y + 16, z + 24) - uL0(t0, x + 16, y + 24, z + 16) - uL0(t0, x + 24, y + 16, z + 16)) + 2.25524041e-7F*(uL0(t0, x + 9, y + 16, z + 16) + uL0(t0, x + 16, y + 9, z + 16) + uL0(t0, x + 16, y + 16, z + 9) + uL0(t0, x + 16, y + 16, z + 23) + uL0(t0, x + 16, y + 23, z + 16) + uL0(t0, x + 23, y + 16, z + 16)) + 2.30222458e-6F*(-uL0(t0, x + 10, y + 16, z + 16) - uL0(t0, x + 16, y + 10, z + 16) - uL0(t0, x + 16, y + 16, z + 10) - uL0(t0, x + 16, y + 16, z + 22) - uL0(t0, x + 16, y + 22, z + 16) - uL0(t0, x + 22, y + 16, z + 16)) + 1.54709492e-5F*(uL0(t0, x + 11, y + 16, z + 16) + uL0(t0, x + 16, y + 11, z + 16) + uL0(t0, x + 16, y + 16, z + 11) + uL0(t0, x + 16, y + 16, z + 21) + uL0(t0, x + 16, y + 21, z + 16) + uL0(t0, x + 21, y + 16, z + 16)) + 7.85634138e-5F*(-uL0(t0, x + 12, y + 16, z + 16) - uL0(t0, x + 16, y + 12, z + 16) - uL0(t0, x + 16, y + 16, z + 12) - uL0(t0, x + 16, y + 16, z + 20) - uL0(t0, x + 16, y + 20, z + 16) - uL0(t0, x + 20, y + 16, z + 16)) + 3.35203899e-4F*(uL0(t0, x + 13, y + 16, z + 16) + uL0(t0, x + 16, y + 13, z + 16) + uL0(t0, x + 16, y + 16, z + 13) + uL0(t0, x + 16, y + 16, z + 19) + uL0(t0, x + 16, y + 19, z + 16) + uL0(t0, x + 19, y + 16, z + 16)) + 1.38271608e-3F*(-uL0(t0, x + 14, y + 16, z + 16) - uL0(t0, x + 16, y + 14, z + 16) - uL0(t0, x + 16, y + 16, z + 14) - uL0(t0, x + 16, y + 16, z + 18) - uL0(t0, x + 16, y + 18, z + 16) - uL0(t0, x + 18, y + 16, z + 16)) + 7.90123476e-3F*(uL0(t0, x + 15, y + 16, z + 16) + uL0(t0, x + 16, y + 15, z + 16) + uL0(t0, x + 16, y + 16, z + 15) + uL0(t0, x + 16, y + 16, z + 17) + uL0(t0, x + 16, y + 17, z + 16) + uL0(t0, x + 17, y + 16, z + 16)) - 4.07312557e-2F*uL0(t0, x + 16, y + 16, z + 16))/(r8*r10 + r9*dampL0(x + 1, y + 1, z + 1));
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
