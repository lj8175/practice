#ifndef POLLER_UNIT_H_
#define POLLER_UNIT_H_

#include <sys/epoll.h>
#include <map>
#include "singleton.h"

using std::map;

class CPollerObject;

class CPollerUnit {
public:
    friend class singleton_default<CPollerUnit>;
    static CPollerUnit& Instance() { return SINGLETON_REF(CPollerUnit); }
    void Init(int maxPollers);
    virtual ~CPollerUnit();

    int AddPollerObj(CPollerObject *obj);
    int DelPollerObj(CPollerObject *obj);
    int ModPollerObj(CPollerObject *obj);

    void WaitAndProcess(int ms);

private:
    CPollerUnit(){};
    int m_epFd;
    int m_maxPollers;
    struct epoll_event *m_events;
    map<int, CPollerObject*>  m_pollerObjs;
};

class CPollerObject {
public:
	CPollerObject (CPollerUnit *pollerUnit, int netFd=-1);
	virtual ~CPollerObject (){};

	virtual int InputNotify() = 0;
	virtual int OutputNotify() = 0;
	virtual int HangupNotify() = 0;
	
	void EnableInput(){
	    m_event.events |= EPOLLIN;
	}
    void DisableInput() {
        m_event.events &= ~EPOLLIN;
    }
	void EnableOutput() {
	    m_event.events |= EPOLLOUT;
	}
	void DisableOutput() {
	    m_event.events &= ~EPOLLOUT;
	}
	void EnableHangup(){
	    m_event.events |= EPOLLHUP;
	}
	void DisableHangup(){
	    m_event.events &= ~EPOLLHUP;
	}

	int AttachPollerUnit(){
	    return m_pollerUnit->AddPollerObj(this);
	}

	int DetachPollerUnit(){
	    return m_pollerUnit->DelPollerObj(this);
	}

	int ApplyEvents(){
		return m_pollerUnit->ModPollerObj(this);
	}

	void SetNetFd(int netFd){
	    m_event.data.fd = netFd;
	}
	int GetNetFd(){
	    return m_event.data.fd;
	}
	struct epoll_event *GetEventPointer(){
	    return &m_event;
	}
	uint32_t GetOldEvent(){
		return m_oldEvent;
	}
	void UpdateOldEvent(uint32_t event){
		m_oldEvent = event;
	}
protected:
	CPollerUnit *m_pollerUnit;
private:
	struct epoll_event m_event;
	uint32_t m_oldEvent;
};





#endif /*POLLER_UNIT_H_*/
