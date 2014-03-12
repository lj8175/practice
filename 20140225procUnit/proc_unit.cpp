#include "proc_unit.h"
#include <stdio.h>
#include <unistd.h>


int CProcUnit::DealCmdOpt(int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "d")) != -1) 
    {
        switch (opt) 
        {
            case 'd':
                printf("run in daemon\n");
                daemon(1,1);
                break;
        }
    }
    return 0;
}

int CProcUnit::Start(int argc, char* argv[])
{
    DealCmdOpt(argc, argv);
    return 0;
}
