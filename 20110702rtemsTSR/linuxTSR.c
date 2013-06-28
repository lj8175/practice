#include <stdio.h>    // for printf()  
#include <stdlib.h>   // for exit()
#include <unistd.h>   // for pause()  
#include <signal.h>   // for signal()  
#include <string.h>   // for memset()  
#include <sys/time.h> // struct itimeral. setitimer()  


#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define TOTAL 200
volatile int total=0;

void periodic(){
  char f8mp[5*1024*1024];
  int fd, i;
  FILE *f;
  struct timeval tpstart,tpend;
  float timeuse;

  gettimeofday(&tpstart,0);
  f=fopen("test.bin","w+");
  for(i=0;i<1;i++)
    fwrite(f8mp,sizeof(f8mp),1,f);
  fflush(f);
  fclose(f);
  gettimeofday(&tpend,0);

  timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
  printf("time used: %f ms \n",timeuse/1000);
  if(++total == TOTAL) exit(0);
}

int main(){
  int res=0;
  struct itimerval tick;
  memset(&tick,0,sizeof(tick));
  tick.it_value.tv_sec=1;
  tick.it_value.tv_usec=0;
  tick.it_interval.tv_sec=1;
  tick.it_interval.tv_usec=0;

  signal(SIGALRM,periodic);
  res=setitimer(ITIMER_REAL,&tick,NULL);
  if(res) {printf("set timer failed!\n");exit(127);}

  while(1) pause;
 
  return 0;
}
