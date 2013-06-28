#ifndef __RANDOMSCHEDULER_H
#define __RANDOMSCHEDULER_H

#include <cstddef>

class RandomScheduler
{
public:
    RandomScheduler();
    virtual ~RandomScheduler();
    int saveServer(int *arr, size_t size);
    int allocServer();

private:
    int *m_scount;
    int *m_random;
    int m_size;

};

inline RandomScheduler::RandomScheduler() : m_scount(NULL), m_random(NULL), m_size(0)
{}

#endif
