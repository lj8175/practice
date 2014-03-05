/*
 * main.cpp
 *
 *  Created on: 2014-3-4
 *      Author: joelliu
 */

#include "poller_unit.h"
#include "tcp_listener.h"
#include "tcpconn_handler.h"


int main()
{
    CPollerUnit::Instance().Init(1000);
    CTcpListener *tl = new CTcpListener(8175, &CPollerUnit::Instance());
    CPollerUnit::Instance().AddPollerObj(tl);
    for(;;)
    {
        CPollerUnit::Instance().WaitAndProcess(50);
    }

    return 0;
}


