#include "poller_unit.h"
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

CPollerObject::CPollerObject(CPollerUnit *pollerUnit, int netFd) : m_pollerUnit(pollerUnit), m_oldEvent(0)
{
    bzero(&m_event, sizeof(struct epoll_event));
    m_event.data.fd = netFd;
}

void CPollerUnit::Init(int maxPollers)
{
    m_maxPollers = maxPollers;
    if ((m_epFd = epoll_create(m_maxPollers)) == -1)
        perror("epoll_create error");
    if ((m_events = (struct epoll_event*)calloc(m_maxPollers, sizeof(struct epoll_event))) == NULL)
        perror("calloc error");
}

CPollerUnit::~CPollerUnit()
{
    free(m_events);
    close(m_epFd);
}

int CPollerUnit::AddPollerObj(CPollerObject *obj)
{
    if (obj->GetNetFd() == -1) return -1;
    int flag = fcntl(obj->GetNetFd(), F_GETFL);
    fcntl(obj->GetNetFd(), F_SETFL, O_NONBLOCK | flag);

    m_pollerObjs[obj->GetNetFd()] = obj;
    obj->UpdateOldEvent(obj->GetEventPointer()->events);
    return epoll_ctl(m_epFd, EPOLL_CTL_ADD, obj->GetNetFd(), obj->GetEventPointer());
}
int CPollerUnit::DelPollerObj(CPollerObject *obj)
{
    m_pollerObjs.erase(obj->GetNetFd());
    return epoll_ctl(m_epFd, EPOLL_CTL_DEL, obj->GetNetFd(), obj->GetEventPointer());

}

int CPollerUnit::ModPollerObj(CPollerObject *obj)
{
	int ret = 0;
	if (obj->GetOldEvent() != obj->GetEventPointer()->events)
	{
		obj->UpdateOldEvent(obj->GetEventPointer()->events);
		ret = epoll_ctl(m_epFd, EPOLL_CTL_MOD, obj->GetNetFd(), obj->GetEventPointer());
	}
	return ret;
}

void CPollerUnit::WaitAndProcess(int ms)
{
    int numEvents = epoll_wait (m_epFd, m_events, m_maxPollers, ms);
    for(int i=0; i<numEvents; ++i)
    {
        if(m_events[i].events & EPOLLIN)
            m_pollerObjs[m_events[i].data.fd]->InputNotify();
        if(m_events[i].events & EPOLLOUT)
            m_pollerObjs[m_events[i].data.fd]->OutputNotify();
        if(m_events[i].events & EPOLLHUP)
            m_pollerObjs[m_events[i].data.fd]->HangupNotify();
        ModPollerObj(m_pollerObjs[m_events[i].data.fd]);
    }

}
