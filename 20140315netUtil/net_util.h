#ifndef _NET_UTIL_H_
#define _NET_UTIL_H_

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <string>

#define SOCKET_INVALID          -1
#define SOCKET_CREATE_FAILED    -2
#define SOCKET_BIND_FAILED      -3
#define SOCKET_LISTEN_FAILED    -4

using std::string;

class CNetUtil
{
public:
	static int setnonblocking(int sock);
    static int TcpBindListen(char *addr, uint16_t port, int backlog=20);
    // error when ret < 0, else return fd
    static int TcpConnect(const char* host, const int port, const int timeout_ms=3000);
    static int UdpBind(char *addr, uint16_t port, int rbufsz = 0, int wbufsz = 0);
    static int InitUnixAddr(struct sockaddr_un *addr, const char *path);
    static int UnixBind (const char* path, int backlog);
    static int UnixConnect (int* netfd, const char* path, int block = 1, int* out_errno = NULL);
    static int InitUdpUnix(const char* szPath, int bFlag);
    static int WaitUdpUnix(int sock_fd, int nTimeout);

    static string IpStr(int ip);
    static int IpInt(string ip);
    static int GetLocalIp(char* name);
    static int GetSockName(int fd, int &ip, int &port);
    static int GetPeerName(int fd, int &ip, int &port);
};

#endif

