







/*#include "Timer.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <vector>

class Timer;


float Totaltime=0;
int Sum_Timers=0;
Timer * root=nullptr;
bool Timer_Filemode=true;
std::stringstream Timer_buff;
std::string Timer_filename="time.txt";

std::vector<Timer*> timer_que;

//Timer::Timer(){
    //Sum_Timers++;
    //start = std::chrono::high_resolution_clock::now();};
//Timer::Timer(std::string msg_):msg(msg_){};


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





Timer::Timer(std::string msg_):msg(msg_)
{
    if(root==nullptr) root=this;
    else {
            parent=root;
            root=this;
            root_num=parent->root_num+1;
            //std::cout<<root_num;
    }
    Sum_Timers++;
    Print_start();
    start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    Stop();
    Print();
    root=parent;
}


void Timer::Start(){
 start = std::chrono::high_resolution_clock::now();

};
void Timer::Stop(){
    end=std::chrono::high_resolution_clock::now();
    duration = end-start;

    Totaltime+=(parent==nullptr)?duration.count():0;
};
void Timer::Print(){
    if(Timer_Filemode)
    {
    Timer_buff<<msg.c_str()<<":"<<duration.count()<<"\n";
    }else
    {
    for(int i=0;i<root_num;i++)
        std::cout<<"----";
    std::cout<<msg.c_str()<<" took "<<duration.count()<<" sec.\n";
    }


};
void Timer::Print_start(){
    if(Timer_Filemode)
        return;

    for(int i=0;i<root_num;i++)
        std::cout<<"    ";
    std::cout<<msg.c_str()<<"->\n";
};

extern "C" void Timer_Print_all(){
if(Timer_Filemode)
{
    Timer_buff<<"Total time:"<<Totaltime<<"\nEND\n";
    std::ofstream outfile;

    outfile.open(Timer_filename, std::ios_base::app);
    outfile << Timer_buff.str();
    outfile.close();

}
    std::cout<<"Total time: "<<Totaltime<<" sec.\n";
    std::cout<<"Number of timers: "<<Sum_Timers<<".\n";
};


extern "C" void Timer_Reset(){
    Sum_Timers=0;
    Totaltime=0;
};

extern "C" void Write_to_file(char* str,char* filename,int append)
{
    if(append==1)
    {
    std::ofstream outfile;
    outfile.open(filename, std::ios_base::app);
    outfile << str;
    outfile.close();
    }else{
    std::ofstream outfile;
    outfile.open(filename);
    outfile << str;
    outfile.close();

    }
}

extern "C" float Timer_Get_Total_Time()
{
    return Totaltime;
}


extern "C" void Set_Timer_filemode(int istofile)
{
    Timer_Filemode=istofile==1;
}
extern "C" void Set_Timer_filename(char* filename)
{
    Timer_filename=filename;
}
extern "C" void Timer_push(char* name)
{
    Timer* t = new Timer(name);
    timer_que.push_back(t);
}
extern "C" void Timer_pop()
{
    if(timer_que.size()<=0)
        return
    delete timer_que[timer_que.size()-1];
    timer_que.pop_back();

}*/





