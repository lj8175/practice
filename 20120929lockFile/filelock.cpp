#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

// mount -oremount,mand /

//touch mandatory.txt
//chmod g+s,g-x mandatory.txt
 
int main(int argc, char **argv) {
    if (argc != 2) 
    {
        printf("Please use as : %s LockFile\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_WRONLY);
    if(fd == -1) {
        printf("Unable to open the file\n");
        exit(1);
    }

    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();
 
    int ret = fcntl(fd, F_SETLKW, &lock);
    printf("Return value of fcntl:%d\n",ret);
    if(ret==0) {
        getchar();
    }
}
