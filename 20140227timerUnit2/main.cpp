#include "timer_unit.h"
#include <stdio.h>
#include <time.h>

class CTimer: public CTimerObject
{
public:
    CTimer()
    {
        CTimerUnit::Instance().AddTimerObject(300, this);
    }
    void TimerNotify()
    {
        printf("[%d] Hello Timer!\n", (int)time(NULL));
        CTimerUnit::Instance().AddTimerObject(500, this);
    }
};

int main()
{
    CTimer t;
    while(1)
    {
        //5ms
        struct timeval timeout={0,5000};
        select(1,NULL,NULL,NULL,&timeout);
        CTimerUnit::Instance().CheckExpired();
    }

    return 0;
}
