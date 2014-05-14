//http://www.cnblogs.com/fly-fish/archive/2011/12/30/2307265.html

#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <unistd.h> 
void* func(void *) 
{ 
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); //允许退出线程 
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //设置立即取消 
  int i=0;
  while (1) 
  { 
    i++;//操作 ; 
  } 
  printf("%d\n",i);
  return NULL; 
} 
int main(int argc, char *argv[]) 
{ 
  pthread_t thrd; 
  pthread_attr_t attr; 
  pthread_attr_init(&attr); 
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 
  if( pthread_create(&thrd, &attr, func, NULL) ) 
  { 
    perror( "pthread_create error"); 
    exit(EXIT_FAILURE); 
   } 
   if( !pthread_cancel(thrd) )  
   { 
     printf( "pthread_cancel OK\n" ); 
   } 
   /*
   pthread_join(thrd,NULL);
   printf("pthread join ok\n");
   */
   sleep(1);
   if( !pthread_cancel(thrd) )  
   { 
     printf( "pthread_cancel OK\n" ); 
   } 
   sleep( 30 ); 
   return 0; 
} 
