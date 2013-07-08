#include "tpns_header.h"
#include "tpns.h"
#include <fstream>

using namespace std;
using namespace TPNS_PUSH_PROTOCOL;

#define BUFLEN 1024
#define SERVER_PORT 5574
#define SERVER_IP "10.1.152.139"


int main()
{
    int servfd,clifd,length = 0;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t socklen = sizeof(servaddr);
    char buf[BUFLEN];
    int start = sizeof(TpnsHeader)+sizeof(TpnsContent);
    int bufLen = 1024 - start;
    int tmpbufLen = bufLen; 

    TpnsHeader ch;
    ch.version = 1;
    ch.cmd= CMD_PUSH;
    ch.type= TYPE_REQ;
    memcpy(buf, &ch, sizeof(TpnsHeader));

    TpnsPushReq tpr;
    tpr.accessId = 1000;
    tpr.msgId = "tpns msgId";
    tpr.token = "tpns token";
    tpr.payload.type = TYPE_TRANSPARENT;
    tpr.payload.transparentMsg.msg = "tpns msg";
    tpr.activity = "tpns activity";
    struct_TpnsPushReq_pack(&tpr, (uint8_t *)(buf+start), &bufLen, NULL);
    start += bufLen;
    cout << "TpnsPushReq len: " << bufLen << endl;

    TpnsContent th;
    th.length = htonl(start);
    th.seq = htonl(rand());
    th.rspCode = 128;
    memcpy(buf+sizeof(TpnsHeader), &th, sizeof(TpnsContent));

    ofstream fout("tpns.txt");
    for(int i=0; i<start; i++)
    {
        fout << buf[i];
    }
    fout << flush;
    fout.close();

    // network
    if ((clifd = socket(AF_INET,SOCK_STREAM, 0 )) < 0 ){
       printf( "create socket error!\n" );
       exit( 1 );
    }

    bzero( & cliaddr, sizeof (cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = htons(INADDR_ANY);

    bzero( & servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    inet_aton(SERVER_IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(SERVER_PORT);

    if (connect(clifd,( struct sockaddr * ) & servaddr, socklen) < 0 ){
       printf( "can't connect to server!\n");
       exit( 1 );
    }

    length = send(clifd, buf, start, 0);
    if (length < 0){
       printf( " error comes when recieve data from server ! " );
       exit( 1 );
    }

    length = recv(clifd, buf, BUFLEN, 0);
    if (length < 0){
       printf( "error comes when recieve data from server !" );
       exit( 1 );
    }

    printf( " from server : %s", buf);

    close(clifd);

}
