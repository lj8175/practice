#ifndef TIMER_UNIT_H_
#define TIMER_UNIT_H_

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <list>
#include <map>
#include "singleton.h"

using std::list;
using std::map;

static inline int64_t GET_TIMESTAMP(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

#ifndef foreach
#define foreach(container,it) \
    for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)
#endif


#define TIMESTAMP_PRECISION 1000ULL

/*
 * @brief: 定时的任务：实现定时回调
 */
class CTimerObject
{
public:
    CTimerObject() : m_exp(0) {}
    virtual ~CTimerObject(void) {}

    virtual void TimerNotify(void) = 0; //  timer到期时回调该函数

    void SetExpireTimeMs(int64_t exp)
    {
        m_exp = exp*1000 + GET_TIMESTAMP();
    }
    int64_t GetExpireTime() { return m_exp; }
    void DisableTimer() { m_exp = -1; }
private:
    int64_t m_exp;
};

class CTimerUnit
{
public:
    friend class singleton_default<CTimerUnit>;
    static CTimerUnit& Instance() { return SINGLETON_REF(CTimerUnit); }
    ~CTimerUnit(void){}

    bool AddTimerObject(int timeoutMs, CTimerObject* obj);
    int CheckExpired(int64_t now = 0);

private:
    map<int, list<CTimerObject*>*> m_tobjListMap;
    CTimerUnit(void){}

};

#endif /* TIMER_UNIT_H_ */
