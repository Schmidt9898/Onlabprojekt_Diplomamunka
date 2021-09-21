
#include <stdio.h>

#include <stdlib.h>

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

int main(int argc, char ** argv) {


//printf("took %f GB memory needed.\n", (float)total_memory_needed / 1000000000);

if(argc<3){
  printf("Not enough parameter!\n");
  return -1;
}



FILE* valid_file = fopen(argv[1],"r");
FILE* test_file = fopen(argv[2],"r");
if(valid_file==NULL || test_file == NULL)
{
  printf("File was not found.\n");
  return -1; 
}
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
printf("Sizes equal.\n");



char * valid_bytes,*test_bytes;
valid_bytes=malloc(valid_size);
test_bytes= malloc(test_size);

fread(valid_bytes,1,valid_size,valid_file);
fread(test_bytes,1,test_size,test_file);

size_t error_count=0;

printf("Testing validating bytes:-----------------------------------------------------------------------------------------v\n");
printf("                        :=");

int darab,perc=0;
darab=valid_size/100;
for(size_t i = 0 ;i< valid_size;i++)
{
  if(i>perc){
    perc+=darab;
    printf("=");
    fflush(stdout);
  }

    if(valid_bytes[i]!=test_bytes[i])
        error_count+=1;
}
printf("\n");
if(error_count>0)
    printf("Test FAILED. err %lu\n",error_count);
else
    printf("Test PASSED. err %lu\n",error_count);


fclose(valid_file);
fclose(test_file);

  //Timer_print2file("viscoacoustic_gpu_v100.meres.txt");

  return 0;

}

