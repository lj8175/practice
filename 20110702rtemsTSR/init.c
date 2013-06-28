#include <stdio.h>
#include <bsp.h>
#include <unistd.h>


rtems_id     TSRtimer;

void print_Routine(){
  static int i=0;
  printk("%d\t",i++);
}

rtems_timer_service_routine TSR_Routine( rtems_id id, void *ignored ){
  print_Routine();
  (void) rtems_timer_reset(id);
}

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;
  status = rtems_timer_create(rtems_build_name( 'T', 'S', 'R', ' ' ), &TSRtimer);
  rtems_timer_fire_after(TSRtimer,33,TSR_Routine,NULL);
  while(1) sleep(1);
  exit(0);

}



#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS         2
#define CONFIGURE_MAXIMUM_TIMERS        2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

