#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <rtems.h>

#define GROW_HIGH 0
#define GROW_LOW  1

int testStack(void *p){
  if(p==NULL){
    printf("0x%08X\n",&p);
    return testStack(&p);
  }
  else{
    printf("0x%08X\n",&p);
    if(&p<p) return GROW_LOW;
    else     return GROW_HIGH;
  }
}

rtems_task Init(rtems_task_argument argument){

  printf("Task stack area (%lu Bytes): 0x%08" PRIXPTR ".. 0x%08" PRIXPTR "\n",
    (unsigned long) _Thread_Executing->Start.Initial_stack.size,
    _Thread_Executing->Start.Initial_stack.area,
    ((char *)_Thread_Executing->Start.Initial_stack.area+_Thread_Executing->Start.Initial_stack.size)
  );
  if(testStack(NULL)==GROW_LOW) printf("Stack Grow Direct: GROW_LOW\n");
  else printf("Stack Grow Direct: GROW_HIGH\n");

  exit(0);
}

#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#include <rtems/confdefs.h>
