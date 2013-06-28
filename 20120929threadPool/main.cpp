#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

const int TASK_COUNT = 1000;
const int THREAD_CNT = 200;

void * myprocess (void *arg)
{
    printf ("threadid is 0x%x, working on task %d\n", (int)pthread_self (),*(int *) arg);
    sleep (1);
    return NULL;
}

int main()
{
    ThreadPool tp(THREAD_CNT);
    int arg[TASK_COUNT] = {0};
    for (int i=0; i<TASK_COUNT; ++i)
    {
        arg[i] = i;
        tp.addTask(myprocess, &arg[i]);
    }
    sleep(10);
}
