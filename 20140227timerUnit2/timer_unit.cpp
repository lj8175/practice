// =====================================================================================
// 
//       Filename:  timer_unit.cpp
// 
//    Description:  定时器控制单元
// 
// =====================================================================================

#include "timer_unit.h"
#include <time.h>
#include <sys/time.h>

bool CTimerUnit::AddTimerObject(int timeoutMs, CTimerObject* obj)
{
    if (timeoutMs <= 0 || NULL == obj)
    {
        return false;
    }
    obj->SetExpireTimeMs(timeoutMs);
    m_tobjQueue.push(obj);
    return true;
}

int CTimerUnit::CheckExpired(int64_t now)
{
    int ret = 0;
    if(now==0)
    {
        now = GET_TIMESTAMP();
    }
    while (!m_tobjQueue.empty())
    {
        if (m_tobjQueue.top()->GetExpireTime() <= now)
        {
            m_tobjQueue.top()->TimerNotify();
            m_tobjQueue.pop();
            ret++;
        }
        else
        {
            break;
        }
    }

    return ret;
}


