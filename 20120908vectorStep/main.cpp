#include <iostream>
#include "lclient.h"

using namespace std;

const int CALL_TIMES = 10000000;

int main()
{
    LClient lc;
    lc.saveServer();
    for( int i = 0; i < CALL_TIMES; ++i)
    {
        if(!lc.isOver())
        {
            lc.allocServer();
        }
    }

}
