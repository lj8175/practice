#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

#define TIMEOUT 3

enum psFlag{FromPhone, FromSms};

pthread_t  main_id;
enum psFlag     flag;

void eventHandle(int sig){
  static enum psFlag  saveFlag;
  static int          phase = 0;
  static time_t       saveTime;

  if(phase == 0){
    phase = 1;
    saveFlag = flag;
    saveTime = time(NULL);
#ifdef DEBUG
    printf("Initialized time %d\n",(int)saveTime);
#endif
  }else{
    if(saveFlag == flag) saveTime = time(NULL);
    else{
#ifdef DEBUG
      printf("previous signal is %s, \ttime %d \n", saveFlag==FromPhone?"FromPhone":"FromSms", (int)saveTime);
      printf("current  signal is %s, \ttime %d \n", flag==FromPhone?"FromPhone":"FromSms", (int)time(NULL));
#endif
      if((time(NULL)-saveTime) < TIMEOUT)
        printf("phone and sms at the same time, %d\n", (int)(time(NULL)-saveTime));
      saveTime = time(NULL);
      saveFlag = flag;
    }

  }

}

void* phone(void *arg){
  int s;
  for(;;){
    s = rand()%10;
    printf("phone: sleep %d s, \t\ttime %d\n", s, (int)time(NULL));
    sleep(s);
    pthread_kill(main_id,SIGUSR1);
    flag = FromPhone;
  }
}
void* sms(void *arg){
  int s;
  srand((unsigned int)time(NULL));
  for(;;){
    s = rand()%10;
    printf("sms:   sleep %d s, \t\ttime %d\n", s, (int)time(NULL));
    sleep(s);
    pthread_kill(main_id,SIGUSR1);
    flag = FromSms;
  }
}

int main(){
  struct sigaction  act;
  pthread_t         phone_id;
  pthread_t         sms_id;

  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask,SIGUSR1);
  pthread_sigmask(SIG_UNBLOCK,&act.sa_mask,NULL);

  act.sa_handler=eventHandle;
  sigaction(SIGUSR1,&act,NULL);

  main_id = pthread_self();
  pthread_create(&phone_id,NULL,phone,NULL);
  pthread_create(&sms_id,NULL,sms,NULL);

  pthread_join(phone_id,NULL);
  pthread_join(sms_id,NULL);

}
