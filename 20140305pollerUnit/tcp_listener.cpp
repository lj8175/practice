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
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

CTcpListener::CTcpListener(int port, CPollerUnit *pollerUnit): CPollerObject(pollerUnit, -1), m_servPort(port)
{
	int netFd = 0;
    if ((netFd = socket(AF_INET,SOCK_STREAM, 0 )) <  0 )
    {
       perror("create socket error" );
       exit( 1 );
    }

    struct sockaddr_in servaddr;
    bzero( &servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(m_servPort);
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(netFd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
       printf( "bind to port %d failure!\n" ,m_servPort);
       exit( 1 );
    }

    if (listen(netFd,10) <  0 )
    {
       printf( "call listen failure!\n " );
       exit( 1 );
    }
    printf("server listen on fd[%d]\n", netFd);
    SetNetFd(netFd);
    EnableInput();
    DisableOutput();
    DisableHangup();
    AttachPollerUnit();
}

int CTcpListener::InputNotify()
{
    int fd = accept (GetNetFd(), NULL, 0);
    if (fd < 0)
    {
        return -1;
    }
//    CTcpConnHandler *hdl = new CTcpConnHandler(m_pollerUnit, fd);
    printf("accept a new client fd[%d]\n", fd);
    new CTcpConnHandler(m_pollerUnit, fd);

    return 0;
}

