const size_t blocksize_x = 16;
const size_t blocksize_y = 4;
const size_t blocksize_z = 32;

//const int blocksize_x = XERTEK;
//const int blocksize_y = YERTEK;
//const int blocksize_z = ZERTEK;

int x_m=0;
int y_m=0;
int z_m=0;

int x_M=100;
int y_M=100;
int z_M=100;



const int Thread_limit=512;

#pragma omp target teams distribute collapse(3) thread_limit(512)
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
						//stuff here
					}
				}
		}









//this one is bad
#define MIN(a,b) (((a) < (b)) ? (a) : (b))