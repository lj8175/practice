/*
 * tcpconn_handler.cpp
 *
 *  Created on: 2014-3-4
 *      Author: joelliu
 */


#include "tcpconn_handler.h"
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include "ssl_connector.h"

extern CSslConnector *g_sc;

CTcpConnHandler::CTcpConnHandler(CPollerUnit *pollerUnit, int netFd) : CPollerObject(pollerUnit, netFd)
{
    EnableInput();
    DisableOutput();
    AttachPollerUnit();
}

CTcpConnHandler::~CTcpConnHandler(){}


int CTcpConnHandler::InputNotify()
{
    char recvBuf[1<<12];
    int32_t ret = recv(GetNetFd(), recvBuf, 1<<12, 0);
    if (-1 == ret)
    {
        if ((errno == EINTR) || (errno == EAGAIN) || (errno == EINPROGRESS))
        {
            return 0;
        }

        Close();
        return -1;
    }

    if (0 == ret)
    {
        Close();
    }
    else
    {
        printf("netFd[%d] recv [%s]\n", GetNetFd(), recvBuf);
        string stemp;
        g_sc->Send(stemp.assign(recvBuf, ret));
    }
    return 0;
}

void CTcpConnHandler::Close()
{
	printf("serv handler fd[%d] closed\n", GetNetFd());
    close(GetNetFd());
    SetNetFd(-1);
    DetachPollerUnit();
}
