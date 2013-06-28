#include "wrrscheduler.h"
#include "arrays.h"

int WRRScheduler::saveServer(int *arr, size_t size)
{
    m_server_size = size;
    m_server_arr = new int[size];
    for (int i = 0; i < size; i++)
    {
        m_server_arr[i] = arr[i];
    }
}

int WRRScheduler::allocServer()
{
    while(true)
    {
        m_index = (m_index + 1) % m_server_size;
        if (m_index == 0)
        {
            int ag = array_gcd(m_server_arr, m_server_size);
            m_cw -= (ag==0) ? 1 : ag;
            if (m_cw <= 0)
            {
                m_cw = array_max(m_server_arr, m_server_size);
                if (m_cw == 0)
                {
                    return -1;
                }
            }
        }

        if (m_server_arr[m_index] >= m_cw)
        {
            --m_server_arr[m_index];
            return m_index;
        }
    }
}
