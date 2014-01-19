#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdlib.h> 


#define SERVER_PORT 8888// define the defualt connect port id 
#define BUFFER_SIZE 256 

int main(int argc, char** argv)
{
    int servfd,clifd,length = 0;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t socklen = sizeof(servaddr);
    char buf[BUFFER_SIZE];
    int on = 1;
    fd_set read_set, rfds, wfds;

    if ((clifd = socket(AF_INET,SOCK_STREAM, 0))<0){
       printf("create socket error!\n");
       exit(1);
    }

    bzero(&cliaddr, sizeof (cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = htons(INADDR_ANY);

    bzero(&servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERVER_PORT);

    if (connect(clifd,(struct sockaddr*)&servaddr, socklen)<0){
       printf("can't connect \n");
       exit(1);
    } 

    setsockopt(clifd, IPPROTO_TCP, TCP_NODELAY,&on,sizeof(on));

    FD_ZERO(&read_set);
    FD_SET(clifd, &read_set);

    int batch_count = 10;
    int count = 0;
    while(1)
    {
        rfds = wfds = read_set;
        select(clifd+1, &rfds, &wfds, NULL, NULL);
        if(FD_ISSET(clifd, &rfds))
        {
            bzero(buf, BUFFER_SIZE);
            int len = recv(clifd, buf, BUFFER_SIZE, 0);
            if (len == -1)
            {
                printf("error occurred when recv clifd\n");
                break;
            }
            else if (len == 0)
            {
                printf("client close socket, clifd[%d]\n", clifd);
                close(clifd);
            }
            else
            {
                printf("%s", buf);
            }
        }
        if(FD_ISSET(clifd, &wfds))
        {
            count++;
            snprintf(buf, BUFFER_SIZE, "Hello%d\n", count);
            send(clifd,buf,strlen(buf),0);
            if(count%batch_count==0) sleep(5);
        }
    }

    close(clifd);
    return 0;
} 
