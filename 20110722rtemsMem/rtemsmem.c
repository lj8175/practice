#include <bsp.h>
#include <stdio.h>
#include <stdlib.h>
void *POSIX_Init(void *);
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_POSIX_INIT_THREAD_TABLE
#define CONFIGURE_MAXIMUM_POSIX_THREADS 1
#define CONFIGURE_INIT
#include <rtems/confdefs.h>

void *POSIX_Init(
  void *argument
)
{
  printf( "RTEMS Work Space : 0x%X -- 0x%X (%dk)\n",
    Configuration.work_space_start,
    (char*)Configuration.work_space_start + Configuration.work_space_size,
    Configuration.work_space_size/1024
  );
  printf( "RTEMS Malloc Heap : 0x%X -- 0x%X (%dM)\n",
    RTEMS_Malloc_Heap->area_begin,
    RTEMS_Malloc_Heap->area_end,
    (RTEMS_Malloc_Heap->area_end - RTEMS_Malloc_Heap->area_begin)/1024/1024
  );
  exit( 0 );
}
