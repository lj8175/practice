#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "ssl_connector.h"

using namespace std;

CSslConnector::CSslConnector(CPollerUnit *pollerUnit, string ip, int port, string certFile, string priFile)
    : CPollerObject(pollerUnit),m_destIp(ip),m_destPort(port),m_ctx(NULL), m_ssl(NULL),
      m_certFile(certFile), m_priFile(priFile), m_sendStrPos(0)
{
	Open();
}

CSslConnector::~CSslConnector()
{
    Close();
}

int32_t CSslConnector::Open()
{
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
    //m_ctx = SSL_CTX_new(SSLv23_client_method());
    m_ctx = SSL_CTX_new(SSLv3_client_method());

    SSL_CTX_set_default_passwd_cb_userdata(m_ctx,(void*)"123456");

    m_ssl = SSL_new(m_ctx);
    if (!m_ssl)
    {
        printf("CSslConnection SSL_new error\n");
        return -1;
    }

    if( 1 != SSL_use_certificate_file(m_ssl, m_certFile.c_str(), SSL_FILETYPE_PEM))
    {
        printf("CSslConnection Error loading certificate from file %s\n",m_certFile.c_str());
        SSL_free(m_ssl);
        return -2;
    }

    if ( 1 != SSL_use_PrivateKey_file(m_ssl, m_priFile.c_str(), SSL_FILETYPE_PEM) )
    {
        printf("CSslConnection Error loading private key from file %s\n",m_certFile.c_str());
        SSL_free(m_ssl);
        return -3;
    }

    // connect
    int netFd = socket (AF_INET, SOCK_STREAM, 0);
    if (netFd<0)
    {
        printf("socket for new fd error\n");
        return -4;
    }

    struct sockaddr_in sa;
    memset (&sa, 0, sizeof(sa));
    sa.sin_family      = AF_INET;
    sa.sin_addr.s_addr = inet_addr (m_destIp.c_str());   /* Server IP */
    sa.sin_port        = htons(m_destPort);
    if(connect(netFd, (struct sockaddr*) &sa, sizeof(sa)))
    {
        printf("connect to ip[%s] port[%d] errno[%d]\n", m_destIp.c_str(), m_destPort, errno);
        return -5;
    }

    SSL_set_fd (m_ssl, netFd);
    if (1 != SSL_connect (m_ssl))
    {
        printf("SSL_connect not successful");
        return -6;
    }

    EnableInput();
    DisableOutput();
    SetNetFd(netFd);
    int ret = AttachPollerUnit(); //将会把fd设置为非阻塞
    if (-1 == ret)
    {
        printf("AttachPoller() failed.");
        Close();
        return -7;
    }
    return 0;
}

int32_t CSslConnector::Destroy() // destroy res.
{
    if(m_ssl)
    {
        SSL_free(m_ssl);
        m_ssl = NULL;
    }
    if(m_ctx)
    {
        SSL_CTX_free(m_ctx);
        m_ctx = NULL;
    }
    return 0;
}

int32_t CSslConnector::Close()
{
    DisableInput();
    DisableOutput();

    SSL_shutdown(m_ssl);
    int32_t ret = DetachPollerUnit();
    if (-1 == ret)
    {
        printf("DetachPolloer() failed.");
    }
    if (GetNetFd() > 0)
    {
        printf("close m_netFd[%d].", GetNetFd());
        close(GetNetFd());

        SetNetFd(-1);
    }

    Destroy();
    return 0;
}

int32_t CSslConnector::InputNotify()
{
    printf("InputNotify() fd[%d]\n", GetNetFd());

    DoRecv();

    return 0;
}

int32_t CSslConnector::DoRecv()
{
    char rbuf[1<<12]={0};
    int32_t ret = 0;
    do{
        ret = SSL_read(m_ssl, rbuf, 1<<12);
        switch (SSL_get_error(m_ssl, ret))
        {
        case SSL_ERROR_NONE:
            break;
        case SSL_ERROR_ZERO_RETURN:
            Close();
            printf("ssl server close connection");
            return -1;
        case SSL_ERROR_WANT_READ:
            break;
        default:
            printf("SSL_read error\n");
            Close();
            return -2;
        }
    } while (SSL_pending(m_ssl));

    m_recvBuf.append(rbuf, ret);
    printf("ssl read [%d] bytes [%s]\n", ret, rbuf);

    return 0;
}


int32_t CSslConnector::OutputNotify()
{
    if (m_sendList.empty() && m_sendStr.empty())
    {
        DisableOutput();
        return 0;
    }
    else if (m_sendStr.empty())
    {
        m_sendStr = m_sendList.front();
        m_sendList.pop_front();
        m_sendStrPos = 0;
    }
    printf("ssl fd[%d] OutputNotify()\n", GetNetFd());
    DoSend();

    return 0;
}

int32_t CSslConnector::DoSend()
{
    int32_t ret = SSL_write(m_ssl, m_sendStr.c_str()+m_sendStrPos, m_sendStr.length()-m_sendStrPos);
    switch(SSL_get_error(m_ssl, ret))
    {
    case SSL_ERROR_NONE:
        m_sendStrPos += ret;
        break;
    case SSL_ERROR_WANT_WRITE:
        break;
    default:
        printf("SSL_write error");
        return -1;
    }

    if ((int)m_sendStr.length() == m_sendStrPos)
    {
        m_sendStr = "";
        m_sendStrPos = 0;
    }

    printf("SSL_write ret[%d]\n", ret);

    return 0;
}

int32_t CSslConnector::HangupNotify()
{
    printf("in hangup notify");
    Close();
    return 0;
}

void CSslConnector::Send(string str)
{
    m_sendList.push_back(str);
    printf("CSslConnector sendList add a str [%s]", str.c_str());
    EnableOutput();
    ApplyEvents();
}
