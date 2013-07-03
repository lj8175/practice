#include "trmem_cache_lib.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BASE_BUFF_SIZE 256

using namespace ssdasn; 
using namespace std; 

int main()
{
    CCharBuffer buf;
    string key = "key";
    string data= "data_value";
    int seq = rand();
    int base_size = BASE_BUFF_SIZE;
    base_size += data.size();
    base_size += key.size();
    if(base_size > MAX_PACKET_SIZE)
    {
        return -1;
    }
    buf.ExpandIfNeed(base_size);
    char *pkt_ptr = NULL;
    int pkt_len = 0;
    TReqExt req;

    trmem_encode(101020276, cache_op_set, seq, key, data, (char*)buf, buf.GetCapacity(), pkt_ptr, pkt_len, -1, 0, 0, -1, &req);
    cout << "pkt_len: " << pkt_len << endl;
    // send
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr ("10.136.9.77");
    remote.sin_port = htons(9101);
    connect(sockfd, (struct sockaddr*)&remote, sizeof(struct sockaddr_in));
    send(sockfd, pkt_ptr, pkt_len, 0);
    //recv
    int length = recv(sockfd, (char*)buf,buf.GetCapacity(), 0);
    cout << "recv length: " << length << endl;
    
    int retcode,ret_seq,ret_bid,op;
    string ret_key;
    int cos;
    trmem_decode((char*)buf, length, ret_bid, op, ret_seq, ret_key,data, retcode, cos, NULL);
    cout << "seq: " << seq << " ret_seq: " << ret_seq << " retcode: " << retcode << endl;
}

