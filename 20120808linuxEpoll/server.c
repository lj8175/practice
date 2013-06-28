#include<stdio.h> 
#include<string.h>
#include<sys/socket.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<netinet/in.h> 
#include<stdlib.h> 
#include<time.h> 
#include<sys/epoll.h>
 
#define SERVER_PORT 8175 // define the defualt connect port id 
#define LENGTH_OF_LISTEN_QUEUE 10 // length of listen queue in server 
#define BUFFER_SIZE 255 
#define WELCOME_MESSAGE "welcome to connect the server. " 
#define MAXEPOLLSIZE 10000

int main(int argc, char** argv)
{
    int servfd,clifd;
    struct sockaddr_in servaddr,cliaddr;
    int epfd;
    struct epoll_event ev[2];
    struct epoll_event events[MAXEPOLLSIZE];
    int nepfds;

    if ((servfd = socket(AF_INET,SOCK_STREAM, 0 )) <  0 ){
       printf( " create socket error!\n " );
       exit( 1 );
    } 
 
    bzero( &servaddr, sizeof (servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(servfd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
       printf( "bind to port %d failure!\n" ,SERVER_PORT);
       exit( 1 );
    } 
 
    if (listen(servfd,LENGTH_OF_LISTEN_QUEUE) <  0 ){
       printf( " call listen failure!\n " );
       exit( 1 );
    } 

    epfd = epoll_create(1); 
    if(epfd < 0 )
    {
      perror("epoll_create()");
      return;
    }
    bzero(ev, 2*sizeof(struct epoll_event));
    ev[0].events = EPOLLIN;
    ev[1].events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLPRI | EPOLLHUP | EPOLLET;
    ev[0].data.fd = STDIN_FILENO;
    ev[1].data.fd = servfd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev[0])<0)
    {
      perror("epoll_ctl");
      return;
    }
    
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, servfd, &ev[1])<0)
    {
      perror("epoll_ctl");
      return;
    }

    while ( 1 ){ 
       char buf[BUFFER_SIZE];
       long timestamp;
       int i;
       socklen_t length =  sizeof (cliaddr);

       nepfds = epoll_wait(epfd, events, 3, -1);
       if(nepfds == -1)
       {
         perror("epoll_wait");
         break;
       }
       
       for(i = 0; i < nepfds; i++)
       {
         if(events[i].data.fd == STDIN_FILENO){
            printf("something happened at stdin~~\n");
            close(servfd);
            exit(0);
         }
         if(events[i].data.fd == servfd)
         {
            clifd = accept(servfd,( struct sockaddr * ) &cliaddr, &length);

            if (clifd <  0 ){
               printf( " error comes when call accept!\n " );
               break ;
            }  
 
            strcpy(buf,WELCOME_MESSAGE);

            // inet_ntop(INET_ADDRSTRLEN,cliaddr.sin_addr,buf,BUFFER_SIZE); 

            printf( "from client,IP:%s, Port:%d \n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

            timestamp = time(NULL);

            strcat(buf, "\n timestamp in server: " );
            strcat(buf,ctime(&timestamp));

            send(clifd,buf,BUFFER_SIZE, 0 );

            close(clifd);      
         }
       }

    } // exit 
 
    close(servfd);

    return  0 ;
}
