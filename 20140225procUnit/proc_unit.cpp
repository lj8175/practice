#include "proc_unit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

bool CProcUnit::m_stoped = false;


int CProcUnit::DealCmdOpt(int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "d")) != -1) 
    {
        switch (opt) 
        {
            case 'd':
                printf("run in daemon\n");
                daemon(1,1);
                break;
            default:
                break;
        }
    }
    return 0;
}

int CProcUnit::Start(int argc, char* argv[])
{
    DealCmdOpt(argc, argv);
    SetupSignal();
    StartProcs();
    MonitorProcs();
    return 0;
}


void CProcUnit::SignalHandler(int signo)
{
    pid_t pid = getpid();
    printf("pid[%d] recv signal[%d], exit now\n", pid, signo);
    m_stoped = true;
    if(signo==SIGQUIT) exit(0);

}

void CProcUnit::SetupSignal()
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    // 设置关键信号非阻塞
    sigset_t sset;
    sigset_t osset;

    sigemptyset(&sset);
    sigaddset(&sset, SIGSEGV);
    sigaddset(&sset, SIGBUS);
    sigaddset(&sset, SIGABRT);
    sigaddset(&sset, SIGILL);
    sigaddset(&sset, SIGFPE);
    sigprocmask(SIG_UNBLOCK, &sset, &osset);

    // 处理退出信号
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = CProcUnit::SignalHandler;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
}

void CProcUnit::StartProcs()
{
    for(map<CProcObject*, int>::iterator it=m_procMap.begin(); it!=m_procMap.end(); it++)
    {
        if(it->second) continue;
        pid_t pid = fork();
        if(pid<0)
        {
            printf("fork failed, exit now\n");
            KillProcs();
            exit(0);
        }
        else if(pid==0) //children
        {
            it->first->Run();
            exit(0);
        }
        else
        {
            it->second = pid;
            printf("[proc uint] start a proc[%d]\n", pid);
        }
    }
}

void CProcUnit::MonitorProcs()
{
    while(!m_stoped)
    {
        int status = 0;
        pid_t pid = waitpid(-1, &status, 0);
        if (pid<0)
        {
            printf("[proc uint] waitpid ret[%d] errno[%d]\n", pid, errno);
            continue;
        }
        for(map<CProcObject*, int>::iterator it=m_procMap.begin(); it!=m_procMap.end(); it++)
        {
            if(it->second == pid)
            {
                printf("[proc unit] pid[%d] quit, restart it\n", pid);
                it->second = 0;
                break;
            }
        }
        printf("[proc unit] MonitorProcs restart Procs\n");
        StartProcs();
    }
    KillProcs();
}

void CProcUnit::KillProcs()
{
    for(map<CProcObject*, int>::iterator it=m_procMap.begin(); it!=
m_procMap.end(); it++)
    {
        if(it->second==0) continue;
        kill(it->second, SIGQUIT);
    }
}















