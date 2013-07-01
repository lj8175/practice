#include "tpnsheader.h"
#include "tpns.h"
#include <fstream>

using namespace std;
using namespace TPNS_PUSH_PROTOCOL;

#define BUFLEN 1024

int main()
{
    char buf[BUFLEN];
    int start = sizeof(TpnsCommonHeader)+sizeof(TpnsHeader);
    int bufLen = 1024 - start;
    int tmpbufLen = bufLen; 

    TpnsCommonHeader ch;
    ch.version = 1;
    ch.cmd= CMD_PUSH;
    ch.flag = 0;
    memcpy(buf, &ch, sizeof(TpnsCommonHeader));

    TpnsPushReq tpr;
    tpr.accessId = 1000;
    memcpy(tpr.msgId.bits,"tpns push", sizeof("tpns push"));
    tpr.token = "tpns token";
    tpr.payload.type = TYPE_TRANSPARENT;
    tpr.payload.transparentMsg.msg = "tpns msg";
    tpr.activity = "tpns activity";
    struct_TpnsPushReq_pack(&tpr, (uint8_t *)(buf+start), &bufLen, NULL);
    start += bufLen;
    cout << "TpnsPushReq len: " << bufLen << endl;

    TpnsHeader th;
    th.length = htonl(start);
    th.seq = htonl(rand());
    th.rspCode = 128;
    memcpy(buf+sizeof(TpnsCommonHeader), &th, sizeof(TpnsHeader));

    ofstream fout("tpns.txt");
    for(int i=0; i<start; i++)
    {
        fout << buf[i];
    }
    fout << flush;

}
