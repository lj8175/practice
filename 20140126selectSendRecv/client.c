#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>

#define SERVER_PORT 8888// define the defualt connect port id 
#define BUFFER_SIZE 256 

// error when ret < 0, else return fd
int connect_timeout(const char* host, const int port, const int timeout_ms)
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

//ret -3 : timeout
int send_timeout(int socket, const void *buffer, const int length, const int timeout_ms)
{
    if(socket<0 || length<=0 || timeout_ms<0) return -1;
    fd_set wset;
    FD_ZERO(&wset);
    FD_SET(socket, &wset);
    struct timeval tval={timeout_ms/1000, (timeout_ms%1000)*1000};
    int ret = select(socket+1, NULL, &wset, NULL, &tval);
    if(ret<0) return -2;
    if(ret==0) return -3;
    if(FD_ISSET(socket, &wset))
    {
        ret = send(socket, buffer, length, 0);
        if (ret<0) return -4;
    }

    return ret;
}

//ret -3 : timeout
int recv_timeout(int socket, void *buffer, const int length, const int timeout_ms)
{
    if(socket<0 || length<=0 || timeout_ms<0) return -1;
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(socket, &rset);
    struct timeval tval={timeout_ms/1000, (timeout_ms%1000)*1000};
    int ret = select(socket+1, &rset, NULL, NULL, &tval);
    if(ret<0) return -2;
    if(ret==0) return -3;
    if(FD_ISSET(socket, &rset))
    {
        ret = recv(socket, buffer, length, 0);
        if (ret<0) return -4;
    }

    return ret;
}

int main(int argc, char** argv)
{
    char buf[BUFFER_SIZE];
    int on = 1;

    int clifd = connect_timeout("127.0.0.1", SERVER_PORT, 2000);
    if (clifd<0) return -1;
    setsockopt(clifd, IPPROTO_TCP, TCP_NODELAY,&on,sizeof(on));

    int count = 0;
    int ret = 0;
    while(1)
    {
        bzero(buf,BUFFER_SIZE);
        ret = recv_timeout(clifd, buf, BUFFER_SIZE, 5);
        if(ret<0 && ret!=-3) printf("recv_timeout ret [%d]\n", ret);
        else if(ret==0) {printf("socket is close!\n"); break;}
        else printf("%s", buf);

        if(count>10000) continue;
        snprintf(buf, BUFFER_SIZE, "Hello%d",count++);
        ret = send_timeout(clifd, buf, strlen(buf), 5);
        if(ret<=0 && ret!=-3) printf("send_timeout ret [%d]\n", ret);
    }

    close(clifd);
    return 0;
} 
