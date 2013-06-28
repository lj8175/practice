
#include "stepscheduler.h"
#include "arrays.h"
#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

StepScheduler::StepScheduler() : m_step(NULL), m_loop(NULL), m_ssize(0), m_lsize(0), m_lindex(0), m_sflag(true), m_lflag(true), m_rflag(true)
{
}

int StepScheduler::saveServer(int *arr, size_t size)
{
    m_step = new STEP_ITEM[size];
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

    for(int i=0; i<size; i++)
    {
        m_step[i].step = (int)r*(arr[i]*1.0/arr_max);
        m_step[i].distance = 0;
        m_step[i].remain_call = arr[i];
        m_lsize += m_step[i].step;
    }
    m_loop = new int[m_lsize];

    m_ssize = size;
}

int StepScheduler::allocServer()
{
#ifdef DEBUG
    static int c[3]={0};
#endif
    if ( m_sflag )
    {
#ifdef DEBUG
        c[0]++;
#endif
        int max_index = 0;
        for ( int i = 0; i < m_ssize; ++i )
        {
            m_step[i].distance += m_step[i].step;
            if ( m_step[i].distance >= m_step[max_index].distance )
            {
                max_index = i;
            } 
        }
        m_step[max_index].distance -= m_lsize;
        --m_step[max_index].remain_call;
        m_loop[m_lindex++] = max_index;
        if( m_lindex >= m_lsize )
        {
            m_sflag = false;
        }
        return max_index;
    }
    else if( m_lflag )
    {
#ifdef DEBUG
        c[1]++;
#endif
        m_lindex %=  m_lsize;
        if( --m_step[m_loop[m_lindex]].remain_call <= 0 )
        {
            m_lflag = false;
        }
        return m_loop[m_lindex++];
    }
    else if( m_rflag )
    {
#ifdef DEBUG
        c[2]++;
        cout << c[0] << "\t" << c[1] << "\t" << c[2] << "\t" << m_lsize << endl;
#endif
        for( int i = 0; i < m_ssize; ++i)
        {
            m_lindex = (m_lindex + 1) % m_ssize;
            if( --m_step[m_lindex].remain_call >= 0)
            {
                return m_lindex;
            }
        }
        m_rflag = false;
    }
    return -1;
}


StepScheduler::~StepScheduler()
{
    delete[] m_step;
    delete[] m_loop;
}









