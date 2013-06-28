#include "arrays.h"
#include "randomscheduler.h"
#include <stdlib.h>
#include <time.h>

int RandomScheduler::saveServer(int *arr, size_t size)
{
    int arr_max = array_max(arr, size);
    int arr_min = array_min(arr, size);
    if (arr_min <= 0)
    {
        arr_min = 1;
    }
    int r = arr_max/arr_min;
    if (r < 10)
    {
        r = 10;
    }
    else if (r > 100)
    {
        r = 100;
    }

    m_scount = new int[size]();
    for (int i=0; i<size; i++)
    {
        m_scount[i] = (int)r*(arr[i]*1.0/arr_max);
        m_size += m_scount[i];
    }

    m_random = new int[m_size]();
    int index = 0;
    for (int i=0; i<size; ++i)
    {
        for (int j=0; j<m_scount[i]; ++j)
        {
            m_random[index++] = i;
        }
    }

    srand(time(NULL));
}


int RandomScheduler::allocServer()
{
    int index = random()%m_size;
    return m_random[index];
}

RandomScheduler::~RandomScheduler()
{
    delete[] m_random;
    delete[] m_scount;
}
