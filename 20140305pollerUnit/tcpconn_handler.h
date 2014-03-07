/*
 * tcpconn_handler.h
 *
 *  Created on: 2014-3-4
 *      Author: joelliu
 */

#ifndef TCPCONN_HANDLER_H_
#define TCPCONN_HANDLER_H_

#include "poller_unit.h"

class CTcpConnHandler : public CPollerObject
{
public:
    CTcpConnHandler(CPollerUnit *pollerUnit, int netFd);
    virtual ~CTcpConnHandler();

    int InputNotify();
    int OutputNotify(){return 0;}
    int HangupNotify(){return 0;}

    bool isOpened(){
        return GetNetFd()==-1;
    }

private:
    void Close();
};


#endif /* TCPCONN_HANDLER_H_ */
