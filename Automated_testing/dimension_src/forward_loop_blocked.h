#ifdef FORBLOCKED
#pragma omp target teams distribute collapse(3) thread_limit(L_THREAD_LIMIT)
	for(int Bx = x_m; Bx <= x_M; Bx += blocksize_x)
		for (int By = y_m; By <= y_M; By += blocksize_y)
			for (int Bz = z_m; Bz <= z_M; Bz += blocksize_z)
			{
			#pragma omp parallel for collapse(3)
			for (int x = Bx; x < Bx + blocksize_x; x++)
				for (int y = By; y < By + blocksize_y; y++)
					for (int z = Bz; z < Bz + blocksize_z; z++)
					{
						if (x <= x_M && y <= y_M && z <= z_M){
#endif