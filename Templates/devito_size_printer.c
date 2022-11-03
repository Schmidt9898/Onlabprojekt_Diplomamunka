

void printdata(char* name,struct dataobj *restrict data,int dim)
{
printf("dataobj ");
printf(name);
printf(" = create_data( %ld,%ld,%ld,%ld,sizeof(float));\n",data->size[0],(dim>=2)? data->size[1]:1,(dim>=3)?data->size[2]:1,(dim>=4)?data->size[3]:1);
};



int Forward(struct dataobj *restrict damp_vec, struct dataobj *restrict rec_vec, struct dataobj *restrict rec_coords_vec, struct dataobj *restrict src_vec, struct dataobj *restrict src_coords_vec, struct dataobj *restrict u_vec, struct dataobj *restrict vp_vec, const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m, const float dt, const float o_x, const float o_y, const float o_z, const int p_rec_M, const int p_rec_m, const int p_src_M, const int p_src_m, const int time_M, const int time_m, const int deviceid, const int devicerm, struct profiler * timers)
{

printdata("damp_vec",damp_vec,3);
printdata("rec_vec",rec_vec,2);
printdata("rec_coords_vec",rec_coords_vec,2);
printdata("src_vec",src_vec,2);
printdata("src_coords_vec",src_coords_vec,2);
printdata("u_vec",u_vec,4);
printdata("vp_vec",vp_vec,3);
printf("int x_M = %d;\n" ,x_M);
printf("int x_m = %d;\n" ,x_m);
printf("int y_M = %d;\n" ,y_M);
printf("int y_m = %d;\n" ,y_m);
printf("int z_M = %d;\n" ,z_M);
printf("int z_m = %d;\n" ,z_m);
printf("float dt = %f;\n" ,dt);
printf("float o_x = %f;\n" ,o_x);
printf("float o_y = %f;\n" ,o_y);
printf("float o_z = %f;\n" ,o_z);
printf("int p_rec_M = %d;\n" ,p_rec_M);
printf("int p_rec_m = %d;\n" ,p_rec_m);
printf("int p_src_M = %d;\n" ,p_src_M);
printf("int p_src_m = %d;\n" ,p_src_m);
printf("int time_M = %d;\n" ,time_M);
printf("int time_m = %d;\n" ,time_m);
printf("int deviceid = %d;\n" ,deviceid);
printf("int devicerm = %d;\n" ,devicerm);

