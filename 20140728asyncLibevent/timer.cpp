#include <stdio.h>
#include <time.h>
#include <event2/event.h>

struct cb_arg  
{  
    struct event *ev;  
    struct timeval tv;  
}; 


void timeout_cb(int fd, short event, void *params)  
{  
    printf("%d\n", (int)time(NULL));  
    struct cb_arg *arg = (struct cb_arg*)params;  
    evtimer_add(arg->ev, &(arg->tv));  
}  
  
int main()  
{  
    struct event_base *base = event_base_new();  
    struct event *timeout_ev = NULL;  
    struct timeval tv = {1, 0};  
    struct cb_arg arg;  
  
    timeout_ev = evtimer_new(base, timeout_cb, &arg);  
    arg.ev = timeout_ev;  
    arg.tv = tv;  

    evtimer_add(timeout_ev, &tv);  
    event_base_dispatch(base);  
    evtimer_del(timeout_ev);  
  
    return 0;  
}
