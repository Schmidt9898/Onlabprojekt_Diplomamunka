#ifdef F800_16
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
#endif