#include "proc_unit.h"
#include <stdio.h>
#include <unistd.h>

class CProc : public CProcObject
{
    void Run()
    {
        printf("run ..");
    }
};


int main(int argc, char* argv[])
{
    CProc p1;
    CProcUnit::Instance().Start(argc, argv);
    sleep(5);
}
