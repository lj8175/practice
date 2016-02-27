#ifndef __m3u8_monitor__Stream__
#define __m3u8_monitor__Stream__

#include <stdio.h>


enum StreamPos {
    STREAM_SET = 0,
    STREAM_CUR,
    STREAM_END
};

enum StreamRetCode {
    STREAM_OK = 0,
    STREAM_FAILED,
    
};

class Stream {
public:
    Stream() {}
    virtual ~Stream(){}
    
    /*
     function: 打开流
     param: 流路径
     return: 0成功，非0失败
     */
    virtual int open(const char*) = 0;
    
    /*
     function: 关闭流
     return: 0成功，非0失败
     */
    virtual int close() = 0;
    
    /*
     function: 读取流
     param: 接收buf, 读取长度
     return:
     */
    virtual int read(char*, int, bool=true) = 0;
    virtual int write(const char*, int) = 0;
    
    virtual int end() = 0;
    virtual int seek(StreamPos, int) { return 0; }
};

#endif /* defined(__m3u8_monitor__Stream__) */
