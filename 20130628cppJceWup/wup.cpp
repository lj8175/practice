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
    pack.setServantName("q");
    pack.setFuncName("q");
    pack.put<Int32>("qq", 1462622762);
    pack.encode(pstr);
    cout << "pack.encode length: " << pstr.length() << endl //51
         << "content : " << pstr << endl;
}
