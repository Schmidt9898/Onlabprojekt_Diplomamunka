#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdio.h>
clock_t begin,end;

double Total_Time=0;


void Timer_Start()
{
     begin = clock();
}
void Timer_Stop(){
    end = clock();
}
//void Timer_reset(){};
void Timer_Print()
{
    end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent %f .",time_spent);
}
/*
clock_t begin = clock();



clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;


*/


/*
//extern "C" {


//extern bool Timer_Filemode;
//extern std::string Timer_filename;

void Set_Timer_filemode(int istofile);
void Set_Timer_filename(char* filename);

void Timer_Print_all();//print all information
void Timer_Reset();//reset timer counter
float Timer_Get_Total_Time();//get the total time of timercounter

void Write_to_file(char* str,char* filename,int  append);

void Timer_push(char* name);
void Timer_pop();


//}





*/
#endif // TIMER_H