#include "proc_unit.h"
#include <stdio.h>
#include <unistd.h>

class CProc : public CProcObject
{
    void Run()
    {
        while(1)
        {
            printf("[%d] run ..\n", getpid());
            sleep(1);
        }
    }
};


int main(int argc, char* argv[])
{
    CProc p1, p2;
    CProcUnit::Instance().AddProcObject(&p1);
    CProcUnit::Instance().AddProcObject(&p2);
    CProcUnit::Instance().Start(argc, argv);
}
