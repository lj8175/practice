#include "Utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/times.h>


bool volatile stopFlag = false;
const char *dumpFileName = "";


uint32_t Utils::get_host_ip(std::string &host, long *dnsTime)
{
    struct in_addr addr;
    struct hostent *h;
    
    long t = clock_ms();
    
    if((h = gethostbyname(host.c_str()))==NULL) {
        return 0;
    }
    
    t = clock_ms() - t;
    
    memcpy(&addr, h->h_addr_list[0], h->h_length);
    
    printf("remote ip: %s\n", inet_ntoa(addr));
    
    if(dnsTime != NULL) {
        *dnsTime = t;
    }
    
    return addr.s_addr;
}


clock_t Utils::clock_ms()
{
    struct tms t;
    static clock_t clk_tck;
    if (!clk_tck) clk_tck = sysconf(_SC_CLK_TCK);
    return times(&t) * 1000 / clk_tck;
}

bool Utils::local_is_little_endian()
{
    union {
        int a;
        char b;
    }check;
    
    check.a = 1;
    return check.b == 1;
}

void Utils::to_big_endian(uint8_t *buf, uint8_t *data, int writeLen, int dataLen)
{
    /*
     当writeLen >= dataLen时
     buf空出的高位字节补0
     当writeLen < dataLen
     buf写入data的低位，剩余的data高位字节丢弃
     */
    
    int diff = 0;
    int fill0 = 0;
    
    if(writeLen >= dataLen) {
        fill0 = writeLen - dataLen;
    } else {
        diff = dataLen - writeLen;
        dataLen = writeLen;
    }
    
    if(local_is_little_endian()) { //当前为小端
        for(int i = 0; i < dataLen; i++) {
            buf[i + fill0] = data[dataLen - i - 1];
        }
    } else {
        memcpy(buf + fill0, data + diff, dataLen);
    }
    
    if(fill0 > 0) {
        memset(buf, 0, fill0);
    }
}

void Utils::to_little_endian(uint8_t *buf, uint8_t *data, int writeLen, int dataLen)
{
    /*
     当writeLen >= dataLen时
     buf空出的高位字节补0
     当writeLen < dataLen
     buf写入data的低位，剩余的data高位字节丢弃
     */
    
    int diff = 0;
    int fill0 = 0;
    
    if(writeLen >= dataLen) {
        fill0 = writeLen - dataLen;
    } else {
        diff = dataLen - writeLen;
        dataLen = writeLen;
    }
    
    if(!local_is_little_endian()) { //当前为大端
        for(int i = 0; i < dataLen; i++) {
            buf[i] = data[dataLen + diff - i - 1];
        }
    } else {
        memcpy(buf, data, dataLen);
    }
    
    if(fill0 > 0) {
        memset(buf + dataLen, 0, fill0);
    }
}

void Utils::big_endian_to_local(uint8_t *buf, uint8_t *data, int writeLen, int dataLen)
{
    /*
     当writeLen >= dataLen时
     buf空出的高位字节补0
     当writeLen < dataLen
     buf写入data的低位，剩余的data高位字节丢弃
     */

    int diff = 0;
    int fill0 = 0;
    
    if(writeLen >= dataLen) {
        fill0 = writeLen - dataLen;
    } else {
        diff = dataLen - writeLen;
        dataLen = writeLen;
    }
    
    if(local_is_little_endian()) {
        for(int i = 0; i < dataLen; i++) {
            buf[i] = data[dataLen + diff - i - 1];
        }
        
        if(fill0 > 0) {
            memset(buf + dataLen, 0, fill0);
        }
    } else {
        memcpy(buf + fill0, data + diff, dataLen);
        
        if(fill0 > 0) {
            memset(buf, 0, fill0);
        }
    }
}

void Utils::little_endian_to_local(uint8_t *buf, uint8_t *data, int writeLen, int dataLen)
{
    /*
     当writeLen >= dataLen时
     buf空出的高位字节补0
     当writeLen < dataLen
     buf写入data的低位，剩余的data高位字节丢弃
     */
    
    int diff = 0;
    int fill0 = 0;
    
    if(writeLen >= dataLen) {
        fill0 = writeLen - dataLen;
    } else {
        diff = dataLen - writeLen;
        dataLen = writeLen;
    }

    if(!local_is_little_endian()) {
        for(int i = 0; i < dataLen; i++) {
            buf[i + fill0] = data[dataLen - i - 1];
        }
        if(fill0 > 0) {
            memset(buf, 0, fill0);
        }
    } else {
        memcpy(buf, data, dataLen);
        if(fill0 > 0) {
            memset(buf + dataLen, 0, fill0);
        }
    }
}

