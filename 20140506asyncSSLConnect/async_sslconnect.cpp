#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>

#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/pkcs12.h>

#define TCP_CONNECT 1;
#define SSL_CONNECT 2;


int g_state = 0;
int g_done = 0;

int fd = 0;
int epfd = 0;
struct epoll_event ev;
bool first_time = true;
bool sslfirst_time = true;

SSL_CTX           *m_ctx;
SSL               *m_ssl;

int async_connect(const char* host, const int port, const int timeout_ms);
int pushmessage(const char *token, const char *payload);


bool ssl_connect()
{
    if (sslfirst_time)
    {
        SSL_load_error_strings();
        ERR_load_BIO_strings();
        OpenSSL_add_all_algorithms();
        SSL_library_init();
        m_ctx = SSL_CTX_new(SSLv23_client_method());

        //SSL_CTX_set_default_passwd_cb_userdata(m_ctx,(void*)"123456");

        m_ssl = SSL_new(m_ctx);
        if (!m_ssl)
        {
            printf("CSslConnection SSL_new error\n");
            exit(0);
        }

        if( 1 != SSL_use_certificate_file(m_ssl, "certfile.txt", SSL_FILETYPE_PEM))
        {
            printf("CSslConnection Error loading certificate from file\n");
            SSL_free(m_ssl);
            exit(0);
        }

        if ( 1 != SSL_use_PrivateKey_file(m_ssl, "prifile.txt", SSL_FILETYPE_PEM) )
        {
            printf("CSslConnection Error loading private key from file\n");
            SSL_free(m_ssl);
            exit(0);
        }

        SSL_set_fd (m_ssl, fd);

        sslfirst_time = false;
    }
    int retcode = SSL_connect (m_ssl);
    if (1 == retcode)
    {
        printf("ssl connect ok, after SSL_connect\n");
        return true;
    }

    switch(SSL_get_error(m_ssl, retcode))
    {
             case SSL_ERROR_WANT_WRITE:
                 ev.events = EPOLLOUT;
                 ev.data.fd = fd;
                 epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
                 printf("ssl connect SSL_ERROR_WANT_WRITE\n");
                 break;
             case SSL_ERROR_WANT_READ:
                 ev.events = EPOLLIN;
                 ev.data.fd = fd;
                 epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
                 printf("ssl connect SSL_ERROR_WANT_READ\n");
                 break;
            default:
                printf("SSL_connect err=%s\n",ERR_error_string(retcode,0));
                return true;
    }
    return false;

}


bool tcp_connect()
{
    struct sockaddr_in server_addr;
    int r_code;
    bzero(&server_addr,sizeof(struct sockaddr_in));

    if (first_time)
    {
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2195);
        inet_aton("17.149.34.188",&server_addr.sin_addr);
        r_code = connect(fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

        if (r_code == 0)
        {
            printf("connect ok first time\n");
            return true;
        }

        if (errno == EINPROGRESS)
        {
                ev.events = EPOLLIN | EPOLLOUT;
                ev.data.fd = fd;
                epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
                printf("connect EINPROGRESS\n");
        }
        else
        {
            printf("error occered , exit\n");
            exit(0);
        }
        first_time = false;
        return false;
    }
    int optval;
    int optlen = sizeof(optval);
    r_code = getsockopt(fd, SOL_SOCKET, SO_ERROR, &optval, (socklen_t*)&optlen);

    if (r_code < 0 || optval)
    {
        printf("r_code[%d] optval[%d], connect error and exit now\n", r_code, optval);
        //first_time = true;
        exit(0);
    }

    /* Get ready for SSL connect */
    ev.events = EPOLLOUT;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
    printf("connect ok, fd[%d]\n", fd);
    return true;
}


void action()
{
    printf("run in action , go to [%d]\n", g_state);
    switch(g_state)
    {
        case 1:
            if(tcp_connect())
            {
                g_state = SSL_CONNECT;
                printf("tcp connect ok, go to ssl connect\n");
            }
            break;
        case 2:
            if(ssl_connect())
            {
                g_done = 1;
                printf("ssl connect ok\n");
            }
            break;
    }
}


int main()
{
    /*
    int ret = async_connect("17.149.34.188", 2195, 3000);
    printf("async_connect ret[%d]", ret);
    return 0;
    */



    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd<=0) return -2;
    int flags= fcntl(fd, F_GETFL, 0);
    if (flags<0)
    {
        return -3;
    }
    if (fcntl(fd, F_SETFL, flags|O_NONBLOCK)==-1)
    {
        return -4;
    }

    int epfd = epoll_create(10);
    if(epfd < 0 ) return -1;


    struct epoll_event events[10];

    ev.events = EPOLLOUT;
    ev.data.fd = fd;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)<0)
    {
        return -5;
    }
    g_state = TCP_CONNECT;
    while(!g_done)
    {
        int num_fd = epoll_wait(epfd, events, 10, 1000);
        if (num_fd <= 0)
        {
            printf("num_fd[%d], continue\n", num_fd);
            continue;
        }
        //printf("num_fd[%d], fd[%d]\n", num_fd, events[0].data.fd);
        action();
    }

    char payload[] = "{\"aps\":{\"alert\":\"Hello world--xiangchen!!! message from c++\",\"badge\":1},\"xg\":{\"bid\":0,\"ts\":1394158966}}";
    pushmessage("a906d80490731b13c620031a788d59c7073c63a31549e2512fa6c4d4f8078eb5", payload);

}


// error when ret < 0, else return fd
int async_connect(const char* host, const int port, const int timeout_ms)
{
    if(NULL==host || 0>=port || 0>timeout_ms) return -1;

    int fret=0;
    struct timeval tval={timeout_ms/1000, (timeout_ms%1000)*1000};
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd<=0) return -2;
    int flags= fcntl(fd, F_GETFL, 0);
    if (flags<0)
    {
        fret=-3; goto errout;
    }
    if (fcntl(fd, F_SETFL, flags|O_NONBLOCK)==-1)
    {
        fret=-4; goto errout;
    }
    
    struct sockaddr_in  host_addr;    
    bzero(&host_addr,sizeof(struct sockaddr_in));
    host_addr.sin_family = AF_INET;
    inet_aton(host,&host_addr.sin_addr);
    host_addr.sin_port = htons(port);
    fret = connect(fd, (struct sockaddr *)&host_addr, sizeof(host_addr));
    if(fret==0) goto done;
    if(fret<0)
    {
        if(errno!=EINPROGRESS)
        {
            fret=-5; goto errout;
        }
    }
    //ret<0 and errno == EINPROGRESS
    fd_set rset, wset;
    FD_ZERO(&rset); FD_SET(fd, &rset);
    wset = rset;
    fret = select(fd+1, &rset, &wset, NULL, &tval);
    if(fret<=0)
    {
        fret=-6; goto errout;
    }
    if(FD_ISSET(fd, &rset) || FD_ISSET(fd, &wset))
    {
        int error;
        int len = sizeof(error);
        int bok = getsockopt(fd, SOL_SOCKET, SO_ERROR, &error,(socklen_t*)&len);
        if (bok<0 || error)
        {
            fret=-7; goto errout;
        }
    }
    else
    {
        fret=-8; goto errout;
    }

done:
    if (fcntl(fd, F_SETFL, flags)==-1)
    {
        fret=-9; goto errout;
    }
    return fd;

errout:
    close(fd);
    return fret;
    
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

int pushmessage(const char *token, const char *payload)
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


/*
int main(int argc, char* argv[])
{
    if(argc<4)
    {
        printf("use as: %s ip port timeout_ms\n", argv[0]);
        exit(0);
    }
    int port = atoi(argv[2]);
    int timeout_ms= atoi(argv[3]);
    int fd = async_connect(argv[1], port, timeout_ms);
    printf("fd:%d\n", fd);
    close(fd);
}
*/
