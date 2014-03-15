#include "net_util.h"
#include <sys/ioctl.h>
#include <net/if.h>

int CNetUtil::TcpConnect(const char* host, const int port, const int timeout_ms)
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


int CNetUtil::InitUnixAddr(struct sockaddr_un *addr, const char *path)
{
    bzero (addr, sizeof (struct sockaddr_un));
    addr->sun_family = AF_LOCAL;
    strncpy(addr->sun_path, path, sizeof(addr->sun_path) - 1);
    socklen_t addrlen = SUN_LEN(addr);
    addr->sun_path[0] = '\0';

    return addrlen;
}

int CNetUtil::UnixConnect(int* netfd, const char* path, int block, int* out_errno)
{
    *netfd = SOCKET_INVALID;
    struct sockaddr_un unaddr;
    socklen_t addrlen;

    if ((*netfd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1)
    {
        if (NULL != out_errno)
        {
            *out_errno = errno;
        }

        return SOCKET_CREATE_FAILED;
    }

    if (!block)
    {
        fcntl (*netfd, F_SETFL, O_RDWR | O_NONBLOCK);
    }

    addrlen = InitUnixAddr(&unaddr, path);

    int ret = ::connect (*netfd, (struct sockaddr *)&unaddr, addrlen);

    if (NULL != out_errno)
    {
        *out_errno = errno;
    }

    return ret;

}

int CNetUtil::InitUdpUnix(const char* szPath, int bFlag)
{
    int sock_fd;
    int sock_opt = 0;
    struct sockaddr_un sa;

    if ((sock_fd = ::socket(PF_UNIX, SOCK_DGRAM, 0)) == -1)
        return -1;

    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(sock_opt));

    if (bFlag)
        fcntl(sock_fd, F_SETFL, O_NONBLOCK);

    bzero (&sa, sizeof (struct sockaddr_un));
    sa.sun_family = AF_LOCAL;
    strncpy(sa.sun_path, szPath, sizeof(sa.sun_path) - 1);
    socklen_t addrlen = SUN_LEN(&sa);
    sa.sun_path[0] = '\0';

    if (::bind(sock_fd, (struct sockaddr *) &sa, addrlen ) == -1 )
        return -1;

    return sock_fd;
}

int CNetUtil::WaitUdpUnix(int sock_fd, int nTimeout)
{
    struct timeval timeout;

    fd_set fd_r;

    FD_ZERO(&fd_r);
    FD_SET(sock_fd, &fd_r);

    if (nTimeout > 0)
    {
        timeout.tv_sec = nTimeout;
        timeout.tv_usec = 0;
        select(sock_fd + 1, &fd_r, NULL, NULL, &timeout);
    }
    else
    {
        select(sock_fd + 1, &fd_r, NULL, NULL, NULL);
    }

    if (!FD_ISSET(sock_fd, &fd_r))
        return -1;

    return 0;
}

int CNetUtil::TcpBindListen(char *addr, uint16_t port, int backlog)
{
    struct sockaddr_in inaddr;
    int reuse_addr = 1;
    int netfd;

    bzero (&inaddr, sizeof (struct sockaddr_in));
    inaddr.sin_family = AF_INET;
    inaddr.sin_port = htons (port);

    char *end = strchr(addr, ':');
    if (end)
    {
        char *p = (char *)alloca(end - addr + 1);
        memcpy(p, addr, end - addr);
        p[end - addr] = '\0';
        addr = p;
    }
    if (strcmp(addr, "*") != 0 && inet_pton(AF_INET, addr, &inaddr.sin_addr) <= 0)
    {
        return -1;
    }

    if ((netfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return -2;
    }

    setsockopt (netfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof (reuse_addr));
    setsockopt (netfd, SOL_TCP, TCP_NODELAY, &reuse_addr, sizeof (reuse_addr));
    reuse_addr = 60;
    setsockopt (netfd, SOL_TCP, TCP_DEFER_ACCEPT, &reuse_addr, sizeof (reuse_addr));

    if (::bind(netfd, (struct sockaddr *)&inaddr, sizeof(struct sockaddr)) == -1)
    {
        close (netfd);
        return -3;
    }

    if (::listen(netfd, backlog) == -1)
    {
        close (netfd);
        return -4;
    }

    return netfd;
}

int CNetUtil::UdpBind(char *addr, uint16_t port, int rbufsz, int wbufsz)
{
    struct sockaddr_in inaddr;
    int netfd;

    bzero (&inaddr, sizeof (struct sockaddr_in));
    inaddr.sin_family = AF_INET;
    inaddr.sin_port = htons (port);

    char *end = strchr(addr, ':');
    if (end)
    {
        char *p = (char *)alloca(end - addr + 1);
        memcpy(p, addr, end - addr);
        p[end - addr] = '\0';
        addr = p;
    }

    if (strcmp(addr, "*") != 0 && inet_pton(AF_INET, addr, &inaddr.sin_addr) <= 0)
    {
        return -1;
    }

    if ((netfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        return -2;
    }

    if (::bind (netfd, (struct sockaddr *)&inaddr, sizeof(struct sockaddr)) == -1)
    {
        close (netfd);
        return -3;
    }

    if (rbufsz > 0)
        setsockopt(netfd, SOL_SOCKET, SO_RCVBUF, &rbufsz, sizeof(rbufsz));

    if (wbufsz > 0)
        setsockopt(netfd, SOL_SOCKET, SO_SNDBUF, &wbufsz, sizeof(wbufsz));

    return netfd;
}

int CNetUtil::UnixBind(const char *path, int backlog)
{
    // add parameter check by xandylai
    if (NULL == path)
    {
        return -1;
    }

    struct sockaddr_un unaddr;
    int netfd;

    socklen_t addrlen = CNetUtil::InitUnixAddr(&unaddr, path);

    if ((netfd = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        return -2;
    }

    if (::bind (netfd, (struct sockaddr *)&unaddr, addrlen) == -1)
    {
        close (netfd);
        return -3;
    }

    if (listen (netfd, backlog) == -1)
    {
        close (netfd);
        return -4;
    }

    return netfd;
}

int CNetUtil::setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0)
    {
        return -1;
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0)
    {
        return -2;
    }

    return 0;
}

string CNetUtil::IpStr(int ip)
{
    struct in_addr addr;
    addr.s_addr = ip;
    return string(inet_ntoa(addr));
}

int CNetUtil::IpInt(string ip)
{
    struct in_addr addr;
    if( inet_aton(ip.c_str(), &addr) == 0 ){
        return -1;
    }

    return addr.s_addr;
}

int CNetUtil::GetLocalIp(char* name)
{
     int sockfd;   
   
     if(-1 == (sockfd = socket(PF_INET, SOCK_STREAM, 0)))  
     {  
         return -1;  
     }  
   
     struct ifreq req;  
     struct sockaddr_in *host;  
     bzero(&req, sizeof(struct ifreq));  
   
     strncpy(req.ifr_name, name, sizeof(req.ifr_name)-1);  
     ioctl(sockfd, SIOCGIFADDR, &req);  

     ::close(sockfd);  

     host = (struct sockaddr_in*)&req.ifr_addr;  

     return host->sin_addr.s_addr;  
}

int CNetUtil::GetSockName(int fd, int &ip, int &port)
{
    if( fd <= 0 ) {
        return -1;
    }

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if( getsockname(fd, (struct sockaddr *)&addr, &addr_len) != 0) {
        return -2;
    }

    ip = addr.sin_addr.s_addr;
    port = ntohs( addr.sin_port );

    return 0;
}

int CNetUtil::GetPeerName(int fd, int &ip, int &port)
{
    if( fd <= 0 ) {
        return -1;
    }

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if( getpeername(fd, (struct sockaddr *)&addr, &addr_len) != 0)
    {
        return -2;
    }

    ip = addr.sin_addr.s_addr;
    port = ntohs( addr.sin_port );

    return 0;
}
