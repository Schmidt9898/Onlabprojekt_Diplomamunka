#ifndef STOPPER_H
#define STOPPER_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

typedef ushort bool;
#define true 1;
#define false 0;

// individual stopper 
typedef struct Stopper Stopper();
//clock_t begin,end;
//globall variables
double Totaltime=0;
int Sum_Stopper=0;
// Stoppers follow LIFO structure
struct Stopper * Stopper_root=NULL; //root of the timers

bool Stopper_Filemode=false; // false write to console true write to filename file
char* _buffer=NULL;// buffer for the strings if we write to file

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;// it thoes not free the original string
}

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
    char name[20];          //20 caracter for a name is enought

};


void Spawn_stopper(char* name)
{
        struct Stopper * stopper= malloc(sizeof(struct Stopper));
        if(!Stopper_Filemode) //console mode
            printf("%s ID:%d started->\n",name,Sum_Stopper);
        strcpy(stopper->name,name);
        stopper->start=op_timer_core();
        stopper->parent=Stopper_root;
        Stopper_root=stopper;
        Sum_Stopper++;
}
void Kill_stopper()
{
    if(Stopper_root==NULL)
        return;
    Stopper_root->end=op_timer_core();
    Stopper_root->end-=Stopper_root->start;
    if(!Stopper_Filemode)
        printf("%s took: %f sec.\n",Stopper_root->name,Stopper_root->end);
    else {
        char output[64];// 64 comes from the air,... i gues
        snprintf(output, 64, "%s:%f\n",Stopper_root->name,Stopper_root->end);
        char* temp_del=_buffer;
        _buffer=concat(_buffer,output);
        free(temp_del);

    }
    struct Stopper* t=Stopper_root;//free the stopper
    Stopper_root=Stopper_root->parent;
    free(t);
}
void Stopper_print2file(const char * filename)
{
    FILE* file = fopen(filename, "a" ); // this will append the file
    //printf("%s",_buffer);
    fputs(_buffer, file);
    printf("%lu",sizeof(_buffer));
    fclose(file);
}


#endif // TIMER_H
