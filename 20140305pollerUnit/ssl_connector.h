#ifndef SSL_CONNECTOR_H_
#define SSL_CONNECTOR_H_

#include <list>
#include <string>
#include "poller_unit.h"
#include <openssl/err.h>
#include <openssl/ssl.h>


using namespace std;

class CSslConnector : public CPollerObject
{
public:
    CSslConnector(CPollerUnit *pollerUnit, string ip, int port, string certFile, string priFile);
    virtual ~CSslConnector();

public: // CPollerObject
    virtual int32_t InputNotify();
    virtual int32_t OutputNotify();
    virtual int32_t HangupNotify();

public:


    // 调用此函数发送命令
    virtual void Send(string str);

    // 连接是否打开？
    virtual inline bool IsOpened()
    {
        return GetNetFd() > 0;
    }

    virtual CPollerUnit* GetPollerUnit()
    {
        return m_pollerUnit;
    }

protected:
    // 打开连接
    virtual int32_t Open();
    // 关闭连接
    virtual int32_t Close();
    virtual int32_t DoRecv();
    virtual int32_t DoSend();
    virtual int32_t Destroy(); // destroy res.

protected:
    string             m_destIp;
    int                m_destPort;

    //ssl相关
    SSL_CTX           *m_ctx;
    SSL               *m_ssl;
    string             m_certFile;
    string             m_priFile;
    list<string>       m_sendList;
    string             m_recvBuf;
    string             m_sendStr;
    int                m_sendStrPos;
};

#endif /* SSL_CONNECTOR_H_ */
