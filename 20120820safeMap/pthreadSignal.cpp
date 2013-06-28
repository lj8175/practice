#include<sstream>
#include<iostream>
#include<pthread.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"safemap.h"

SafeMap sm;

void *threadFunc(void *arg)
{
    srand((unsigned)(time(NULL)*(*(int*)arg)));
    while(1)
    {
        usleep(1000);
        ostringstream oss;
        int a, b;
        a = rand()%256;
        b = rand()%256;
        oss<<"127.0."<<a<<"."<<b;
//        cout<<oss.str()<<endl;
        sm.tryInsert(oss.str(), time(NULL));
    }
}

void *sigFunc(void *arg)
{
    int sig;
    sigset_t sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGUSR1);
    while(1)
    {
        sigwait(&sigs, &sig); 
        sm.write2file(string("ip.txt"));
        printf("receive signal %d\n", *(int*)arg);
    }
}

int main()
{
    sigset_t sigs;
    pthread_t pid[4];
    int args[4]={1,2,3,4};
    int i;
    
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigs, NULL);

    for(i=0;i<3;i++)
    {
        pthread_create(&pid[i], NULL, &threadFunc, &args[i]);
    }
    pthread_create(&pid[3], NULL, &sigFunc, &args[3]);
    for(i=0;i<4;i++)
    {
        pthread_join(pid[i], NULL);
    }
}
