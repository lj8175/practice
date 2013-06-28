#include "wup/wup.h"
#include <iostream>

using namespace wup;
using namespace std;

int main()
{
    UniPacket<> pack;
    cout << "sizeof UniPacket<> : " << sizeof(pack) << endl;
    string pstr;
    pack.setRequestId(1);
    pack.setServantName("server");
    pack.setFuncName("func()");
    pack.put<Int32>("qq", 1462622762);
    pack.encode(pstr);
    cout << "pack.encode length: " << pstr.length() << endl //61
         << "content : " << pstr << endl;
}
