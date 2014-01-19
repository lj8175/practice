#include<stdio.h> 
#include<string.h>
#include<unistd.h> 
#include<sys/types.h> 
#include<stdlib.h> 
#include<time.h> 

#include<sys/socket.h> 
#include<netinet/in.h> 
#include<arpa/inet.h>
#include<netinet/tcp.h>
 
#define SERVER_PORT 8888 // define the defualt connect port id 
#define LENGTH_OF_LISTEN_QUEUE 10 // length of listen queue in server 
#define BUFFER_SIZE 256 
#define WELCOME_MESSAGE "Welcome to connect the server." 

int main(int argc, char** argv)
{
    int servfd,clifd=STDOUT_FILENO,maxfd;
    struct sockaddr_in servaddr,cliaddr;
    fd_set read_set, ready_set;
    int on = 1;
    int count = 1;

    if ((servfd = socket(AF_INET,SOCK_STREAM, 0)) <  0 ){
       printf("create socket error!\n");
       exit(1);
    } 

    bzero(&servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(servfd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
       printf("bind to port %d failure!\n" ,SERVER_PORT);
       exit(1);
    } 
 
    if (listen(servfd,LENGTH_OF_LISTEN_QUEUE) <  0 ){
       printf("call listen failure!\n");
       exit(1);
    } 

    setsockopt (servfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));

    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    FD_SET(servfd, &read_set);
    maxfd = servfd>STDIN_FILENO?servfd:STDIN_FILENO;
 
    while (1){
       char buf[BUFFER_SIZE];
       long timestamp;
       socklen_t length =  sizeof (cliaddr);
       ready_set = read_set;
       select(maxfd+1, &ready_set, NULL, NULL, NULL);
       
       if(FD_ISSET(STDIN_FILENO, &ready_set)){
          printf("something happened at stdin, so exit now\n");
          close(servfd);
          exit(0);
       }

        if(FD_ISSET(servfd, &ready_set))
        {
            if(clifd!=STDOUT_FILENO)
            {
                FD_CLR(clifd, &read_set);
                close(clifd);
            }
            clifd = accept(servfd,( struct sockaddr * ) &cliaddr, &length);
            if (clifd<0){
                printf("error comes when call accept!\n" );
                break ;
            } 
            on = 1;
            setsockopt(clifd, IPPROTO_TCP, TCP_NODELAY,&on,sizeof(on));
            FD_SET(clifd, &read_set);
            maxfd = maxfd>clifd?maxfd:clifd;

            printf("Connect from client IP:%s, Port:%d \n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
        }

       if(FD_ISSET(clifd, &ready_set))
       {
            bzero(buf, BUFFER_SIZE);
            int len = recv(clifd, buf, BUFFER_SIZE, 0);
            if (len == -1)
            {
                printf("error occurred when recv clifd\n");
            }
            else if (len == 0)
            {
                printf("client close socket, clifd[%d]\n", clifd);
                FD_CLR(clifd, &read_set);
            }
            else
            {
                //printf("[%d] %s", len, buf);
                printf("%s", buf);
            }

            int i = 0;
            for(i=0; i<len; i++)
            if('H' == buf[i] )
            {
                char buf2[BUFFER_SIZE];
                strcpy(buf2,WELCOME_MESSAGE);
                timestamp = time(NULL);
                strcat(buf2, " Timestamp in server: " );
                strcat(buf2,ctime(&timestamp));
                snprintf(buf2+strlen(buf2)-1, BUFFER_SIZE-strlen(buf2)+1, " [%d]\n", count++);
                int ret = send(clifd,buf2,strlen(buf2),0);
                if (ret<0) perror("send ");
            }
       }

    }
 
    close(servfd);

    return  0 ;
}
