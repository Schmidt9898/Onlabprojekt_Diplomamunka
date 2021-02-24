#ifndef TIMER_H
#define TIMER_H
#include <string>
#include <chrono>
void Timer_Print_all();

class Timer
{

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
        std::chrono::duration<float> duration;
    public:
        std::string msg="Duration:";

        Timer();
        Timer(std::string msg_);
        virtual ~Timer();

        void Start();
        void Stop();
        void Print();


    private:
};

#endif // TIMER_H
