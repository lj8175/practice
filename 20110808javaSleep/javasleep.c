#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(){
  pthread_cond_t	cond;
  pthread_mutex_t	mutex;
  struct timespec	to;
  struct timeval	tv;
  int			i;

  pthread_cond_init(&cond,NULL);
  pthread_mutex_init(&mutex,NULL);

  for(i=0;i<10;i++){
    gettimeofday (&tv, NULL);
    to.tv_sec=tv.tv_sec;
    to.tv_nsec=tv.tv_usec*1000+10000000;
    if(to.tv_nsec>1000000000){to.tv_sec++;to.tv_nsec-=1000000000;}

    pthread_mutex_lock(&mutex);
    pthread_cond_timedwait(&cond,&mutex,&to); //sleep 10 ms
    pthread_mutex_unlock(&mutex);

    gettimeofday (&tv, NULL);
    printf("%d: sleep %d ns\n",i, (tv.tv_sec-to.tv_sec)*1000000000+tv.tv_usec*1000-to.tv_nsec+10000000);
  }
}

