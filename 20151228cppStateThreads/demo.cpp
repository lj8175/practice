#include <stdio.h>
#include <stdlib.h>
#include "st.h"

void* do_sleep(void* arg){
    int sleep_s = (int)(long int)(char*)arg;

    for(;;){
        printf("in sthread #%d s\n", sleep_s);
        st_sleep(sleep_s);
    }

    return NULL;
}

int main(int argc, char** argv){
    if(argc <= 1){
        printf("Usage: %s <sthread_count>\n"
               "eg. %s 10000\n", argv[0], argv[0]);
        return -1;
    }

    if(st_init() < 0){
        printf("error!");
        return -1;
    }
    int count = atoi(argv[1]);

    for(int i = 1; i <= count; i++){
        if(st_thread_create(do_sleep, (void*)i, 0, 0) == NULL){
            printf("error!");
            return -1;
        }
    }

    st_thread_exit(NULL);

    return 0;
}
