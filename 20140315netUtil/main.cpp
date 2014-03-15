#include "net_util.h"

int main()
{
    int sfd = CNetUtil::TcpBindListen((char*)"127.0.0.1", 8176);
    for(;;)
    {
        int fd = accept(sfd,NULL, NULL);
        int ip, port;
        CNetUtil::GetPeerName(fd, ip, port);
        printf("client ip [%s] port [%u]\n", CNetUtil::IpStr(ip).c_str(), (int)port);
    }
    return 0;
}
