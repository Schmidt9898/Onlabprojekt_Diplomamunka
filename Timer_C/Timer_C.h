#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

typedef struct Timer_C Timer_C();
//clock_t begin,end;
double Totaltime=0;
int Sum_Timers=0;
struct Timer_C * Timer_root=NULL;
//bool Timer_Filemode=true;



double op_timers_core() {

struct timeval t;

 gettimeofday(&t, (struct timezone *)0);
return t.tv_sec + t.tv_usec * 1.0e-6;
}

struct Timer_C
{
    /* data */
    struct Timer_C* parent;//=NULL;
    double start,end;
    char name[20];
    /*Timer_C start(){
        start=op_timers_core();
        parent=Timer_root;
        Timer_root=this;
        
    }
    ~Timer_C()
    {
        double end=op_timers_core();
        end-=start;
        printf("Time took: %f sec.\n",end);
        //if(parent!=0)
        Timer_root=parent;
    }*/
};


void Section_Start(char* name)
{
        //printf("%d\n",sizeof(struct Timer_C));
        struct Timer_C * timer= malloc(sizeof(struct Timer_C));
        printf("%s started->\n",name);
        strcpy(timer->name,name);
        //printf("%s\n",timer->name);
        timer->start=op_timers_core();
        timer->parent=Timer_root;
        Timer_root=timer;
}
void Section_End()
{
    if(Timer_root==NULL)
        return;
    Timer_root->end=op_timers_core();
    Timer_root->end-=Timer_root->start;
    printf("%s took: %f sec.\n",Timer_root->name,Timer_root->end);
    struct Timer_C* t=Timer_root;
    Timer_root=Timer_root->parent;
    free(t);
}

/*
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
    printf("Time spent %f .\n",time_spent);
}
*/
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