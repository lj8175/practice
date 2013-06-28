#include <sys/time.h>

class Timer
{
public:
    Timer();
    int setBegin();
    int setEnd();
    int timeDiff();

private:
    struct timeval m_begin;
    struct timeval m_end;
    struct timeval m_now;

};

