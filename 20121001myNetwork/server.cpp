#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "connectpool.h"
#include "threadpool.h"

using namespace std;

ConnectPool cp(0);
ThreadPool tp;
void *qprocess(void *arg)
{
    cp.dumpClientMsg();
}
void *oneread(void* arg)
{
    while(1)
    {
        tp.addTask(qprocess, NULL);
        usleep(100000);
    }
}

int main(int argc, char *argv[])
{
    int listenfd;
    int connfd;
    int port;
    int clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port> \n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);

    if ((listenfd = socket(AF_INET,SOCK_STREAM, 0 )) <  0 ){
       fprintf(stderr, " create socket error!\n " );
       exit( 1 );
    }

    bzero( &serveraddr, sizeof (serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(listenfd,(struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ){
       fprintf(stderr, "bind to port %d failure!\n" ,port);
       exit( 1 );
    }

    if (listen(listenfd,10) <  0 ){
       fprintf(stderr, " call listen failure!\n " );
       exit( 1 );
    }

    cp.addServerFd(listenfd);
    
    pthread_t tid;
    pthread_create(&tid, NULL, &oneread, NULL);

    int num = 0;
    while (1)
    {
        cp.m_ready_set = cp.getReadSet();
        cp.m_nready = select(cp.getMaxFd()+1, &cp.m_ready_set, NULL, NULL, NULL);
        if (-1 == cp.m_nready)
        {
            perror("select()");
        }

//        cout << "ready: " << cp.m_nready << endl;

        bool flags = cp.checkServer();
        bool flagc = cp.checkClients();
//        cout << boolalpha << flags << "\t" << flagc << endl;
    }


}
