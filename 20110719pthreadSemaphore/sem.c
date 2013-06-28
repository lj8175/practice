#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define NUMBER 5

void *startRoutine(void *arg){
  sleep(1);
  if(sem_post((sem_t *)arg)) printf("sem_post failed\n");
  sleep(10);
}

int main(){
  pthread_t th[NUMBER];
  pthread_attr_t attr;
  sem_t *sem;
  int i;
  pthread_attr_init(&attr);
  for(i=0;i<NUMBER;i++){
    sem=(sem_t*)malloc(sizeof(sem_t)); if(!sem) printf("malloc failed\n");
    if(sem_init(sem,0,0)) printf("sem_init failed\n");
    if(pthread_create(th+i,&attr,startRoutine,sem)) printf("pthread_create failed\n");
    while (sem_wait(sem)) {
      printf("errno=%d EINVAL %d\n",errno, (errno==EINVAL)?1:0);
      if (EINTR != errno) {printf("sem_wait failed\n"); exit(127);}
    }
    if(sem_destroy(sem)) printf("sem_destroy failed\n");
    free(sem);
    printf("created a new thread!\n");
  }
  
  for(i=0;i<NUMBER;i++){
    if(pthread_join(th[i],0)) printf("pthread_join failed\n");
  }
}
