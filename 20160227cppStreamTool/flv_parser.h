#ifndef __flv_server__flv_parser__
#define __flv_server__flv_parser__

#include <stdio.h>
#include <stdint.h>
#include "HttpStream.h"
#include "VideoCodec.h"
#include "AudioCodec.h"



#define FLV_HEADER_SIZE 9

#define FLV_TAG_HEADER_SIZE 11


class flv_header {

public:
    uint8_t flvHeaderBytes[9];
    
    uint8_t version;
    
#define FLV_FLAG_AUDIO_MASK 0x01
#define FLV_FLAG_VIDEO_MASK 0x02

    uint8_t flag;
    uint32_t headerLen;
    
    int flv_parse_header(Stream&);
};


class flv_script_tag {
public:
    int parse_script_tag(uint8_t*, uint32_t);
    
private:
    int flv_script_string_parser(uint8_t *buf, uint32_t bufLen, char *strBuf, uint32_t strMaxLen);
    int flv_script_array_parser(uint8_t *buf, uint32_t bufLen);
    int flv_script_data_parser(uint8_t *sBuf, uint32_t sLen);
    
};


class flv_tag {

public:
    uint8_t *tagBytesData; //保存该tag的二进制数据
    
    /*
     Type of contents in this tag. The following types are
     defined:
     8 = audio
     9 = video
     18 = script data
     */

#define FLV_AUDIO_TAG 0x08
#define FLV_VIDEO_TAG 0x09
#define FLV_SCRIPT_TAG 0x12
    
    uint8_t tagType;
    
    uint32_t dataSize;
    
    uint32_t tagSize;
    
    uint32_t timestamp;
    
    uint32_t streamID;
    
    uint8_t *data;
    
    VideoCodecHeader *videoHeader;
    AudioCodecHeader *audioHeader;
    flv_script_tag *scriptTag;
    
    flv_tag();
    ~flv_tag();
    
    int flv_parse_tag(Stream&);
};


class flv_parser {
public:
    flv_header header;
    
    int parse_flv(Stream&);
    int repair_flv_file(const char *);
};


#endif /* defined(__flv_server__flv_parser__) */
