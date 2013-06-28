#include<stdio.h>
#include<unistd.h>
#include"tlog.h"

using namespace tbase::tlog;
CTLog g_log;

int main()
{
  int err, i = 0;
  err = g_log.LOG_OPEN(LOG_TRACE, LOG_TYPE_CYCLE, ".", "mylog", 1<<30, 3);
  if(0 != err) 
  {
    printf("LOG_OPEN err");
  }
  for(i = 0; i < 100; i++)
  {
    g_log.LOG_P_ALL(LOG_TRACE, "i = %d \n", i);
    sleep(1);
  }
}
