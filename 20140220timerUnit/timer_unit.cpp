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
    if (m_tobjListMap.find(timeoutMs) == m_tobjListMap.end())
    {
        list<CTimerObject*> tmpl;
        m_tobjListMap[timeoutMs] = tmpl;
    }
    m_tobjListMap[timeoutMs].push_back(obj);
    return true;
}

int CTimerUnit::CheckExpired(int64_t now)
{
    int ret = 0;
    if(now==0)
    {
        now = GET_TIMESTAMP();
    }
    list<CTimerObject*> needNotify;
    for (map<int, list<CTimerObject*> >::iterator it=m_tobjListMap.begin(); it!=m_tobjListMap.end(); )
    {
        if ((it->second).empty())
        {
            m_tobjListMap.erase(it++);
        }
        else
        {
            list<CTimerObject*>* timerList = &it->second;
            for (list<CTimerObject*>::iterator lit=timerList->begin(); lit!=timerList->end(); )
            {
                if ((*lit)->GetExpireTime()<=now)
                {
                    needNotify.push_back(*lit);
                    timerList->erase(lit++);
                }
                else
                {
                    lit++;
                }
            }
            it++;
        }
    }
    foreach(needNotify, it)
    {
        if ((*it)->GetExpireTime()!=-1)
        {
            (*it)->TimerNotify();
            ret++;
        }
    }

    needNotify.clear();
    return ret;
}


