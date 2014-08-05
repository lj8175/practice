#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string>

using namespace std;

string hostname2ip(string hostname)
{
    char    buf[4096];
    struct  hostent hostinfo,*phost;
    int     ret;
    string  ret_str;

    if(gethostbyname_r(hostname.c_str(),&hostinfo,buf,sizeof(buf),&phost,&ret)==0)
    {
        if (phost && hostinfo.h_addr_list[0])
        {
            ret_str = inet_ntoa(*(struct in_addr*)hostinfo.h_addr_list[0]);
        }
    }
    return ret_str;
}

int main(int argc, char **argv)
{
    string ip = hostname2ip(string(argv[1]));
    printf("ip : %s\n", ip.c_str());
}
