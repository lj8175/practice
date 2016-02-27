#include "HttpStream.h"
#include "Utils.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <algorithm>



#define LRLN "\r\n"
#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))



HttpStream::HttpStream()
{
    sockfd = 0;
    recvBufMaxLen = 4 << 10;
    recvBuf = new char[recvBufMaxLen];
    streamEnd = true;
}

HttpStream::~HttpStream()
{
    if(sockfd > 0) {
        ::close(sockfd);
        sockfd = 0;
    }
    delete []recvBuf;
    
    responseHeader.clear();
    requestHeader.clear();
}

int HttpStream::open(const char *url)
{
    recvBufLen = 0;
    recvBodyLen = 0;
    responseBodyLen = 0;
    chunkSizeUnRead = 0;
    chunkedMode = false;
    noContentLenAndChunk = false;
    streamEnd = false;

    this->url = url;
    lastConnInfo.url = url;
    
    string host;
    string path;
    int port;
    
    if(parse_url(this->url, host, path, port)) {
        printf("parse url error.\n");
        return -1;
    }
    
    string requestType = "GET";
    string requestText;
    string requestBody;
    
    if(make_request_text(requestType, requestText, requestBody, this->url, host, port, path)) {
        printf("parse url error.\n");
        return -1;
    }
    
//    printf("%s\n", requestText.c_str());
    
    if(send_http_request(this->url, host, path, Utils::get_host_ip(host, &lastConnInfo.dnsTime), port, requestText)) {
    	printf("send_http_request error");
        return -1;
    }
    
    if(responseHeader.find("httpCode")->second.compare("302") == 0) {
        //302跳转
        close();
        string location = responseHeader.find("Location")->second;
        
        printf("Http重定向至%s\n", location.c_str());
        
        connInfo = lastConnInfo;
        
        return open(location.c_str());
    }
    
//    cout<<"DNS解析耗时"<<connInfo.dnsTime<<"ms"<<endl
//        <<"TCP连接耗时"<<connInfo.connTime<<"ms"<<endl
//        <<"原始url"<<connInfo.url<<endl
//        <<"302 DNS解析耗时"<<lastConnInfo.dnsTime<<"ms"<<endl
//        <<"302 TCP连接耗时"<<lastConnInfo.connTime<<"ms"<<endl
//        <<"302 url"<<lastConnInfo.url<<endl;
    
    return 0;
}

int HttpStream::close()
{
    if(sockfd > 0) {
        ::close(sockfd);
        sockfd = 0;
    }
    
//    printf("close http connection\n");
    return 0;
}


int HttpStream::read(char *buf, int len, bool wait)
{
    int needRead = len;
    
read_again:
    
    if(0 == needRead || streamEnd) { //读取完全部body直接返回
        return len - needRead;
    }
    
    long read = 0;
    if(chunkedMode) { //chunked模式传输

        if(chunkSizeUnRead > 0) { //当前chunk还没读完
            
            read = MIN(needRead, chunkSizeUnRead);
            read = MIN(read, recvBufLen);
            
            if(buf) {
                memcpy(buf + len - needRead, recvBuf, read);
            }
            needRead -= read;
            chunkSizeUnRead -= read;
            recvBufLen -= read;
            memmove(recvBuf, recvBuf + read, recvBufLen);

            if(0 == chunkSizeUnRead) {
                //读取完一个chunk
                while(recvBufLen < 2) {
                    long recvLen = recv(sockfd, recvBuf + recvBufLen, recvBufMaxLen - recvBufLen, 0);
                    
                    if(recvLen <= 0) {
                        if(0 == recvLen) {
                            streamEnd = true;
                            goto read_again;
                        }
                        
                        printf("Error: read socket failed. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
                        return -1;
                    }
                    recvBufLen += recvLen;
                    recvBodyLen += recvLen;
                }
                
                //跳过chunk结尾\r\n
                recvBufLen -= 2;
                memmove(recvBuf, recvBuf + 2, recvBufLen);
            }
            
            if(0 == recvBufLen) {
                long recvLen = recv(sockfd, recvBuf + recvBufLen, recvBufMaxLen - recvBufLen, 0);
                
                if(recvLen <= 0) {
                    
                    if(0 == recvLen) {
                        streamEnd = true;
                        goto read_again;
                    }
                    
                    printf("Error: read socket failed. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
                    return -1;
                }
                recvBufLen += recvLen;
                recvBodyLen += recvLen;
            }
            
        } else { //读取新的chunk
            
            int i;
            char chunkSizeStr[512];
            for(i = 0; i < recvBufLen - 1 && (recvBuf[i] != '\r' || recvBuf[i+1] != '\n'); i++) {
                chunkSizeStr[i] = recvBuf[i];
            }
            if(i >= recvBufLen - 1) {
                
                long recvLen = recv(sockfd, recvBuf + recvBufLen, recvBufMaxLen - recvBufLen, 0);
                
                if(recvLen <= 0) {
                    
                    if(0 == recvLen) {
                        streamEnd = true;
                        goto read_again;
                    }
                    
                    printf("Error: read socket failed. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
                    return -1;
                }
                recvBufLen += recvLen;
                recvBodyLen += recvLen;
                goto read_again;
            }
            chunkSizeStr[i] = '\0';
            chunkSizeUnRead = strtol(chunkSizeStr, NULL, 16);
            
            recvBufLen -= i + 2;
            memmove(recvBuf, recvBuf + i + 2, recvBufLen);
            
       //     printf("chunk size : %ld\n", chunkSizeUnRead);
            
            //check chunk size
            if(0 == chunkSizeUnRead) {
                streamEnd = true;
                goto read_again;
            }
            
        }
        
    } else { //非chunked模式
        
        if(recvBufLen > 0) {
            
            read = MIN(needRead, recvBufLen);
            
            if(buf) {
                memcpy(buf + len - needRead, recvBuf, read);
            }
            needRead -= read;
            recvBufLen -= read;
            memmove(recvBuf, recvBuf + read, recvBufLen);
            
        } else {
            
            if(responseBodyLen <= recvBodyLen && !noContentLenAndChunk) {
                streamEnd = true;
                goto read_again;
            }
            
            long recvLen = recv(sockfd, recvBuf + recvBufLen, recvBufMaxLen - recvBufLen, 0);
            
            if(recvLen <= 0) {
                
                if(0 == recvLen) {
                    streamEnd = true;
                    goto read_again;
                }
                
                printf("Error: read socket failed. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
                return -1;
            }
            recvBufLen += recvLen;
            recvBodyLen += recvLen;
            goto read_again;
            
        }
    }
    
    if(wait && needRead > 0) {
        goto read_again;
    }
    
    return len - needRead;
}


int HttpStream::write(const char *buf, int len)
{
    return len;
}

int HttpStream::end()
{
    return streamEnd;
}

int HttpStream::seek(StreamPos pos,  int offset)
{
    return 0;
}



int HttpStream::openPost(const char *url, const char *reqBody)
{
    recvBufLen = 0;
    recvBodyLen = 0;
    responseBodyLen = 0;
    chunkSizeUnRead = 0;
    chunkedMode = false;
    noContentLenAndChunk = false;
    streamEnd = false;
    
    this->url = url;
    
    string host;
    string path;
    int port;
    
    if(parse_url(this->url, host, path, port)) {
        printf("parse url error.\n");
        return -1;
    }
    
    string requestType = "POST";
    string requestText;
    string requestBody = reqBody;

    if(make_request_text(requestType, requestText, requestBody, this->url, host, port, path)) {
        printf("parse url error.\n");
        return -1;
    }
    
 //   printf("%s\n", requestText.c_str());
    
    if(send_http_request(this->url, host, path, Utils::get_host_ip(host, &lastConnInfo.dnsTime), port, requestText)) {
        return -1;
    }
    
//    cout << "post url: " << url << " post data: " << requestText << endl;

    if(responseHeader.find("httpCode")->second.compare("302") == 0) {
        //302跳转
        close();
        string location = responseHeader.find("Location")->second;
        
        printf("Http重定向至%s\n", location.c_str());
        return openPost(location.c_str(), reqBody);
    }
    
    return 0;
}

int HttpStream::parse_url(string &url, string &host, string &path, int &port)
{
    if(url.length() == 0) {
        return -1;
    }
    
    port = 80;
    
    const char *p = url.c_str(), *t;
    
    if(p[4] == ':') {
        p += 7; //跳过http://
    } else {
        return -1;
    }
    
    char buf[1024];
    
    if((t = strchr(p, ':'))) {
        sscanf(t + 1, "%d", &port);
        memcpy(buf, p, t - p);
        buf[t - p] = '\0';
        if(NULL == (t = strchr(t, '/'))) {
            t = "/";
        }
    } else {
        if(NULL == (t = strchr(p , '/'))) {
            t = p + strlen(p);
        }
        memcpy(buf, p, t - p);
        buf[t - p] = '\0';
    }
    
    host = buf;
    path = t;
    
    if(*t == '\0') {
        path = "/";
    }
    
    return 0;
}

int HttpStream::parse_response_header(map<string, string> &header, char *headText, long headLen)
{
    bool first;
    char *p;
    char *t;
    
    char httpVersion[10];
    char httpCode[10];
    
    first = true;
    p = headText;
    header.clear();
    
    while(p - headText < headLen) {
        if(first) {
            first = false;
            sscanf(p, "HTTP/%s %s", httpVersion, httpCode);
            
            header.insert(pair<string, string>(string("httpCode"), string(httpCode)));
            header.insert(pair<string, string>(string("httpVersion"), string(httpVersion)));
            
            while(*p++ != '\n');
        } else {
            
            char key[1024];
            char value[1024];
            
            //获得key
            t = p;
            while(*(++t) != ':');
            
            key[t - p] = '\0';
            memcpy(key, p, t - p);
            
            p = t + 1;
            if(*p == ' ') { p++; }
            
            //获得value
            while(*(++t) != '\r');
            
            value[t - p] = '\0';
            memcpy(value, p, t - p);
            
            header.insert(pair<string, string>(string(key), string(value)));
            
            p = t + 2;
        }
    }
    
    return 0;
}


int HttpStream::make_request_text(string &requestType, string &requestText, string &requestBody, string &url, string &host, int port, string &path)
{
    int len;
    char buf[1024 * 16];
    char portStr[256];
    
    
    sprintf(portStr, ":%d", port);
    
    len = sprintf(buf, "%s %s HTTP/1.1" LRLN
            "Host:%s%s" LRLN
            "Connection:close" LRLN
            "Content-Length:%lu" LRLN
            "Pragma:no-cache" LRLN
            "Cache-Control:no-cache" LRLN
            "Accept:*/*" LRLN
            LRLN //http header end
            "%s",
            requestType.c_str(),
            path.c_str(),
            host.c_str(),
            port == 80 ? "" : portStr,
            requestBody.length(),
            requestBody.c_str()
            );
    
    buf[len] = '\0';
    requestText = buf;
    
    return 0;
}

//在字符串中匹配\r\n\r\n
char *match_http_header_end(char *str, long len)
{
    const char *pattern = "\r\n\r\n";
    const int kmp_state[] = {-1, -1, 0, 1}; //串\r\n\r\n的kmp状态转移数组
    int state = -1;
    
    for(int i = 0; i < len; i++) {
        while(state != -1 && str[i] != pattern[state + 1]) {
            state = kmp_state[state];
        }
        
        if(str[i] == pattern[state + 1]) {
            state++;
            if(state == 3) { //匹配到\r\n\r\n
                return str + i - 3;
            }
        }
    }
    
    return NULL;
}

int HttpStream::send_http_request(string &url, string &host, string &path, uint32_t ip, int port,
                                  string &requestText)
{
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        return -1;
    }
    
    struct sockaddr_in conn_addr;
    bzero(&conn_addr, sizeof(conn_addr));
    conn_addr.sin_family = AF_INET;
    conn_addr.sin_port = htons(port);
    conn_addr.sin_addr = *(struct in_addr *)&ip;
    
    lastConnInfo.connTime = clock_ms();
    if(-1 == connect(sockfd, (struct sockaddr *)&conn_addr, sizeof(conn_addr))) {
        printf("Error: socket connect faild. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
        goto err;
    }
    lastConnInfo.connTime = clock_ms() - lastConnInfo.connTime;
    
    if(-1 == send(sockfd, requestText.c_str(), requestText.length(), 0)) {
        printf("Error: socket send faild. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
        goto err;
    }
    
    int len;
    recvBufLen = 0;

    for(;;) {
        
        if((len = recv(sockfd, recvBuf + recvBufLen, recvBufMaxLen - recvBufLen, 0)) <= 0) {
        	printf("recv error [%d]\n", len);
            goto err;
        }
        
        char *httpHeaderEnd;
        
        if((httpHeaderEnd = match_http_header_end(recvBuf, recvBufLen + len)) != NULL) {
            httpHeaderEnd += 2;
            
            if(-1 == parse_response_header(responseHeader, recvBuf, httpHeaderEnd - recvBuf)) {
            	printf("parse_response_header err\n");
                goto err;
            }

            httpHeaderEnd += 2;
            
            recvBufLen = recvBuf + recvBufLen + len - httpHeaderEnd;
            recvBodyLen = recvBufLen;
            memmove(recvBuf, httpHeaderEnd, recvBufLen);
            
            map<string, string>::iterator it = responseHeader.find("Content-Length");
            if(it != responseHeader.end()) {

                responseBodyLen = atol(it->second.c_str());
            } else {
                
                it = responseHeader.find("Transfer-Encoding");
                if(it != responseHeader.end() && it->second.compare("chunked") == 0) {
                    chunkedMode = true;
                } else {
                    noContentLenAndChunk = true;
                }
            }
            
            break;
        } else if(recvBufLen + len >= recvBufMaxLen) {
            printf("Error: response http header too long. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
            goto err;
        }
        
        recvBufLen += len;
    }

    return 0;
err:
    ::close(sockfd);
    sockfd = 0;
    return -1;
}
