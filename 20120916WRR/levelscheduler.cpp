#include <float.h>
#include <math.h>
#include "levelscheduler.h"
#include "arrays.h"

LevelScheduler::LevelScheduler() : m_alloc_times(NULL), m_can_alloc(NULL), m_size(0), m_min_index(0), m_all(0.0)
{
}

int LevelScheduler::saveServer(int *arr, size_t size)
{
    m_size = size;
    m_alloc_times = new int[size]();
    m_can_alloc = new int[size];
    array_copy(m_can_alloc, size, arr, size);
    m_min_index = 0;
}


int LevelScheduler::allocServer()
{
    double square_min = DBL_MAX;
    for(int i = 0; i < m_size; ++i)
    {
        double all_inner = m_all + 1.0/m_can_alloc[i];
        m_alloc_times[i]++;
        double square = 0.0;
        for(int j=0; j < m_size; j++)
        {
            square +=pow((1.0*m_alloc_times[j]/m_can_alloc[j] - all_inner/m_size), 2);
        }
        m_alloc_times[i]--;
        if(square < square_min)
        {
            m_min_index = i;
            square_min = square;
        }
    }
    m_all += 1.0/m_can_alloc[m_min_index];
    ++m_alloc_times[m_min_index];
    return m_min_index;
}

LevelScheduler::~LevelScheduler()
{
    delete[] m_alloc_times;
    delete[] m_can_alloc;
}
