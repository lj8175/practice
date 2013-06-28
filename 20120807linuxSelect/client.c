#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h> 


#define SERVER_PORT 8175 // define the defualt connect port id 
#define BUFFER_SIZE 255 

int main(int argc, char** argv)
{
    int servfd,clifd,length = 0;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t socklen = sizeof(servaddr);
    char buf[BUFFER_SIZE];

    if (argc < 2 ){
    	 printf( "usage: %s IpAddr\n" ,argv[0]);
       exit( 1 );
    } 

    if ((clifd = socket(AF_INET,SOCK_STREAM, 0 )) < 0 ){
       printf( "create socket error!\n" );
       exit( 1 );
    }
 
    bzero( & cliaddr, sizeof (cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = htons(INADDR_ANY);

    bzero( & servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    inet_aton(argv[ 1 ], &servaddr.sin_addr);
    servaddr.sin_port = htons(SERVER_PORT);

    if (connect(clifd,( struct sockaddr * ) & servaddr, socklen) < 0 ){
       printf( "can't connect to %s!\n", argv[ 1 ]);
       exit( 1 );
    } 

    length = recv(clifd, buf, BUFFER_SIZE, 0);
    if (length < 0){
       printf( "error comes when recieve data from server %s!", argv[1] );
       exit( 1 );
    } 

    printf( " from server %s : %s", argv[1], buf);

    close(clifd);
    return 0;
} 
