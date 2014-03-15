#include "str_util.h"
#include <iostream>

using namespace std;

int main()
{
    string str = CStrUtil::trim(" Hello World!\n ");
    str = CStrUtil::trim(str, "\n");
    cout << str << endl;
}
