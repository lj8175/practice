#include "flv_parser.h"
#include "RtmpStream.h"
#include "FileStream.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/times.h>
using namespace std;


void *stop_client(void *param);
int parseCMD(int argc, const char *argv[]);


int main(int argc, const char * argv[])
{
//    mallopt(M_MMAP_MAX, 0);
//    mallopt(M_TRIM_THRESHOLD, 0);
    
    pthread_t pid;
    if(pthread_create(&pid, NULL, stop_client, NULL) < 0) {
        printf("Error: 创建线程失败.\n");
        return -1;
    }
    
    if(parseCMD(argc - 1, argv + 1)) {
        return -1;
    }
    return 0;
}


int parseCMD(int argc, const char *argv[])
{
    const char *type = "";
    const char *url = "";
    
    if(argc < 4) {
        printf("Error: 输入参数有误，参数示例:\n"
               "http拉流: -type http -i \"http://xl.pull.test.live.p2cdn.com/live/xltest.flv\"\n"
               "rtmp拉流: -type rtmp -i \"rtmp://xl.pull.test.live.00cdn.com/live/xltest\"\n"
               "本地flv文件: -type file -i \"/Users/hongduoxing/Desktop/dump_flv/ads.flv\"\n"
               "导出文件(可选): -o \"/Users/hongduoxing/Desktop/dump_flv/dump.flv\"\n"
               
               "注:为避免写入文件缺失,请输入q终止拉流\n");
        return -1;
    }
    
    for(int i = 0; i < argc; i++) {
        const char **p = NULL;
        if(!strcmp(argv[i], "-type")) {
            p = &type;
        } else if(!strcmp(argv[i], "-i")) {
            p = &url;
        } else if(!strcmp(argv[i], "-o")) {
            p = &dumpFileName;
        } else {
            printf("Error: 未知的参数%s.\n", argv[i]);
            return -1;
        }
        
        if(i < argc - 1) {
            *p = argv[i+1];
            i++;
        } else {
            printf("Error: 参数输入有误.\n");
            return -1;
        }
    }
    
    Stream *stm;
    
    if(!strcmp(type, "file")) {
        stm = new FileStream();
        if(stm->open(url)) {
            printf("Error: 文件打开失败.\n");
            goto end;
        }
        flv_parser parser;
        if(parser.parse_flv(*stm)) {
            printf("Error: 解析flv文件失败.\n");
        }
        
    } else if(!strcmp(type, "http")) {
        stm = new  HttpStream();
        
        if(stm->open(url)) {
            printf("Error: http流打开失败.\n");
            goto end;
        }
        flv_parser parser;
        if(parser.parse_flv(*stm)) {
            printf("Error: 解析flv流失败.\n");
        }
        
    } else if(!strcmp(type, "rtmp")) {
        stm = new RtmpStream;
        ((RtmpStream*)stm)->test(url);
    } else {
        printf("Error: -type 参数错误.\n");
        return -1;
    }
    
end:
    stm->close();
    delete stm;
    
    return 0;
}

void *stop_client(void *param)
{
    char ch;
    do {
        ch = getchar();
        if(ch == 'Q' || ch == 'q') {
            stopFlag = true;
            break;
        }
    }while(ch);
    return NULL;
}
