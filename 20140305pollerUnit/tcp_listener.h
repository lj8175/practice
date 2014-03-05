/*
 * tcp_listener.h
 *
 *  Created on: 2014-3-4
 *      Author: joelliu
 */

#ifndef TCP_LISTENER_H_
#define TCP_LISTENER_H_

#include "poller_unit.h"

class CTcpListener : public CPollerObject
{
public:
    CTcpListener(int port, CPollerUnit *pollerUnit);
    int InputNotify();
    int OutputNotify(){return 0;}
    int HangupNotify(){return 0;}

private:
    int m_servPort;
};


#endif /* TCP_LISTENER_H_ */
