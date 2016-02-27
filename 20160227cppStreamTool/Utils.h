#ifndef __m3u8_monitor__Utils__
#define __m3u8_monitor__Utils__

#include <stdio.h>
#include <stdint.h>
#include <iostream>


extern bool volatile stopFlag;
extern const char *dumpFileName;


//#define DUMP_FLV //dump flv流到文件
#define DUMP_FLV_PATH "/Users/hongduoxing/Desktop/dump_flv/"

class Utils {
    
public:
    static uint32_t get_host_ip(std::string &host, long *dnsTime);
    static clock_t clock_ms();
    
    static bool local_is_little_endian();
    static void to_little_endian(uint8_t *buf, uint8_t *data, int writeLen, int dataLen);
    static void to_big_endian(uint8_t *buf, uint8_t *data, int writeLen, int dataLen);
    static void big_endian_to_local(uint8_t *buf, uint8_t *data, int writeLen, int dataLen);
    static void little_endian_to_local(uint8_t *buf, uint8_t *data, int writeLen, int dataLen);

};

#endif /* defined(__m3u8_monitor__Utils__) */
