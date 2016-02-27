#ifndef __m3u8_monitor__HttpStream__
#define __m3u8_monitor__HttpStream__


#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include "Stream.h"
using namespace std;


//保存连接过程中的耗时(ms)
struct ConnectInfo {
    long dnsTime; //DNS耗时
    long connTime; //创建连接耗时
    string url;
    
//    ConnectInfo operator = (const ConnectInfo &info) {
//        dnsTime = info.dnsTime;
//        connTime = info.connTime;
//        url = info.url;
//    }
};


class HttpStream: public Stream {

private:
    string url;
    
    map<string, string> requestHeader;
    map<string, string> responseHeader;
    
    int sockfd;
    
    char *recvBuf;
    long recvBufLen; //当前recvBuf中的数据长度
    long recvBufMaxLen; //recvBuf的最大长度
    long responseBodyLen; //response body的长度
    long recvBodyLen; //当前已经接收body的长度
    
    bool noContentLenAndChunk; //未指定content length和chunked模式传输
    bool chunkedMode; //是否是chunked模式传输
    long chunkSizeUnRead; //标记当前chunk中有多少数据没有读取
    
    bool streamEnd; //http流是否全部读完
        
    int parse_url(string &url, string &host, string &path, int &port);
    int make_request_text(string &requestType, string &requestText, string &requestBody, string &url, string &host, int port, string &path);
    int send_http_request(string &url, string &host, string &path, uint32_t ip, int port, string &requestText);
    int parse_response_header(map<string, string> &header, char *headText, long len);
    
public:
    HttpStream();
    ~HttpStream();
    
    int open(const char*);
    int close();
    
    int read(char*, int, bool = true);
    int write(const char*, int);
    
    int end();
    int seek(StreamPos, int);
    
    int openPost(const char*, const char *);
    
    ConnectInfo lastConnInfo; //保存连接耗时
    ConnectInfo connInfo;
};

clock_t clock_ms();

#endif /* defined(__m3u8_monitor__HttpStream__) */
