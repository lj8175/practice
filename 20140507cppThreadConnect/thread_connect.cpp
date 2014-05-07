/*
 * thread_connect.cpp
 *
 *  Created on: 2014-5-7
 *      Author: joelliu
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include "openssl/ssl.h"

enum{
    SSLCONN_IDLE=1,
    SSLCONN_CONNECTING,
    SSLCONN_FAILED,
    SSLCONN_CONNECTED,
};

class CThreadConnect
{
public:
    CThreadConnect();
    int Open();
    int PushMessage(const char *token, const char *payload);
private:
    static void* SSLConnect(void *arg);
public:
    SSL_CTX           *m_ctx;
    SSL               *m_ssl;
    pthread_t          m_pid;
    int                m_sslConnState;
};

CThreadConnect::CThreadConnect():m_ctx(NULL),m_ssl(NULL),m_pid(0),m_sslConnState(SSLCONN_IDLE)
{
}

int CThreadConnect::Open()
{
    if(m_sslConnState==SSLCONN_CONNECTED)
    {
        printf("conn is connected\n");
        return -1;
    }
    if(m_sslConnState==SSLCONN_CONNECTING)
    {
        printf("conn is on the way\n");
        return -1;
    }
    m_sslConnState=SSLCONN_CONNECTING;
    if(pthread_create(&m_pid,NULL,SSLConnect,(void *)this) != 0)
    {
        m_sslConnState=SSLCONN_IDLE;
        printf("create thread error\n");
        return -1;
    }

    return 0;
}

void* CThreadConnect::SSLConnect(void *arg)
{
    CThreadConnect *ptr = (CThreadConnect*) arg;
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
    ptr->m_ctx = SSL_CTX_new(SSLv23_client_method());

    //SSL_CTX_set_default_passwd_cb_userdata(m_ctx,(void*)"123456");

    ptr->m_ssl = SSL_new(ptr->m_ctx);
    if (!ptr->m_ssl)
    {
        ptr->m_sslConnState=SSLCONN_FAILED;
        printf("CSslConnection SSL_new error\n");
        return NULL;
    }

    if( 1 != SSL_use_certificate_file(ptr->m_ssl, "certfile.txt", SSL_FILETYPE_PEM))
    {
        ptr->m_sslConnState=SSLCONN_FAILED;
        printf("CSslConnection Error loading certificate from file\n");
        SSL_free(ptr->m_ssl);
        return NULL;
    }

    if ( 1 != SSL_use_PrivateKey_file(ptr->m_ssl, "prifile.txt", SSL_FILETYPE_PEM) )
    {
        ptr->m_sslConnState=SSLCONN_FAILED;
        printf("CSslConnection Error loading private key from file\n");
        SSL_free(ptr->m_ssl);
        return NULL;
    }

    // connect
    int netfd = socket (AF_INET, SOCK_STREAM, 0);
    if (netfd<0)
    {
        ptr->m_sslConnState=SSLCONN_FAILED;
        printf("socket for new fd error\n");
        return NULL;
    }

    struct sockaddr_in sa;
    memset (&sa, 0, sizeof(sa));
    sa.sin_family      = AF_INET;
    sa.sin_addr.s_addr = inet_addr ("17.149.34.188");   /* Server IP */
    sa.sin_port        = htons(2195);
    if(connect(netfd, (struct sockaddr*) &sa, sizeof(sa)))
    {
        ptr->m_sslConnState=SSLCONN_FAILED;
        printf("connect to ip port errno[%d]\n", errno);
        return NULL;
    }
    printf("tcp connect to ip port success, use certFile\n");

    SSL_set_fd (ptr->m_ssl, netfd);
    if (1 != SSL_connect (ptr->m_ssl))
    {
        ptr->m_sslConnState=SSLCONN_FAILED;
        printf("SSL_connect not successful");
        return NULL;
    }
    ptr->m_sslConnState=SSLCONN_CONNECTED;
    printf("ssl connect successful SSLCONN_CONNECTED\n");
    return NULL;
}

void token2bytes(const char *token, char *bytes)
{
    int val;
    while (*token)
    {
        sscanf(token, "%2x", &val);
        *(bytes++) = (char)val;
        token += 2;
        while (*token == ' ')
        {
            // skip space
            ++token;
        }
    }
}

#pragma pack(1)
struct CAPNsPushAns
{
    char m_command;
    /*
    0  No errors encountered
    1 Processing error
    2 Missing device token
    3 Missing topic
    4 Missing payload
    5 Invalid token size
    6 Invalid topic size
    7 Invalid payload size
    8 Invalid token
    10 Shutdown
    255 None (unknown)
    */
    char m_errcode;
    uint32_t m_seq;
};
#pragma pack()

int CThreadConnect::PushMessage(const char *token, const char *payload)
{
    char tokenBytes[32]= {0};
    char message[293];
    int msgLength;

    token2bytes(token, tokenBytes);
    //snprintf(tokenBytes,sizeof(tokenBytes),"%s",TestDeviceToken);

    unsigned char command = 0;
    size_t payloadLength = strlen(payload);
    char *pointer = message;
    unsigned short networkTokenLength = htons((u_short)32);
    unsigned short networkPayloadLength = htons((unsigned short)payloadLength);
    memcpy(pointer, &command, sizeof(unsigned char));
    pointer +=sizeof(unsigned char);
    memcpy(pointer, &networkTokenLength, sizeof(unsigned short));
    pointer += sizeof(unsigned short);
    memcpy(pointer, tokenBytes, 32);
    pointer += 32;
    memcpy(pointer, &networkPayloadLength, sizeof(unsigned short));
    pointer += sizeof(unsigned short);
    memcpy(pointer, payload, payloadLength);
    pointer += payloadLength;
    msgLength = (int)(pointer - message);
    printf("%ld,before SSL_write\n",time(NULL));
    int ret = SSL_write(m_ssl, message, msgLength);
    printf("%ld,end SSL_write:%d\n",time(NULL),ret);

    printf("%ld,before SSL_read,need ans len:%zd\n",time(NULL),sizeof(CAPNsPushAns));
    CAPNsPushAns ans;
    ret = SSL_read(m_ssl,&ans,sizeof(ans));
    printf("%ld,end SSL_read,ret:%d\n",time(NULL),ret);

    return ret;
}



int main()
{
    CThreadConnect ctc;
    ctc.Open();
    ctc.Open();
    sleep(3);
    ctc.Open();
    char payload[] = "{\"aps\":{\"alert\":\"Hello world--xiangchen!!! message from c++\",\"badge\":1},\"xg\":{\"bid\":0,\"ts\":1394158966}}";
    ctc.PushMessage("a906d80490731b13c620031a788d59c7073c63a31549e2512fa6c4d4f8078eb5", payload);
    printf("sleep 10 second and exit\n");
    sleep(10);
}

