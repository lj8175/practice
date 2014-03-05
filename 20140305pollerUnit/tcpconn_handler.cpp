/*
 * tcpconn_handler.cpp
 *
 *  Created on: 2014-3-4
 *      Author: joelliu
 */


#include "tcpconn_handler.h"
#include <errno.h>
#include <sys/socket.h>

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
    int32_t ret = recv(m_netFd, recvBuf, 1<<12, 0);
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
        printf("netFd[%d] recv [%s]\n", m_netFd, recvBuf);
    }
    return 0;
}

void CTcpConnHandler::Close()
{
    close(m_netFd);
    m_netFd = -1;
    DetachPollerUnit();
}
