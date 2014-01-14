#include <sys/prctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void setProcName(int argc, char **argv, const char *name)
{
    extern char **environ;
    char *last;

    // copy environ
    size_t size = 0;
    for (int i = 0; environ[i]; ++i) {
        size += strlen(environ[i])+1; 
    }   

    char *raw = new char[size];
    for (int i = 0; environ[i]; ++i) {
        memcpy(raw, environ[i], strlen(environ[i]) + 1); 
        environ[i] = raw;
        raw += strlen(environ[i]) + 1;
    }   

    last = argv[0];
    for (int i = 0; i < argc; ++i) {
        last += strlen(argv[i]) + 1;   
    }   
    for (int i = 0; environ[i]; ++i) {
        last += strlen(environ[i]) + 1;
    }   

    // now set name, for ps aux
    argv[1] = 0;
    char *p = argv[0];
    memset(p, 0x00, last - p);
    strncpy(p, name, last - p);
    //for top
    prctl(PR_SET_NAME, name, NULL, NULL, NULL);
}

int main(int argc, char *argv[])
{
    setProcName(argc, argv, "my name is fuck you:[do not kill me]~~");
    sleep(30);  // you can ps now
    return 0;
}

