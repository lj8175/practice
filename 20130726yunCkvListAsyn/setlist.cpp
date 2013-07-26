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
    //user data, not over MAX_LIST_SIZE==256 is best
    vector<TKeyNode> v_node;
    TKeyNode tkn;
    tkn.key = "key1";
    tkn.data = "data1";
    v_node.push_back(tkn);
    tkn.key = "key2";
    tkn.data = "data2";
    v_node.push_back(tkn);

    // set buffer
    CCharBuffer buf;
    int seq = rand();
    int base_size = BASE_BUFF_SIZE;
    for(int i = 0; i < (int)v_node.size(); ++i)
    {
        base_size += v_node[i].data.size();
        base_size += v_node[i].key.size();
    }
    if(base_size > MAX_PACKET_SIZE)
    {
        return -1;
    }
    buf.ExpandIfNeed(base_size);
    char *pkt_ptr = NULL;
    int pkt_len = 0;
    TReqExt req;

    //encode
    trmem_encode_list(101020276, cache_op_setlist, seq, v_node, (char*)buf, buf.GetCapacity(), pkt_ptr, pkt_len,&req);
    cout << "pkt_len: " << pkt_len << " buf addr: " << ((char*)buf)[0] << " pkt_ptr addr: " << hex << (long)pkt_ptr << dec << endl;
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
    
    // check pack
    int ret = trmem_pkt_complete((char*)buf, length);
    cout << "check recv: " << ret << endl;
    
    //decode
    int retcode,ret_seq,op;
    string ret_key;
    retcode = trmem_decode_list((char*)buf, length, op, ret_seq, v_node);
    cout << "seq: " << seq << " ret_seq: " << ret_seq << " decode ret: " << retcode << endl;

    for ( size_t i=0; i<v_node.size(); i++ )
    {
        cout << v_node[i].key << " : " << v_node[i].data << " : " << v_node[i].retcode << endl;
    }
}

