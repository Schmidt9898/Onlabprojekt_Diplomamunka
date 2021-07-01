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
int Timer_Filemode=0;
char* Timer_buffer;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

double op_timers_core() {

struct timeval t;

 gettimeofday(&t, (struct timezone *)0);
return t.tv_sec + t.tv_usec * 1.0e-6;
}

struct Timer_C
{
    /* data */
    struct Timer_C* parent;
    double start,end;
    char name[20];

};


void Section_Start(char* name)
{

        struct Timer_C * timer= malloc(sizeof(struct Timer_C));
        if(Timer_Filemode==0)
            printf("%s started->\n",name);
        strcpy(timer->name,name);
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
    if(Timer_Filemode==0)
        printf("%s took: %f sec.\n",Timer_root->name,Timer_root->end);
    else {
        char output[64];
        snprintf(output, 64, "%s:%f\n",Timer_root->name,Timer_root->end);
        char* temp_del=Timer_buffer;
        Timer_buffer=concat(Timer_buffer,output);
        free(temp_del);

    }
    struct Timer_C* t=Timer_root;
    Timer_root=Timer_root->parent;
    free(t);
}
void Timer_print2file(const char * filename)
{
    FILE* file = fopen(filename, "a" );
    //printf("%s",Timer_buffer);
    fputs(Timer_buffer, file);
    fclose(file);
}


#endif // TIMER_H