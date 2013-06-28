
#include "lclient.h"
#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

LClient::LClient() : m_step(NULL), m_loop(NULL), m_lsize(0), m_lindex(0), m_sflag(true), m_lflag(true), m_rflag(true)
{
}

int LClient::saveServer()
{
    m_step = new STEP_ITEM[SERVER_COUNT];
    for(int i=0; i<SERVER_COUNT; i++)
    {
        m_step[i].step = 100;
        m_step[i].distance = 0;
        m_step[i].remain_call = 10000;
        m_lsize += m_step[i].step;
    }
    m_loop = new int[m_lsize];
}

int LClient::allocServer()
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
        for ( int i = 0; i < SERVER_COUNT; ++i )
        {
            m_step[i].distance += m_step[i].step;
            if ( m_step[i].distance > m_step[max_index].distance )
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
        for( int i = 0; i < SERVER_COUNT; ++i)
        {
            if( --m_step[i].remain_call >= 0)
            {
                return i;
            }
        }
        m_rflag = false;
    }
    return -1;
}


LClient::~LClient()
{
    delete[] m_step;
    delete[] m_loop;
}









