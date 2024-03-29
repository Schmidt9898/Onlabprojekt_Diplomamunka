#ifndef STOPPER_H
#define STOPPER_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "FileWriterStream.h"
/*

typedef ushort bool;
#define true 1;
#define false 0;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;// it thoes not free the original string
}
*/
// individual stopper 
typedef struct Stopper Stopper();
//clock_t begin,end;
//globall variables
double Totaltime=0;
int Sum_Stopper=0;
// Stoppers follow LIFO structure
struct Stopper * Stopper_root=NULL; //root of the timers

bool Stopper_Filemode=false; // false write to console true write to filename file
char* stopper_str_buffer=NULL;// buffer for the strings if we write to file

bool Stopper_Startmode=true;

//get the curent time 
//also it does magic because i dont remember what it does actualy
double op_timer_core() {
struct timeval t;
gettimeofday(&t, (struct timezone *)0);
return t.tv_sec + t.tv_usec * 1.0e-6;
}

// proper definition of the stopper
struct Stopper
{
    /* data */
    struct Stopper* parent; //am i a child
    double start,end;       //preaty self explain
    char name[256];          //20 caracter for a name is enought //update it is few week later and 256 because turns out it isn't enought, this took like 3 painfull hours to find out.

};


void Spawn_stopper(char* name)
{
        struct Stopper * stopper= malloc(sizeof(struct Stopper));
        if(!Stopper_Filemode && Stopper_Startmode) //console mode
            printf("%s ID:%d started->\n",name,Sum_Stopper);
            else
            {
                //pass
            }
        strcpy(stopper->name,name);
        stopper->start=op_timer_core();
        stopper->parent=Stopper_root;
        Stopper_root=stopper;
        Sum_Stopper++;
}
double Kill_stopper()
{
    double ret=0;
    if(Stopper_root==NULL)
        return ret;
    Stopper_root->end=op_timer_core();
    ret=Stopper_root->end-=Stopper_root->start;
    if(!Stopper_Filemode)//console 
        printf("%s took: %f sec.\n",Stopper_root->name,Stopper_root->end);
    else {//file mode
        char output[64];// 64 comes from the air,... i gues
        snprintf(output, 64, "%s:%f\n",Stopper_root->name,Stopper_root->end);
        char* temp_del=stopper_str_buffer;
        stopper_str_buffer=concat(stopper_str_buffer,output);
        free(temp_del);

    }
    struct Stopper* t=Stopper_root;//free the stopper
    Stopper_root=Stopper_root->parent;
    free(t);
    return ret;

}


void Stopper_print2file(const char * filename)
{
    FILE* file = fopen(filename, "a" ); // this will append the file
    //printf("%s",stopper_str_buffer);
    fputs(stopper_str_buffer, file);
    printf("%lu",sizeof(stopper_str_buffer));
    fclose(file);
}


#endif // TIMER_H
