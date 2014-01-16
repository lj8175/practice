#include<stdio.h> 
#include<string.h>
#include<unistd.h> 
#include<sys/types.h> 
#include<stdlib.h> 
#include<time.h> 

#include<sys/socket.h> 
#include<netinet/in.h> 
#include<arpa/inet.h>
 
#define SERVER_PORT 8175
#define LENGTH_OF_LISTEN_QUEUE 10 
#define BUFFER_SIZE 256 

int main(int argc, char** argv)
{
    int servfd,clifd;
    struct sockaddr_in servaddr;
    int reuse_addr = 1;

    if ((servfd = socket(AF_INET,SOCK_STREAM, 0 )) <  0 ){
       printf( "create socket error!\n" );
       exit( 1 );
    } 

    setsockopt (servfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof (reuse_addr));
 
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(servfd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
       printf( "bind to port %d failure!\n" ,SERVER_PORT);
       exit(1);
    } 
 
    if (listen(servfd,LENGTH_OF_LISTEN_QUEUE) <  0 ){
       printf( "call listen failure!\n" );
       exit( 1 );
    } 
 
    while ( 1 ){ // server loop will nerver exit unless any body kill the process 
       char buf[BUFFER_SIZE]={0};
       long timestamp;
       struct sockaddr_in serv, cli;
       socklen_t cli_len =  sizeof(struct sockaddr_in);
       socklen_t serv_len =  sizeof(struct sockaddr_in);
       char serv_ip[20], cli_ip[20];
       clifd = accept(servfd,(struct sockaddr*)NULL, NULL);
       if (clifd <  0 ){
          printf("error comes when call accept!\n");
          break ;
       } 
 
       getpeername(clifd, (struct sockaddr *)&cli, &cli_len);
       inet_ntop(AF_INET, &cli.sin_addr, cli_ip, sizeof(cli_ip));
       getsockname(clifd, (struct sockaddr *)&serv, &serv_len);
       inet_ntop(AF_INET, &serv.sin_addr, serv_ip, sizeof(serv_ip));
       timestamp = time(NULL);
       ctime_r(&timestamp, buf);
       int len = strlen(buf)-1;
       snprintf(buf+len, BUFFER_SIZE-len, " | from client IP[%s] Port[%d] accepted by server IP [%s] Port[%d] \n" ,
                cli_ip, ntohs(cli.sin_port), serv_ip, ntohs(serv.sin_port));
       printf("%s",buf);

       send(clifd,buf,strlen(buf), 0 );

       close(clifd);      
    } // exit 
 
    close(servfd);

    return  0 ;
}
