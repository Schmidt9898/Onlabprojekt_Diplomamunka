#ifndef TIMER_H
#define TIMER_H
#include <string>
#include <chrono>
//#include <iostream>
/**
Ez a Timer osztály.

Igy használd

{
Timer t1("t1");
    {
    Timer t2("t2");
    }
}

Soha ne csinálj ilyet.
Timer t1("t1");
Timer t2("t2");


Mert nem tudom mi történne és tuti nem jó dolog.

*/



#ifdef __cplusplus
extern "C" {
#endif

extern bool Timer_Filemode;
extern std::string Timer_filename;
void Timer_Print_all();

class Timer
{

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
        std::chrono::duration<float> duration;
        Timer * parent = nullptr;
    public:
        std::string msg="Timer";
        int root_num=0;

        Timer();
        Timer(std::string msg_);
        virtual ~Timer();
    protected:
        void Start();
        void Stop();
        void Print();
        void Print_start();


    private:
};

#ifdef __cplusplus
}
#endif





#endif // TIMER_H
