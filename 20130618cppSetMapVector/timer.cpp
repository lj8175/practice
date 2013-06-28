#include "timer.h"
#include "stdlib.h"

Timer::Timer()
{
    setBegin();
}

int Timer::setBegin()
{
    return gettimeofday(&m_begin, NULL);
}

int Timer::setEnd()
{
    return gettimeofday(&m_end, NULL);
}

int Timer::timeDiff()
{
    setEnd();
    int time_used = ((m_end.tv_sec - m_begin.tv_sec) * 1000000 \
                    +(m_end.tv_usec - m_begin.tv_usec));
    setBegin();
    return time_used;
}
