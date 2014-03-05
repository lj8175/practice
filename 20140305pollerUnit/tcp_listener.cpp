/*
 * tcp_listener.cpp
 *
 *  Created on: 2014-3-4
 *      Author: joelliu
 */

#include "tcp_listener.h"
#include "tcpconn_handler.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

CTcpListener::CTcpListener(int port, CPollerUnit *pollerUnit): CPollerObject(pollerUnit, -1), m_servPort(port)
{
    if ((m_netFd = socket(AF_INET,SOCK_STREAM, 0 )) <  0 )
    {
       perror("create socket error" );
       exit( 1 );
    }

    struct sockaddr_in servaddr;
    bzero( &servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(m_servPort);
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(m_netFd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
       printf( "bind to port %d failure!\n" ,m_servPort);
       exit( 1 );
    }

    if (listen(m_netFd,10) <  0 )
    {
       printf( "call listen failure!\n " );
       exit( 1 );
    }
    SetNetFd(m_netFd);
    EnableInput();
    DisableOutput();
    DisableHangup();
    AttachPollerUnit();
}

int CTcpListener::InputNotify()
{
    int fd = accept (m_netFd, NULL, 0);
    if (fd < 0)
    {
        return -1;
    }
    //CTcpConnHandler *hdl = new CTcpConnHandler(m_pollerUnit, fd);
    new CTcpConnHandler(m_pollerUnit, fd);

    return 0;
}

