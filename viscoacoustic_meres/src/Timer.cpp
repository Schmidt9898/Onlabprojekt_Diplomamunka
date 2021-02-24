#include "Timer.h"
#include <iostream>
#include <sstream>

float Totaltime=0;
int Sum_Timers=0;

Timer::Timer(){Sum_Timers++;};
//Timer::Timer(std::string msg_):msg(msg_){};

Timer::Timer(std::string msg_):msg(msg_)
{
    Sum_Timers++;
    start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    Stop();
    Print();
}


void Timer::Start(){
 start = std::chrono::high_resolution_clock::now();

};
void Timer::Stop(){
    end=std::chrono::high_resolution_clock::now();
    duration = end-start;
    Totaltime+=duration.count();
};
void Timer::Print(){
    //std::stringstream ss;
    //ss<<msg.c_str()<<duration.count()<<" sec."
    std::cout<<msg.c_str()<<duration.count()<<" sec.\n";
    //printf(msg.c_str()<<": %f sec.\n",duration.count());
    //printf(msg+": %f sec.\n",duration.count());
};
void Timer_Print_all(){

    std::cout<<"Total time: "<<Totaltime<<" sec.\n";
    std::cout<<"Number of timers: "<<Sum_Timers<<".\n";

};
