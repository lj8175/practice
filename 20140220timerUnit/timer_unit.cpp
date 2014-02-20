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
    m_tobjList.push_back(obj);
    return true;
}

int CTimerUnit::CheckExpired(int64_t now)
{
    int ret = 0;
    if(now==0)
    {
        now = GET_TIMESTAMP();
    }
    foreach(m_tobjList, it)
    {
        if ((*it)->GetExpireTime()<=now)
        {
            (*it)->TimerNotify();
            ret++;
        }
    }

    return ret;
}


