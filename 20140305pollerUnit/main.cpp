/*
 * main.cpp
 *
 *  Created on: 2014-3-4
 *      Author: joelliu
 */

#include "poller_unit.h"
#include "tcp_listener.h"
#include "tcpconn_handler.h"
#include "ssl_connector.h"

#define CERT_PATH "../20140218sslTwowayAuth/sslclientcert.pem"
#define KEY_PATH  "../20140218sslTwowayAuth/sslclientkey.pem"

CSslConnector *g_sc;

int main()
{
    CPollerUnit::Instance().Init(1000);
    new CTcpListener(8176, &CPollerUnit::Instance());
    g_sc = new CSslConnector(&CPollerUnit::Instance(), "127.0.0.1", 4433, CERT_PATH, KEY_PATH);
    for(;;)
    {
        CPollerUnit::Instance().WaitAndProcess(50);
    }

    return 0;
}


