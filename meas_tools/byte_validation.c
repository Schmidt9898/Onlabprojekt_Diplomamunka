
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX(x,y) (((x) >= (y)) ? (x) : (y))
#define MIN(x,y) (((x) <= (y)) ? (x) : (y))


/*

size_t total_memory_needed=0;


  a.data =  malloc(elemsize*size0*size1*size2*size3);//new char[elemsize*size0*size1*size2*size3]; // 340*340*340
  total_memory_needed+=elemsize*size0*size1*size2*size3;
  size_t arrsize=size0*size1*size2*size3;
  Read_from_file_savepos("Startfile.dataobj",(char*)a.data,arrsize*sizeof(float));
  for(size_t i=0;i<arrsize;i++)
  {
    if(a.data[i]!=1){
      printf("dataobj %f\n",a.data[i]);
      break;
    }
    //a.data[i]=1;
  }

void delete_dataobj(dataobj a)
{
//Write_byte_to_file("Startfile.dataobj",(char*)a.data,a.size[0]*a.size[1]*a.size[2]*a.size[3]*sizeof(float));


free(a.data);
free(a.size);

}
*/


//TODO add -h parameter and help if empty
int main(int argc, char ** argv) {


//printf("took %f GB memory needed.\n", (float)total_memory_needed / 1000000000);

if(argc<3){
  printf("Not enough parameter!\n");
  return -1;
}

int silentmode= (argc>=4) ? strcmp(argv[3],"get") : 1;

FILE* valid_file = fopen(argv[1],"r");
FILE* test_file = fopen(argv[2],"r");
if(valid_file==NULL || test_file == NULL)
{
  printf("File was not found.\n");
  return -1; 
}

if(!silentmode)
	printf(" %s and %s exemaning...\n",argv[1],argv[2]);

size_t valid_size=0,test_size=0;


fseek(valid_file, 0, SEEK_END); // seek to end of file
valid_size = ftell(valid_file); // get current file pointer
fseek(valid_file, 0, SEEK_SET);

fseek(test_file, 0, SEEK_END); 
test_size = ftell(test_file);
fseek(test_file, 0, SEEK_SET);

if(valid_size!=test_size){
    printf("Size not equal!! %lu != %lu",valid_size,test_size);
    return -1;
}



char * valid_bytes,*test_bytes;
valid_bytes=malloc(valid_size);
test_bytes= malloc(test_size);

fread(valid_bytes,1,valid_size,valid_file);
fread(test_bytes,1,test_size,test_file);

float* valid_f = (float*)valid_bytes
	,* test_f = (float*)test_bytes;

size_t error_count=0;

valid_size/=sizeof(float);

if(!silentmode)
{


printf("Sizes equal. %lu\n",valid_size);
printf("Float numbe:  %lu\n",valid_size);
printf("Testing validating bytes:---------------------------------------------------------------------------------------------------v\n");
printf("                        :");
}


int darab,perc=0;
darab=valid_size/100;
/*
for(size_t i = 0 ;i< valid_size;i++)
{
  if(i>perc){
    perc+=darab;
    printf("=");
    fflush(stdout);
  }
	//TODO absulute value
	//devide with the grater absolute value

    if(valid_f[i]-test_f[i]>0.0001)
	{
        error_count+=1;
		if(silentmode==0)
			printf("%f - %f = %f\n",valid_f[i],test_f[i],valid_f[i]-test_f[i]);
	}
}
*/
	{
		//size_t arrsize = a.size[0] * a.size[1] * a.size[2] * a.size[3];
		//3,880,880,880
		//for (size_t t = 0; t < 3; t++)
			for (size_t i = 0; i < 880; i++)
				for (size_t j = 0; j < 880; j++)
					for (size_t k = 0; k < 880; k++)
					{
						size_t index = /*t * (880 * 880 * 880) +*/ i * (880 * 880) + j * (880) + k;
						//printf("index: %lu | t: %lu, x: %lu, y: %lu, z: %lu, \n",index, t,i,j,k);
						float valid = valid_f[index];
						float test = test_f[index];
						if(test == test)//Nan value filter
						//	continue;
						{
							if(MAX(valid,test) == 0)
							{
								continue; // in case of 0 devision 
							}
							float error = (valid - test) / MAX(valid,test);
							//printf("(valid - test) %f,%f\n",valid,test);

							error = fabsf(error); /// abs(valid>test? valid : test);
							//printf("error: %f - %f = %f | at: t: %lu, x: %lu, y: %lu, z: %lu, \n", valid, test, error, 0, i, j, k);
							//printf("error: %f\n",error);
							// FIXME a-b / max(a,b)

							if (error > 0.00001)
							{
								error_count += 1;
								if (silentmode == 0)
									printf("error: %f - %f = %f | at: x: %lu, y: %lu, z: %lu, \n", valid, test, error, i, j, k);
								// printf("%f - %f = %f\n", valid_f[i], test_f[i], valid_f[i] - test_f[i]);
							}
						}
					}
	}



if(error_count>0)
    printf("Test FAILED. err %lu\n",error_count);
else
    printf("Test PASSED. err %lu\n",error_count);


fclose(valid_file);
fclose(test_file);

free(valid_bytes);
free(test_bytes);


  //Timer_print2file("viscoacoustic_gpu_v100.meres.txt");

  return 0;

}

