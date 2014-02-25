#include "proc_unit.h"
#include <stdio.h>

class CProc : public CProcObject
{
    void Run()
    {
        printf("run ..");
    }
};


int main()
{
    CProc p1;
    CProcUnit::Instance().AddProcObject(&p1);

}
