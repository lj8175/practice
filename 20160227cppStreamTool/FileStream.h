#ifndef __m3u8_monitor__FileStream__
#define __m3u8_monitor__FileStream__

#include <stdio.h>
#include <iostream>
#include "Stream.h"


class FileStream: public Stream {
    FILE *file;
    std::string filePath;
    
    long fileSize;
public:
    FileStream();
    ~FileStream();
    
    int open(const char*);
    int close();
    
    int read(char*, int, bool = true);
    int write(const char*, int);
    
    int end();
    int seek(StreamPos, int);
};

#endif /* defined(__m3u8_monitor__FileStream__) */
