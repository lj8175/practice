#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#if defined(__rtems__)
# include <rtems.h>
#endif

#define NUMBER 5

#define rtems_test_assert(__exp) \
  do { \
    if (!(__exp)) { \
      printf( "%s: %d %s\n", __FILE__, __LINE__, #__exp ); \
      exit(0); \
    } \
  } while (0)


void *startRoutine(void *arg){
  pthread_attr_t attr;
  int detachstate;
  int status;
#if defined(__rtems__)
  printf("Thread %x Stack: %x -- %x (%dk) \n", *(int*)arg, _Thread_Executing->Start.Initial_stack.area, (char*)_Thread_Executing->Start.Initial_stack.area + _Thread_Executing -> Start.Initial_stack.size, _Thread_Executing -> Start.Initial_stack.size/1024);
//  pthread_exit(NULL);
#else
  status = pthread_getattr_np(pthread_self(),&attr);
  rtems_test_assert( status ==0 );

  status = pthread_attr_getdetachstate (&attr, &detachstate);
  rtems_test_assert( status ==0 );

  if(detachstate == PTHREAD_CREATE_DETACHED)
    printf("New Thread %x: PTHREAD_CREATE_DETACHED", *(int *)arg);
  if(detachstate == PTHREAD_CREATE_JOINABLE)
    printf("New Thread %x: PTHREAD_CREATE_JOINABLE", *(int *)arg);

  printf(" exit\n");
//  pthread_exit(NULL);
#endif
}

int main(){
  pthread_t th[NUMBER];
  pthread_attr_t attr;
  int i,errno,status;
  int detachstate;

  status=pthread_attr_init(&attr);
  rtems_test_assert( status ==0 );

  status = pthread_attr_getdetachstate (&attr, &detachstate);
  rtems_test_assert( status ==0 );

  if(detachstate == PTHREAD_CREATE_DETACHED)
    printf("pthread_attr_getdetachstate(default): PTHREAD_CREATE_DETACHED\n");
  if(detachstate == PTHREAD_CREATE_JOINABLE)
    printf("pthread_attr_getdetachstate(default): PTHREAD_CREATE_JOINABLE\n");

  status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  //status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  rtems_test_assert( status ==0 );

  for(i=0;i<NUMBER;i++){
    if(pthread_create(th+i,&attr,startRoutine,th+i)) printf("pthread_create failed\n");
  }

  //wait for all thread terminate
  sleep(3);
  for(i=0;i<NUMBER;i++){
    if(errno=pthread_join(th[i],0)){
      printf("pthread_join: %x %d %s\n",th[i],errno,strerror(errno));
    }
  }
}
