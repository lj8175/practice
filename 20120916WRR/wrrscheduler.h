#ifndef __WRRSCHEDULER_H
#define __WRRSCHEDULER_H

#include <stddef.h>

class WRRScheduler
{
public:
    WRRScheduler();
    int saveServer(int *arr, size_t size);
    int allocServer();
private:
    int* m_server_arr;
    size_t m_server_size;
    int m_index;
    int m_cw;
};

inline WRRScheduler::WRRScheduler() : m_server_arr(NULL), m_server_size(0), m_index(-1), m_cw(0)
{
}

#endif
