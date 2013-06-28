#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#if defined(__rtems__)
# include <rtems.h>
#endif

#define SIG_SUSPEND SIGUSR1
#define SIG_THR_RESTART SIGUSR2
#define TIMES 5

#define rtems_test_assert(__exp) \
  do { \
    if (!(__exp)) { \
      printf( "%s +%d: in %s %s\n", __FILE__, __LINE__, __FUNCTION__, #__exp ); \
      exit(0); \
    } \
  } while (0)

sem_t GC_suspend_ack_sem;
sigset_t suspend_handler_mask;
pthread_t GC_threads[2];
static int times=0;

void GC_suspend_handler(int sig){
  printf("run in GC_suspend_handler times: %d\n", ++times);
  sem_post(&GC_suspend_ack_sem);
  sigsuspend (&suspend_handler_mask);
}
void GC_restart_handler(int sig){
  times++;
  printf("run in GC_restart_handler %d\n",times );
}

void* thread2(void *arg){
  int       status;

  printf("T2 (%lx) pthread_kill SIG_SUSPEND %lx\n", pthread_self(), GC_threads[0]);
  status = pthread_kill(GC_threads[0], SIG_SUSPEND);
  rtems_test_assert( status == 0 );

  printf("T2 (%lx) pthread_kill SIG_SUSPEND %lx\n", pthread_self(), GC_threads[1]);
  status = pthread_kill(GC_threads[1], SIG_SUSPEND);
  rtems_test_assert( status == 0 );

  status = sem_wait(&GC_suspend_ack_sem);
  rtems_test_assert( status == 0 );

  status = sem_wait(&GC_suspend_ack_sem);
  rtems_test_assert( status == 0 );

  printf("T2 (%lx) pthread_kill SIG_THR_RESTART %lx\n", pthread_self(), GC_threads[0]);
  status = pthread_kill(GC_threads[0], SIG_THR_RESTART);
  rtems_test_assert( status == 0 );

  printf("T2 (%lx) pthread_kill SIG_THR_RESTART %lx\n", pthread_self(), GC_threads[1]);
  status = pthread_kill(GC_threads[1], SIG_THR_RESTART);
  rtems_test_assert( status == 0 );

  puts("T2 - exit");
  return NULL;
}

void* thread1(void *arg){
  int       status;
  pthread_t newThread;

  status = pthread_create(&newThread,NULL,thread2,NULL);
  rtems_test_assert( status == 0 );

  puts( "T1 - Join with T2" );
  status = pthread_join( newThread, NULL );
  if(status) printf("NewThread1 pthread_join return %d\n", status);
  //rtems_test_assert( status == 0 );

  puts( "T1 - exit" );
  return NULL;
}
int main(){
  struct sigaction act;
  pthread_t        newThread;
  pthread_t        mainThread;
  int              status;
  int              i;

  status = sem_init(&GC_suspend_ack_sem, 0, 0);
  rtems_test_assert( status == 0 );

  status = sigemptyset(&act.sa_mask);
  rtems_test_assert( status == 0 );

  status = sigaddset(&act.sa_mask,SIG_SUSPEND);
  rtems_test_assert( status == 0 );

  status = sigaddset(&act.sa_mask,SIG_THR_RESTART);
  rtems_test_assert( status == 0 );

  status = pthread_sigmask(SIG_UNBLOCK,&act.sa_mask,NULL);
  rtems_test_assert( status == 0 );

  act.sa_handler=GC_suspend_handler;

  status = sigaction(SIG_SUSPEND,&act,NULL);
  rtems_test_assert( status == 0 );

  act.sa_handler=GC_restart_handler;

  status = sigaction(SIG_THR_RESTART,&act,NULL);
  rtems_test_assert( status == 0 );

  status = sigfillset(&suspend_handler_mask);
  rtems_test_assert( status == 0 );

  status = sigdelset(&suspend_handler_mask, SIG_THR_RESTART);
  rtems_test_assert( status == 0 );

  mainThread = pthread_self();
  status = pthread_create(&newThread,NULL,thread1,NULL);
  rtems_test_assert( status == 0 );

  GC_threads[0] = mainThread;
  GC_threads[1] = newThread;

  for(i=0; i<5; i++){
    printf("%d\n",i);
    status = sleep(1);
    printf("sleep returned %d/%s\n", status, strerror(errno) );
  }

  status = pthread_join( newThread, NULL );
  if(status) printf("MainThread pthread_join return %d\n", status);
  rtems_test_assert( status == 0 );

  printf( "Back from MainThread pthread_join times: %d\n", times );
  puts( "*** END OF TEST ***" );
  return 0;
}
