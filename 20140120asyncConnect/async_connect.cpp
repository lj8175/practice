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
