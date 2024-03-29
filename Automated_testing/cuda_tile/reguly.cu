__global__ void cuda_section0(const int x_M, const int x_m, const int y_M, const int y_m, const int z_M, const int z_m, int t0, int t1, int t2, float *vp, float *u, float *damp)
{
	int z1 = threadIdx.x + blockIdx.x * blocksize_z * tilesize_z + z_m;
	int y1 = threadIdx.y + blockIdx.y * blocksize_y * tilesize_y + y_m;
	int x1 = threadIdx.z + blockIdx.z * blocksize_x * tilesize_x + x_m;
	#pragma unroll
	for (int i = 0; i < tilesize_x; i++)
		#pragma unroll
		for (int j = 0; j < tilesize_y; j++)
			#pragma unroll
			for (int k = 0; k < tilesize_z; k++)
			{
				int x = x1 + i*blocksize_x;
				int y = y1 + j*blocksize_y;
				int z = z1 + k*blocksize_z;
				if (x <= x_M && y <= y_M && z <= z_M)
				{
					float r10 = 1.0F / (vpL0(x + 16, y + 16, z + 16) * vpL0(x + 16, y + 16, z + 16));
					uL0(t2, x + 16, y + 16, z + 16) = (r9 * dampL0(x + 1, y + 1, z + 1) * uL0(t0, x + 16, y + 16, z + 16) + r10 * (-r8 * (-2.0F * uL0(t0, x + 16, y + 16, z + 16)) - r8 * uL0(t1, x + 16, y + 16, z + 16)) + 1.07916777e-8F * (-uL0(t0, x + 8, y + 16, z + 16) - uL0(t0, x + 16, y + 8, z + 16) - uL0(t0, x + 16, y + 16, z + 8) - uL0(t0, x + 16, y + 16, z + 24) - uL0(t0, x + 16, y + 24, z + 16) - uL0(t0, x + 24, y + 16, z + 16)) + 2.25524041e-7F * (uL0(t0, x + 9, y + 16, z + 16) + uL0(t0, x + 16, y + 9, z + 16) + uL0(t0, x + 16, y + 16, z + 9) + uL0(t0, x + 16, y + 16, z + 23) + uL0(t0, x + 16, y + 23, z + 16) + uL0(t0, x + 23, y + 16, z + 16)) + 2.30222458e-6F * (-uL0(t0, x + 10, y + 16, z + 16) - uL0(t0, x + 16, y + 10, z + 16) - uL0(t0, x + 16, y + 16, z + 10) - uL0(t0, x + 16, y + 16, z + 22) - uL0(t0, x + 16, y + 22, z + 16) - uL0(t0, x + 22, y + 16, z + 16)) + 1.54709492e-5F * (uL0(t0, x + 11, y + 16, z + 16) + uL0(t0, x + 16, y + 11, z + 16) + uL0(t0, x + 16, y + 16, z + 11) + uL0(t0, x + 16, y + 16, z + 21) + uL0(t0, x + 16, y + 21, z + 16) + uL0(t0, x + 21, y + 16, z + 16)) + 7.85634138e-5F * (-uL0(t0, x + 12, y + 16, z + 16) - uL0(t0, x + 16, y + 12, z + 16) - uL0(t0, x + 16, y + 16, z + 12) - uL0(t0, x + 16, y + 16, z + 20) - uL0(t0, x + 16, y + 20, z + 16) - uL0(t0, x + 20, y + 16, z + 16)) + 3.35203899e-4F * (uL0(t0, x + 13, y + 16, z + 16) + uL0(t0, x + 16, y + 13, z + 16) + uL0(t0, x + 16, y + 16, z + 13) + uL0(t0, x + 16, y + 16, z + 19) + uL0(t0, x + 16, y + 19, z + 16) + uL0(t0, x + 19, y + 16, z + 16)) + 1.38271608e-3F * (-uL0(t0, x + 14, y + 16, z + 16) - uL0(t0, x + 16, y + 14, z + 16) - uL0(t0, x + 16, y + 16, z + 14) - uL0(t0, x + 16, y + 16, z + 18) - uL0(t0, x + 16, y + 18, z + 16) - uL0(t0, x + 18, y + 16, z + 16)) + 7.90123476e-3F * (uL0(t0, x + 15, y + 16, z + 16) + uL0(t0, x + 16, y + 15, z + 16) + uL0(t0, x + 16, y + 16, z + 15) + uL0(t0, x + 16, y + 16, z + 17) + uL0(t0, x + 16, y + 17, z + 16) + uL0(t0, x + 17, y + 16, z + 16)) - 4.07312557e-2F * uL0(t0, x + 16, y + 16, z + 16)) / (r8 * r10 + r9 * dampL0(x + 1, y + 1, z + 1));
				}
			}
}